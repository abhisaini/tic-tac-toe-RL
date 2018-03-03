#include <iostream>
#include <vector>
#include <stdlib.h>
// #include "matplotlibcpp.h"
#define WIN 1
#define BLOCK 2
#define FORK 3
#define OPP_FORK_BLOCK 4
#define CENTER 5
#define OPP_CORNER 6
#define EMPTY_CORNER 7
#define EMPTY_SIDE 8
#define GREEDY 0
#define EXPLORATORY 1
#define PLAYER_X 1
#define PLAYER_O -1
#define EMPTY 0
#define LOSE 0
#define DRAW 4

typedef std::vector<std::vector<int> > Matrix;

class state { // inorder to describe  a state with the positions of X and O and its value function
public:
	int gridSize;
	Matrix mat; // contains the configuration
	float val; // the value of the state
	int config; // to describe the model in which the state lies
	int turnCount; // nth turn
	state(int value){
		gridSize = value;
		mat.resize(gridSize);
		for (int i = 0; i < gridSize; i++)
		    mat[i].resize(gridSize);
		val = 0.5;
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
//
// void pushInOrder(int modelNo, int turnCount, state* SPtr, int index) {
//
// 	state S(3);
// 	S.mat = SPtr->mat;
// 	S.val = SPtr->val;
// 	models[index].array.erase(SPtr);
// 	for(int i = models[index].array.size()-1; i >= 0; i--) {
// 			if (models[index].array[i].val <= S.val) models[index].array.insert(models[index].array.begin() + i, S); break;
// 	}
// 	return;
// }

int rowSum(Matrix mat, int rowIndex, int player) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
		if (mat[i][rowIndex] == player) sum++;
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
