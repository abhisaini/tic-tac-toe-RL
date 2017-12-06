#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

struct state {
	int mat[3][3];
	float val;
};

bool compareMatrix(int mat1[3][3], int mat2[3][3]){
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3;)
	}

}

float check(state s, std::vector<state> stateArray ){
	for (int i = 0; i < stateArray.size(); i++){
		if (s == stateArray[i]) {
			return stateArray[i].val;
		}
	}

	return 0.5;
}
