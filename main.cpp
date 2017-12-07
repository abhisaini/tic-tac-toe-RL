#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

struct state {
	int mat[3][3];
	float val;
};

std::vector<state> stateArray;

bool compareMatrices(int mat1[3][3], int mat2[3][3]){
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3;){
			if (mat1[i][j] != mat2[i][j]) return false;
		}
	}
	return true;
}

// checks the state array if the given state already exist or not

float checkStateArray(state s){
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
                                	nextState.val = checkStateArray(nextState);
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
		nextState.val = checkStateArray(nextState);		
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
