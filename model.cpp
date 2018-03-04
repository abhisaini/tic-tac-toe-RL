#include <iostream>
#include <vector>
#include <stdlib.h>

// model No.'s
#define WIN 1
#define BLOCK 2
#define FORK 3
#define OPP_FORK_BLOCK 4
#define CENTER 5
#define OPP_CORNER 6
#define EMPTY_CORNER 7
#define EMPTY_SIDE 8
#define OTHERS 9

// policies
#define GREEDY 0
#define EXPLORATORY 1

// players
#define PLAYER_X 1
#define PLAYER_O -1

// Gameresults
#define EMPTY 0
#define LOSE 0
#define DRAW 4

typedef std::vector<std::vector<int> > Matrix;

class state { // inorder to describe  a state with the positions of X and O and its value function
	public:
		int gridSize;
		Matrix mat;
		float val;
		int config;
		int turnCount;
		state(int value){
			gridSize = value;
			mat.resize(gridSize);
			for (int i = 0; i < gridSize; i++)
			    mat[i].resize(gridSize);
			val = 0.5;
			config = OTHERS;
			turnCount = 0;
		}
		~state(void){}
};

typedef std::vector<state> stateArray;

class model{
	public:
		int config;
		int turnCount;
		stateArray array;
		model(int value, int count) {
			config = value;
			turnCount = count;
		}
		~model(void){}
};

std::vector<model> models;

int rowSum(Matrix mat, int rowIndex, int player) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
		if (mat[rowIndex][i] == player) sum++;
  }
  return sum;
}

int colSum(Matrix mat, int colIndex, int player) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    if (mat[i][colIndex] == player) sum++;
  }
  return sum;
}

int diaSum(Matrix mat, int direction, int player) {

	int sum = 0;
	for (int i = 0; i < 3; i++) {
		if (direction == 1) {
			if (mat[i][i] == player) sum++;
		}
		else {
			if (mat[i][2-i] == player) sum++;
		}
	}
	return sum;
}

int whoseTurn(state const& S) {

	int sum1 = 0;
	int sum2 = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (S.mat[i][j] == 1) sum1++;
			else if (S.mat[i][j] == -1) sum2++;
		}
	}
	if (sum1 == sum2) return PLAYER_X;
	else return PLAYER_O;
}

int countTurns(state const& S) {

	int sum1 = 0;
	int sum2 = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (S.mat[i][j] == 1) sum1++;
			else if (S.mat[i][j] == -1) sum2++;
		}
	}
	if (sum1 == sum2) return sum1;
	else return -1;
}

void printMat(int gridSize, Matrix mat){
    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            std::cout << mat[i][j];
						if(j<gridSize-1){
				 			std::cout<< " |";
						}
        }
        if(i < gridSize - 1) std::cout << std::endl << "--------" << std::endl;
    }
    std::cout <<std::endl;
}

void reSort(state& State) {
	state S(State.gridSize);
	S = State;

	int i = 0;
	for (i = 0; i < models.size(); i++) {
		if ((models[i].config == S.config) && (models[i].turnCount == S.turnCount)) {
			for (int j = 0; j < models[i].array.size(); j++) {
				if (S.mat == models[i].array[j].mat) { models[i].array.erase(models[i].array.begin() + j); break; }
			}
			break;
		}
	}

	for (int j = 0; j < models[i].array.size(); j++) {
			if (S.val >= models[i].array[j].val) {
				models[i].array.insert(models[i].array.begin() + j, S);
				break;
		 }
	}

	return;
}

void dummyfunction(state S) {

	std::cout << "S.val : " << S.val << " , " << "S.config : " << S.config << " , " << "S.turnCount : " << S.turnCount << '\n';

}

int winClassifier(state const& S, int player) {

  for (int i = 0; i < 3; i++) {
    if (((rowSum(S.mat, i, player) == 2)&&(rowSum(S.mat, i, -1*player) == 0))||((colSum(S.mat, i, player) == 2)&&(colSum(S.mat, i, -1*player) == 0))) {
      return 1;
    }
  }

  if (((diaSum(S.mat, 1, player) == 2)&&(diaSum(S.mat, 1, -1*player) == 0))||((diaSum(S.mat, -1, player) == 2)&&(diaSum(S.mat, -1, -1*player) == 0))) return 1;

  return 0;

}

int forkedClassifier(state const& S, int player) {


	int winCount = 0;
  for (int i = 0; i < 3; i++) {
    if ((rowSum(S.mat, i, player) == 2) && (rowSum(S.mat, i, -1*player) == 0)) winCount++;
    if ((colSum(S.mat, i, player) == 2) && (colSum(S.mat, i, -1*player) == 0)) winCount++;
  }

  if ((diaSum(S.mat, 1, player) == 2) && (diaSum(S.mat, 1, -1*player) == 0)) winCount++;
  if ((diaSum(S.mat, -1, player) == 2) && (diaSum(S.mat, -1, -1*player) == 0)) winCount++;

  if (winCount > 1) return 1;
	else return 0;

}

int forkClassifier(state const& S, int player) {

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (S.mat[i][j] == 0) {
				state S1(3);
				S1 = S;
				S1.mat[i][j] = player;
				if (forkedClassifier(S1, player)) return 1;
			}
		}
	}

	return 0;

}

int emptyCornerClassifier(state const& S, int player) {

	if ((S.mat[0][0] == 0 && S.mat[2][2] == 0)||(S.mat[2][0] == 0 && S.mat[0][2] == 0)) return 1;
	else return 0;
}

int oppCornerClassifier(state const& S, int player) {

	if ((S.mat[0][0] == -1*player)&&(S.mat[2][2] == 0)) return 1;
	else if ((S.mat[0][0] == 0)&&(S.mat[2][2] == -1*player)) return 1;
	else if ((S.mat[2][0] == -1*player)&&(S.mat[0][2] == 0)) return 1;
	else if ((S.mat[0][2] == -1*player)&&(S.mat[2][0] == 0)) return 1;
	else return 0;
}

int emptyClassifier(state const& S, int player) {
  int sum1 = 0;
	int sum2 = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        if (S.mat[i][j] == player) sum1 = sum1 + S.mat[i][j];
				else sum2 = sum2 + S.mat[i][j];
    }
  }
  if (sum1 == 0 && sum2 ==0) return 1;
  else return 0;
}

int emptySideClassifier(state const& S, int player) {

	if ((rowSum(S.mat, 2, player) == 0&&rowSum(S.mat, 2, -1*player) == 0)||(colSum(S.mat, 2, player) == 0&&colSum(S.mat, 2, -1*player) == 0)||(rowSum(S.mat, 0, player) == 0&&rowSum(S.mat, 0, -1*player) == 0)||(colSum(S.mat, 0, player) == 0&&colSum(S.mat, 0, -1*player) == 0)) return 1;
  return 0;
}

// to classify a state in any of the above models
int modelClassifier(state const& S, int player) {

  if (winClassifier(S, player)) return WIN;
  else if (winClassifier(S, -1*player)) return BLOCK;
	else if (forkClassifier(S, player)) return FORK;
	else if (forkClassifier(S, -1*player)) return OPP_FORK_BLOCK;
	else if (emptyClassifier(S, player)) return CENTER;
	else if (emptyCornerClassifier(S, player)) return EMPTY_CORNER;
	else if (oppCornerClassifier(S, player)) return OPP_CORNER;
	else if (emptySideClassifier(S, player)) return EMPTY_SIDE;
	else return OTHERS;
}

void getVal (state& S, int turnCount, int modelNo) {

	for (int i = 0; i < models.size(); i++) {
		if (models[i].config == modelNo && models[i].turnCount == turnCount) {
			model M(models[i].config, models[i].turnCount);
			M = models[i];
			for (int j = 0; j < M.array.size(); j++) {
				if (S.mat == M.array[j].mat) {S.val = M.array[j].val; return;}
				else continue;
			}
			S.val = 0.5;
			models[i].array.push_back(S);
			return;
		}
		else continue;
	}
	model M(modelNo, turnCount);
	models.push_back(M);
	M.array.push_back(S);

}

// the functions counts the positions where a player can move
int spaceRandom(state const &S) {
    int sz = S.mat.size();
    int val = 0;
    for(int i = 0; i < sz; i++) {
       for(int j = 0; j < sz; j++) {
           if(S.mat[i][j] != 0) {
               val++;
           }
       }
   }
    return sz*sz-val;
}

//function will take the array containing the position of vacancies as input and returns a random vacancy
int randomInput(int arr[], int length){

	// srand(time(NULL)); // can use this to decrease the training time
  int input=rand()%length;
  return arr[input];
}

//it will make the move i.e it takes the state,vacancy and player(1 or -1) as input and changes the vacancy to player
void randomChanged(state &S, int move, int player){

	int sz = S.mat.size();
	int moveX=move/sz, moveY=move%sz;
  S.mat[moveX][moveY]= player;
  return;
}

void randomMove(state &S1, state &S2, int player){
    int space = spaceRandom(S1);
    int sz = S1.mat.size();
    int i = 0, j = 0, index = 0;
    int freespace[space];
    for(i = 0; i < sz; i++){
        for(j = 0; j < sz; j++){
            if(S1.mat[i][j] == 0){
                freespace[index]=sz*i+j;
                index++;
            }
        }
    }
    int move=randomInput(freespace,space);
		S2 = S1;
    randomChanged(S2, move, player);
    return;
}

int gameOver(Matrix mat) {
	int emptySpaceCount = 0;
	for (int i = 0; i < mat.size(); i++) {
		for (int j = 0; j < mat.size(); j++) {
				if (mat[i][j] != 0) emptySpaceCount++;
		}
	}
	if (emptySpaceCount == 0) return DRAW;

	for (int i = 0; i < mat.size(); i++) {
		if (rowSum(mat, i, PLAYER_O) == 3||colSum(mat, i, PLAYER_O) == 3) return LOSE;
		else if (rowSum(mat, i, PLAYER_X) == 3||colSum(mat, i, PLAYER_X) == 3) return WIN;
		else continue;
	}

	if (diaSum(mat, 1, PLAYER_O) == 3||diaSum(mat, -1, PLAYER_O) == 3) return LOSE;
	else if (diaSum(mat, 1, PLAYER_X) == 3||diaSum(mat, -1, PLAYER_X) == 3) return WIN;
	else return 2;
}

state* nextMove(state& S1, state& S2, int player, int policy, int gridSize, bool train){ // policy is only for PLAYER_X

    if (player == PLAYER_X) {
				std::cout << "PLAYER_X's turn" << '\n';
        int modelNo;
        int turnCount;
        if (policy == GREEDY) {
					std::cout << "GREEDY policy choosen by PLAYER_X" << '\n';
					modelNo = modelClassifier(S1, PLAYER_X);
					turnCount = countTurns(S1);
					int i = 0;
					for (i = 0; i < models.size(); i++) {
						if (models[i].config == modelNo && models[i].turnCount == turnCount) {
							printMat(3, models[i].array[0].mat);
							return &models[i].array[0];
						}
						else continue;
					}

					randomMove(S1, S2, PLAYER_X);
					S2.config = modelNo;
					S2.turnCount = turnCount;
					if (train) {
						model M(modelNo, turnCount);
						std::cout << "Built a model with ModelNo : " << modelNo << " and turnCount : " << turnCount << '\n';
						M.array.push_back(S2);
						models.push_back(M);
					}
					printMat(3, S2.mat);
					return &models[i].array[0];

        }
        else {
					std::cout << "EXPLORATORY policy choosen by PLAYER_X" << '\n';
          randomMove(S1, S2, PLAYER_X);
          modelNo = modelClassifier(S1, PLAYER_X);
          turnCount = countTurns(S1);
					getVal(S2, modelNo, turnCount);
					S2.config = modelNo;
					S2.turnCount = turnCount;
					if (train) {
						model M(modelNo, turnCount);
						std::cout << "Built a model with ModelNo : " << modelNo << " and turnCount : " << turnCount << '\n';
						M.array.push_back(S2);
						models.push_back(M);
					}
					printMat(3, S2.mat);
          return NULL;
        }
    }

    else {
			std::cout << "PLAYER_O's turn" << '\n';
      randomMove(S1, S2, PLAYER_O);
			std::cout << "EXPLORATORY policy choosen by PLAYER_X" << '\n';
			printMat(3, S2.mat);
			return NULL;
    }
}

void playGame(float epsilon, float alpha, int gridSize){

    state oState(gridSize);
		state xState(gridSize);
    state* xStatePtr = NULL;
		state* dummyStatePtr = NULL	;
		state dummyState(gridSize);
    int turns = int (1/epsilon);
    int turn_count = 0;
    int policy;

    while(1){
        // decide the policy in this game turn
        if (epsilon)
				{
					turn_count++;
					if (!(turn_count%turns)) policy = EXPLORATORY;
					else policy = GREEDY;
				}
				else policy = GREEDY;

				xStatePtr = nextMove(oState, xState, PLAYER_X, policy, 3, true);
				std::cout << "-------------------------------------------------" << '\n';
				if (turn_count != 1) dummyStatePtr->val = dummyStatePtr->val + alpha*(xStatePtr->val - dummyStatePtr->val); // update
				if (turn_count != 1) reSort(dummyState);
				if (gameOver(xStatePtr->mat) == WIN||gameOver(xStatePtr->mat) == DRAW) break;
				nextMove(xState, oState, PLAYER_O, EXPLORATORY, 3, true);
				std::cout << "-------------------------------------------------" << '\n';
				dummyStatePtr = xStatePtr;
				dummyState.mat = dummyStatePtr->mat;
				dummyState.val = dummyStatePtr->val;
				dummyState.config = dummyStatePtr->config;
				dummyState.turnCount = dummyStatePtr->turnCount;
				if (gameOver(oState.mat) == LOSE||gameOver(oState.mat) == DRAW) break;

    }
		return;
}

int game(int gridSize){ // only plays not update, output will be win or loose

	state oState(gridSize);
	state xState(gridSize);
	int gameRes;
	while(1){

		nextMove(oState, xState, PLAYER_X, GREEDY, 3, false);
		int gameRes = gameOver(xState.mat);
		if ((gameRes == WIN)||(gameRes == DRAW)) return gameRes;

		nextMove(xState, oState, PLAYER_O, EXPLORATORY, 3, false);
		gameRes = gameOver(oState.mat);
		if ((gameRes == LOSE)||(gameRes == DRAW)) return gameRes;

	}
}

int main () {

	float epsilon, alpha;
	int gridSize, trains;
	int checkCount;

	std::cout << "Enter epsilon : " << '\n';
	std::cin >> epsilon;
	std::cout << "Enter alpha : " << '\n';
	std::cin >> alpha;
	std::cout << "Enter gridSize : " << '\n';
	std::cin >> gridSize;
	std::cout << "Enter the no. of games to train : " << std::endl;
	std::cin >> trains;
	std::cout << "Enter checkCount : " << std::endl;
	std::cin >> checkCount;

	std::vector<float> winPercent;
	std::vector<float> drawPercent;
	std::vector<float> notLost;
	std::vector<float> lostPercent;

	for (int i = 0; i < trains; i++) {

		playGame(epsilon, alpha, gridSize);

		float gamesWin = 0;
		float gamesDraw = 0;
		float gamesLose = 0;

		for (int j = 0; j < checkCount; j++) {

			int gameResult = game(gridSize);
			if (gameResult == WIN) gamesWin++;
			else if (gameResult == LOSE) gamesLose++;
			else if (gameResult == DRAW) gamesDraw++;

		}

		winPercent.push_back(100*(float(gamesWin)/checkCount));
		drawPercent.push_back(100*(float(gamesDraw)/checkCount));
		notLost.push_back(100*(float(gamesDraw)/checkCount + float(gamesWin)/checkCount));
		lostPercent.push_back(100*(float(gamesLose)/checkCount));

		std::cout << i << "th training :=  Win Percent : " << winPercent[i] << " | Draw Percent :" << drawPercent[i] << " | Lost Percent :" << lostPercent[i] << std::endl;

	}
	playGame(0.1, 0.5, 3);
	return 0;
}
