#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#define GREEDY 0
#define EXPLATORY 1

using namespace std;

class state {
	int gridSize;
	std::vector<std::vector<int> > mat;
	float val;
	state(int value){
		gridSize = value;
		for (int i = 0; i < gridSize; i++) {
			for (int j = 0; j < gridSize; j++){
				mat[i][j] = 0;
			}
		}
		val = 0.5;
	}
	~state(void){}
};

std::vector<state> stateArray;
const int alpha;

bool compareMatrices(int mat1[3][3], int mat2[3][3]){
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3;){
			if (mat1[i][j] != mat2[i][j]) return false;
		}
	}
	return true;
}

// checks the state array if the given state already exist or not

float getVal(state s){
	for (int i = 0; i < stateArray.size(); i++){
		if (compareMatrices(s.mat, stateArray[i].mat)) {
			return stateArray[i].val;
		}
	}
	return 0.5;
}

void equate (state &state1, state &state2) {
	state2.mat = state1.mat;
	state2.val = state1.val;
}

void nextState (state const &currState, state &nextState, int policy){
	
	float largestValue = 0;
	state dummyState;
	if (policy == 1){	
		for (int i = 0; i < 3; i++) {
                	for (int j = 0; j < 3; j++) {
                        	if (currState.mat[i][j] == 0){
                                	equate(currState, nextState);
                                	nextState.mat[i][j] = 1;
                                	nextState.val = getVal(nextState);
                                	if (nextValue.val >= largestValue){
                                        	largestValue = nextValue.val;
                                        	equate(nextState, dummyState);
                                	}
                        	}
                	}
        	}
		equate(dummyState, nextState);
	}

	else {
		int position = rand()%10;
		if ( position == 9) position = 8;
		equate (currState, nextState);
		nextState.mat[position/3][position%3] = 1;
		nextState.val = getVal(nextState);		
	}
}

int last_to_act(state prevState){
	int count_x = 0;
	int count_o = 0;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++){
			if (prevState.mat[i][j] == 1) count_x++;
			else if (prevState.mat[i][j] == -1) count_o++;
		 }
	}

	if (count_x == count_o) return -1;
	else if (count_x == (count_0 + 1)) return 1;

	return 0;
}

int getStateIndex(state &State){

	for (int i = 0; i < stateArray.size(); i++) {
		if (compareMatrices(State.mat, stateArray[i].mat)) return i;
	}
	
	return i;

}

void backUp(state &prevState, state &currState){

	// assuming prevState and currState has already been pushed in the stateArray

	prevState.val = prevState.val + alpha*(currState.val - prevState.val);

}

void pushBack(state &currState){

	stateArray.push_back(currState);

}

void playGame(float epsilon){ // plays a game, objective is to update the stateArray and value function table
	
	state prevState(3);
	
	while(GameOver(prevState.mat)){
		
		state currState = new state(3);
		nextState(prevState, currState, policy);
		stateArray.push_back(currState);
		if (GameOver(currState.mat)) break;
		
	}		
}

bool game(){} // only plays not update, output will be win or loose 

int main(int argc, char **argv){

	int gamesCount = 0;
	std::cin >> gamesCount;
	std::cin >> policy;

	for (int i =0; i < gamesCount; i++) {
		playGame();
	}

}
