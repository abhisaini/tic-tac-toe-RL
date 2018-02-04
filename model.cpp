#include <iostream>
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
			model A(modelNo);
			A.turnCount = turns;
			models.push_back(A);
			A.array.push_back(S);
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

int main(int argc, char** argv) {



}
