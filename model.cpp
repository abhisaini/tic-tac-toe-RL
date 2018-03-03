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
	model(int value) {
		config = value;
	}
	~model(void){}
};

typedef std::vector<model> models;

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

// void pushInOrder(state const& S, model A) {
// 	state S1;
// 	equate(S, S1);
// 	for(int i = A.array.size()-1; i >= 0; i--) {
// 			if (A.array[i].val <= S1.val) A.array.insert(A.array.begin() + i, S1); break;
// 	}
// 	return;
// }

void printMat(int gridSize, Matrix mat){
    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            std::cout << mat[i][j];
						if(j<gridSize-1){
				 			std::cout<< " |";
						}
        }
        if(i < gridSize - 1) std::cout << std::endl << "------------" << std::endl;
    }
    std::cout <<std::endl;
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
    if ((rowSum(S.mat, i, player) == 2) && (rowSum(S.mat, i, -1*player) == 0)) { printMat(3, S.mat); winCount++; std::cout << "row " << i << std::endl; }
    if ((colSum(S.mat, i, player) == 2) && (colSum(S.mat, i, -1*player) == 0)) { printMat(3, S.mat); winCount++; std::cout << "col " << i << std::endl; }
  }

  if ((diaSum(S.mat, 1, player) == 2) && (diaSum(S.mat, 1, -1*player) == 0)) { printMat(3, S.mat); winCount++; std::cout << "dia " << 1 << std::endl; }
  if ((diaSum(S.mat, -1, player) == 2) && (diaSum(S.mat, -1, -1*player) == 0)) { printMat(3, S.mat); winCount++; std::cout << "dia " << -1 << std::endl; }

  if (winCount > 1) {

		std::cout << "wincount : " << winCount << std::endl;
		return 1;
  }
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

state* nextMove(state& S1, state& S2, int player, int policy, int gridSize){ // policy is only for PLAYER_X

    // move of PLAYER_X
    // greedy or random
    if (player == PLAYER_X) {

        int modelNo;
        int turnCount;
        if (policy == GREEDY) {
					modelNo = modelClassifier(S1, PLAYER_X);
					turnCount = countTurns(S1);
					int i = 0;
					for (i = 0; i < models.size(); i++) {
						if (models[i].config == modelNo && models[i].turnCount == turnCount) {
							return &models[i].array[0];
						}
						else continue;
					}
					model M(modelNo, turnCount);
					models.push_back(M);
					randomMove(S1, S2, PLAYER_X);
					M.array.push_back(S2);
					return &models[i].array[0];

        }
        else {
          randomMove(S1, S2, PLAYER_X);
          modelNo = modelClassifier(S1, PLAYER_X);
          turnCount = countTurns(S1);
          // get the val of the current state
					getVal(S2, modelNo, turnCount);
          return NULL;
        }
    }

    // move of PLAYER_O
    // random
    else {
        randomMove(S1, S2, PLAYER_O);
        return NULL;
    }
}

void playGame(float epsilon, float alpha, int gridSize){

    state oState(gridSize);
		state xState(gridSize);
    state* xStatePtr(gridSize);
		state* dummyState(gridSize);

    int turns = int (1/epsilon);
    int turn_count = 0;
    int policy;

    while(1){
        // decide the policy in this game turn
        if (epsilon)
				{
					turn_count++;
					if (!(j%turns)) policy = EXPLORATORY;
					else policy = GREEDY;
				}
				else policy = GREEDY;

        xStatePtr = nextMove(oState, xState, policy, PLAYER_X);
				dummyStatePtr->val = dummyStatePtr->val + alpha*(xStatePtr->val - dummyStatePtr->val); // update
				if (gameOver(xStatePtr->mat) == WIN||gameOver(xStatePtr->mat) == DRAW) break;
				nextMove(xState, oState, EXPLORATORY, PLAYER_O);
				dummyStatePtr = xStatePtr;
				if (gameOver(oState.mat) == WIN||gameOver(oState.mat) == DRAW) break;

    }
		return;
}

int main () {

	state S(3);
	// S.mat[0][0] = PLAYER_X;
	S.mat[1][0] = PLAYER_O;
	S.mat[2][0] = PLAYER_X;
	S.mat[2][2] = PLAYER_O;
	S.mat[2][1] = PLAYER_X;
	// S.mat[2][2] = PLAYER_O;
	printMat(3, S.mat);
	// std::cout << rowSum(S.mat, 2, PLAYER_X) << '\n';
	// std::cout << rowSum(S.mat, 2, PLAYER_O) << '\n';
	int modelNo = modelClassifier(S, PLAYER_X);
	std::cout << "modelNo is : " << modelNo << std::endl;
	// std::cout << whoseTurn(S) << std::endl;

	return 0;
}
