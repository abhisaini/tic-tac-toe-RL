#include <iostream>
#include <bits/stdc++.h>
#include "matplotlibcpp.h"
#define WIN 1
#define BLOCK 2
#define FORK 3
#define OPP_FORK_BLOCK 4
#define CENTER 5
#define OPP_CORNER 6
#define EMPTY_CORNER 7
#define EMPTY_SIDE 8
#define OTHERS 9
#define GREEDY 0
#define EXPLORATORY 1
#define PLAYER_X 1
#define PLAYER_O -1
#define EMPTY 0
#define LOSE 0
#define DRAW 4

using namespace std;
namespace plt = matplotlibcpp; // namespace for plotting
typedef std::vector<int> Vector;
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
	}
	~state(void){}
};

std::vector<state> stateArray;

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

// posStateArray win(1);
// posStateArray block(2);
// posStateArray fork(3);
// posStateArray oppForkBlock(4);
// posStateArray center(5);
// posStateArray oppCorner(6);
// posStateArray emptyCorner(7);
// posStateArray emptySide(8);
// posStateArray others(9);

std::vector<model> models;

int rowSum(state const& S, int rowIndex, int player) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
		if (S.mat[i][j] == player) sum = sum + S.mat[rowIndex][i];
  }
  return sum;
}

int colSum(state const& S, int colIndex, int player) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    if (S.mat[i][j] == player) sum = sum + S.mat[i][colIndex];
  }
  return sum;
}

int diaSum(state const& S, int direction, int player) {

	int sum = 0;
	for (int i = 0; i < 3; i++) {
		if (direction == 1) {
			if (S.mat[i][i] == player) sum = sum + S.mat[i][i];
		}
		else {
			if (S.mat[i][2-j] == player) sum = sum + S.mat[i][2-i];
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
	if (sum1 == sum2) return 1;
	else return -1;
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

void pushInOrder(state const& S, model A) {
	state S1;
	equate(S, S1);
	for(int i = A.array.size()-1; i >= 0; i--) {
			if (A.array[i].val <= S1.val) A.array.insert(A.array.begin() + i, S1); break;
	}
	return;
}

void classifyPush(state const& S) {

	int modelNo = modelClassifier(S, PLAYER_X);
	int turns = countTurns(S); // only for PLAYER_X
	for (int i = 0; i < models.size(); i++) {
		if (models[i].config == modelNo && models[i].turnCount == turns) {
			pushInOrder(S, models[i]);
		}
		else {
      state S1(3);
      equate(S, S1);
			model A(modelNo);
			A.turnCount = turns;
			models.push_back(A);
			A.array.push_back(S1);
		}
	}
	return;

}

// int countTurns(state const& S, int player) {
//
// 	int count = 0;
// 	for (int i = 0; i < 3; i++) {
// 		for (int j = 0; j < 3; j++) {
// 			if (S.mat[i][j] == player) count++;
// 		}
// 	}
// 	return count;
// }

int winClassifier(state const& S, int player) {

  for (int i = 0; i < 3; i++) {
    if ((rowSum(S, i, player) == 2*player)||(colSum(S, i, player) == 2*player) {
      return 1;
    }
  }

  if ((diaSum(S, 1, player)==2*player)||(diaSum(S, -1, player)==2*player)) return 1;

  return 0;

}

// int blockClassifier(state const& S, int player) {
//
//   for (int i = 0; i < 3; i++) {
//     if ((rowSum(S, i) == -2*player)||(colSum(S, i) == -2*player) {
//       return 1;
//     }
//   }
//
//   if ((diaSum(S, 1) == -2*player)||(diaSum(S, -1) == -2*player)) return 1;
//
//   return 0;
//
// }

int forkedClassifier(state const& S, int player) {

  winCount = 0;
  for (int i = 0; i < 3; i++) {
    if (rowSum(S, i, player) == 2*player) winCount++;
    if (colSum(S, i, player) == 2*player) winCount++;
  }
  if (diaSum(S, 1, player) == 2*player) winCount++;
  if (diaSum(S, -1, player) == 2*player) winCount++;

  if (winCount > 1) return 1;
  else return 0;

}

int forkClassifier(state const& S, int player) {

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (S.mat[i][j] == 0) {
				state S1;
				equate(S, S1);
				S1.mat[i][j] = player;
				if (forkedClassifier(S1, player)) return 1;
			}
		}
	}

	return 0;

}

// int centerClassifier(state const& S, int player) {
//   int sum = 0;
//   for (int i = 0; i < 3; i++) {
//     for (int j = 0; j < 3; j++) {
//         sum = sum + S.mat[i][j];
//     }
//   }
//
//   if ((sum == player)&&(S.mat[1][1] == player)) return 1;
//   else return 0;
// }

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

int emptySideClassifier(state const&, int player) {

	if ((rowSum(S, 2, player) == 0&&rowSum(S, 2, -1*player) == 0)||(colSum(S, 2, player) == 0&&colSum(S, 2, -1*player) == 0)||(rowSum(S, 0, player) == 0&&rowSum(S, 0, -1*player) == 0)||(colSum(S, 0, player) == 0&&colSum(S, 0, -1*player) == 0)) return 1;
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

bool compareMatrices(Matrix mat1, Matrix mat2){
	for (int i = 0; i < mat1.size(); i++) {
		for (int j = 0; j < mat1.size(); j++){
			if (mat1[i][j] != mat2[i][j]) return false;
		}
	}
	return true;
}

std::vector<state> stateArray; // array to store all the states possible

// function to check value of states which are not stored in the stateArray
float getValUnknown(Matrix arr)
{
	int sz = arr.size();
	float val = 0.5;
  //checking for sz 1s or sz -1s in every row
  for(int i = 0; i < sz; i++){
  	int sum = 0;
    for(int j = 0; j < sz; j++){
      sum = sum + arr[i][j];
    }
    if(sum == sz){
      return 1;
    }
    if(sum == -sz){
      return 0;
    }
  }

  //checking for sz 1s or sz -1s in a column
  for(int i = 0; i < sz; i++){
    int sum = 0;
  	for (int j = 0; j < sz; j++) {
			sum = sum + arr[j][i];
    }
    if(sum == sz){
      return 1;
    }
    if(sum == -sz){
        return 0;
    }
	}
	//checking sz 1s or -1s in  diagonal
  int sumd1 = 0, sumd2 = 0;
  for (int i = 0; i < sz; i++) {
		sumd1 = sumd1 + arr[i][i];
    sumd2 = sumd2 + arr[i][sz-1-i];
  }

	if((sumd1 == sz)||(sumd2 == sz)) return 1;
  if((sumd1 == sz)||(sumd2 == sz)) return 0;

	return val;
}

// gives the value function of the function
float getVal(state s){
	for (int i = 0; i < stateArray.size(); i++) {
		if (compareMatrices(s.mat, stateArray[i].mat)) {
			return stateArray[i].val;
		}
	}
	return getValUnknown(s.mat);
}

void equate(state &state1, state &state2) {
	for (int i = 0; i < state1.mat.size(); i++) {
		for (int j = 0; j < state1.mat.size(); j++) {
			state2.mat[i][j] = state1.mat[i][j];
		}
	}
	state2.val = state1.val;
}

// the functions counts the positions where a player can move
int spaceRandom(state const &s) {
    int sz = s.mat.size();
    int val = 0;
    for(int i = 0; i < sz; i++) {
       for(int j = 0; j < sz; j++) {
           if(s.mat[i][j] != 0) {
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
void randomChanged(state &S1, state &S2, int move, int player){

	int sz = S1.mat.size();
	int moveX=move/sz, moveY=move%sz;
  equate(S1, S2);

  S2.mat[moveX][moveY]= player;
  return;
}

void randomMove(state &S, state &S1, int player){
    int space = spaceRandom(S);
    int sz = S.mat.size();
    int i = 0, j = 0, index = 0;
    int freespace[space];
    for(i = 0; i < sz; i++){
        for(j = 0; j < sz; j++){
            if(S.mat[i][j] == 0){
                freespace[index]=sz*i+j;
                index++;
            }
        }
    }
    int move=randomInput(freespace,space);

    randomChanged(S, S1, move, player);
		if (player == PLAYER_X) {S1.val = getVal(S1);}
    return;
}

int GameOver(state &arr){

    float val=getValUnknown(arr.mat);
    int vacancy=spaceRandom(arr);
    if(vacancy==0&&val==0.5) return DRAW;
    if(val!=0.5) return val;//will return LOSE if lost and WIN if won;
    else if(vacancy>0&&val==0.5) return 2;//2 stands for game not over yet

}

string OX(int a){
	string empty = "  ", xOccupied = " x", oOccupied = " o";
	if (a == 0) return empty;
	if (a == 1) return xOccupied;
	if (a == -1) return oOccupied;
	else return to_string(a);
}

void printMat(int gridSize, Matrix mat){
    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            cout << OX(mat[i][j]);
						if(j<gridSize-1){
				 			cout<< " |";
						}
        }
        if(i < gridSize - 1) cout << endl << "--------------------" << endl;
    }
    cout <<endl<< "___________________" << endl;
}

void nextMove (state &currState, state &nextState, int policy, int player){

	float largestValue = 0;
	int gridSize = currState.mat.size();
	// state dummyState(gridSize);
	if (policy == GREEDY) {
		for (int i = 0; i < gridSize; i++) {
			for (int j = 0; j < gridSize; j++) {
				if (currState.mat[i][j] == 0){
          // modelClassifier(currState);
					equate(currState, nextState);
					nextState.mat[i][j] = 1;
					nextState.val = getVal(nextState);
					if (nextState.val >= largestValue){
						largestValue = nextState.val;
						// equate(nextState, dummyState);
					}
				}
			}
		}
		std::vector<int> array;
		int count = 0;
		for (int i = 0; i < gridSize; i++) {
			for (int j = 0; j < gridSize; j++) {
				if (currState.mat[i][j] == 0){
					equate(currState, nextState);
					nextState.mat[i][j] = 1;
					nextState.val = getVal(nextState);
					if (nextState.val == largestValue){
						array.push_back(3*i + j);
						count++;
					}
				}
			}
		}
		int freespace[count];
		for (int i = 0; i < count; i++){
		    freespace[i] = array[i];
		}
		// equate(dummyState, nextState);
		int move = randomInput(freespace, count);
        randomChanged(currState, nextState, move, PLAYER_X);
		nextState.val = getVal(nextState);

	}
	else {
		randomMove(currState, nextState, player);
	}
}

int getStateIndex(state &State){

	for (int i = 0; i < stateArray.size(); i++) {
		if (compareMatrices(State.mat, stateArray[i].mat)) return i;
	}
	return -1;
}

void backUp(state &prevState, state &currState, float alpha){ // assuming prevState and currState has already been pushed in the stateArray
	prevState.val = prevState.val + alpha*(currState.val - prevState.val);
}

void pushBack(state S){
	int gridSize = S.mat.size();
	state S1(gridSize);
	equate(S, S1);
	stateArray.push_back(S1);
}

bool alreadyExist(state S){

	for (int i = 0; i < stateArray.size(); i++){
		if (compareMatrices(stateArray[i].mat, S.mat)){
		    return true;
		}
	}
	return false;
}

int win = 0;
int loss = 0;
int draw = 0;

void playGame(float epsilon, float alpha, int gridSize){ // plays a game, objective is to update the stateArray and value function table

	state oState(gridSize);
	state xState(gridSize);
	state dummyState(gridSize);

	int turns = int (1/epsilon);
	int j = 0;
	int policy;

	while(1){

		if (epsilon)
		{
			j++;
			if (!(j%turns)){ policy = EXPLORATORY;}
			else {policy = GREEDY;}
		}
		else {policy = GREEDY;}

		nextMove(oState, xState, policy, PLAYER_X); // next move exclusively for PLAYER_X
		int gameRes = GameOver(xState);
		if (!alreadyExist(xState)) pushBack(xState);
		int i = getStateIndex(dummyState);
		if (i != -1 && policy == GREEDY ) backUp(stateArray[i], xState, alpha);
		if ((gameRes == WIN)||(gameRes == DRAW)) {
		    if (gameRes == WIN) win++;
		    else draw++;
		    break;
		}

		equate(xState, dummyState);

		// nextMove(xState, oState, EXPLORATORY, PLAYER_O);
    randomMove(xState, oState, PLAYER_O);
    gameRes = GameOver(oState);
		if ((gameRes == LOSE)||(gameRes == DRAW)) {
				if (gameRes == LOSE) loss++;
				else draw++;
		    break;
		}
	}
}

int game(int gridSize){ // only plays not update, output will be win or loose

	state oState(gridSize);
	state xState(gridSize);
	int gameRes;
	while(1){

		nextMove(oState, xState, GREEDY, PLAYER_X);
		int gameRes = GameOver(xState);
		if ((gameRes == WIN)||(gameRes == DRAW)) return gameRes;

		nextMove(xState, oState, EXPLORATORY, PLAYER_O);
		gameRes = GameOver(oState);
		if ((gameRes == LOSE)||(gameRes == DRAW)) return gameRes;

	}
}

// To Play against human
void viewArena(int gridSize){
	cout << "Here is ur field :)" << endl;
    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
					cout << i*gridSize+j+1;
					if(j < gridSize - 1) {
				 		cout<< " |";
					}
        }
        if(i < gridSize - 1) cout << endl << "--------------------" << endl;
    }
    cout << endl<<"___________________" << endl;
}

void humanInput(state &S1, state &S2,int gridSize){
	int move;
	cout << "choose ur move between 1 to " + to_string(gridSize) << endl;
	cout << "Please dont choose the space occupied by X" << endl;
	cin >> move;
	move--;
	int sz = S1.mat.size();
	int moveX = move/sz, moveY = move%sz;
  equate(S1, S2);
	S2.mat[moveX][moveY]= -1;
  return;
}

void humanPlay(int gridSize){ // only plays not update, output will be win or loose

	state oState(gridSize);
	state xState(gridSize);
	viewArena(gridSize);
	int gameRes;

	while(1){
		cout << "Learner's Move :" << endl;
		nextMove(oState, xState, GREEDY, PLAYER_X);
		gameRes = GameOver(xState);
		printMat(gridSize, xState.mat);
		if ((gameRes == WIN)){
				cout << "Learner Won" << endl; // printMat(gridSize, xState.mat);
		    return;
		}
		if ((gameRes == DRAW)){
				cout << " It's a Draw" << endl; // printMat(gridSize, xState.mat);
		    return;
		}

		cout << "Your Move :" << endl;
		humanInput(xState, oState, gridSize);
		gameRes = GameOver(oState);
		printMat(gridSize, oState.mat);
		if ((gameRes == LOSE)){
				cout << "Learner Lost, You won" << endl; // printMat(gridSize, xState.mat);
		    return;
		}
		if ((gameRes == DRAW)){
				cout << " It's a Draw" << endl; // printMat(gridSize, xState.mat);
		    return;
		}
	}
}

// Below snippet plotts the graphs
// the below function gives destination for plotting image

string plotFile(){
	string epsilon, alpha,gridSize,trains,checkCount;
	cout << "What was the epsilon ?" << endl;
	std::cin >> epsilon;
	cout << "Enter the Alpha again :-)" << endl;
	std::cin >> alpha;
	cout << "What was the size of tic-tac-toe?" << endl;
	std::cin >> gridSize;
	cout << "How many times you trained the Agent?" << endl;
	std::cin >> trains;
	std::cout << "Enter value of checkCount again :-)" << '\n';
	std::cin >> checkCount;

	string plotfile = "Results/Grid-" + gridSize+ "/ Alpha-"+alpha+"_Epsilon-" +epsilon + "_Trainings-" + trains + "CheckCount" +checkCount;
	return plotfile;
}

//function to plot the graphs;
void plotGraph(std::vector<float> Trainings, std::vector<float> winPercent, std::vector<float> drawPercent, std::vector<float> notLost, std::vector<float> lostPercent, string plotfileName){
	string plotTitle = "Percent vs Trains \n" + plotfileName;
	using namespace plt;
	named_plot("Draw", Trainings, drawPercent, "pink");
	named_plot("Win", Trainings, winPercent, "r");
	named_plot( "Win+Draw", Trainings, notLost, "b--");
	named_plot("Lost", Trainings, lostPercent, "g--");
	legend();
	ylim(-5, 105);
	title(plotTitle);
	save(plotfileName);
	cout << "The Graph is stored in file named : " << plotfileName << endl;

}

int main(int argc, char **argv){

	float epsilon, alpha;
	int gridSize, trains;
	float checkCount;
	cout << "What epsilon u want,choose any number between 0 to 1 : " << endl
	<<"But for better training prefer lesser value of epsilon"<<endl;
	std::cin >> epsilon;
	cout << "What alpha u want,choose any number between 0 to 1" << endl;
	std::cin >> alpha;
	cout << "choose gridsize of tic-tac-toe" << endl;
	std::cin >> gridSize;
	cout << "How many times do you want to train the Agent" << endl;
	std::cin >> trains;
	cout << "For how many games u want to calculate probablity after each Training i.e checkCount :" << endl;
	cout << "If u thoose both no of trains and checkCount a high no,it may take a lot of time to calculate " << endl;
	std::cin >> checkCount;
	cout << "Please enter the data again that u entered now !" << endl;

	std::vector<float> winPercent;
	std::vector<float> drawPercent;
	std::vector<float> Trainings;
	std::vector<float> notLost;
	std::vector<float> lostPercent;

	string fileName = plotFile();

	string plotfileName = "Plot-" + fileName + ".png";
	string textfileName = "Text-" + fileName + ".txt";
	ofstream fout(textfileName.c_str());

	fout << "No of Trainings | Percentage";

	for (int i = 0; i < trains; i++) {

		playGame(epsilon, alpha, gridSize);

		float gamesWin = 0;
		float gamesDraw = 0;
		float gamesLose = 0;

		for (int j = 0; j < int(checkCount); j++) {

			int gameResult = game(gridSize);
			if (gameResult == WIN) gamesWin++;
			else if (gameResult == LOSE) gamesLose++;
			else if (gameResult == DRAW) gamesDraw++;

		}

		winPercent.push_back(100*gamesWin/checkCount);
		drawPercent.push_back(100*gamesDraw/checkCount);
    notLost.push_back(100*( gamesDraw/checkCount + gamesWin/checkCount) );
    lostPercent.push_back(100*gamesLose/checkCount);
		Trainings.push_back(i);

		cout << i << "th training :=  Win Percent : " << winPercent[i] << " | Draw Percent :" << drawPercent[i] << " | Lost Percent :" << lostPercent[i] << endl;
		fout << i << "th training :=  Win Percent :" << winPercent[i] << " | Draw Percent :" << drawPercent[i] << " | Lost Percent :" << lostPercent[i] << endl;

	}

	plotGraph(Trainings, winPercent, drawPercent, notLost, lostPercent, plotfileName);
	fout.close();
	cout << "And the stats are stored in file named : " << textfileName << endl;
	cout << "Wanna play with learner ??" << endl;
	int humanGameCount = 1;
	while(humanGameCount){
		cout << "Hey do u want to play with me :)" << endl;
		string humansChoice;
		cout << "Please reply with yes/no" << endl;
		cin >> humansChoice;
		if(humansChoice == "yes") humanPlay(gridSize);
		else humanGameCount--;
	}

 return 0;

}
