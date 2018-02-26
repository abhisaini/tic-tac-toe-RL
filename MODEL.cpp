#include <iostream>
#include "matplotlibcpp.h"
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

std::vector<model> models;

void sort();

// the functions counts the positions where a player can move
int spaceRandom(state const &S) {
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
void randomChanged(state &S, int move, int player){

	int sz = S.mat.size();
	int moveX=move/sz, moveY=move%sz;
    S.mat[moveX][moveY]= player;
    return;
}

void randomMove(state &S1, state &S2, int player){
    int space = spaceRandom(S1);
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

    randomChanged(S2, move, player);
    return;
}


nextMove(state& S1, state& S2, int player, int policy, int gridSize){ // policy is only for PLAYER_X

    // move of PLAYER_X
    // greedy or random
    if (player == PLAYER_X) {

        int modelNo;
        int turnCount;
        if (policy == GREEDY) {

        }
        else {
            randomMove(S1, S2, PLAYER_X);
            modelNo = modelClassifier(S1, PLAYER_X);
            turnCount = countTurns(S1);
            
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

        dummyState = nextMove(oState, xState, policy, PLAYER_X);

    }

}
