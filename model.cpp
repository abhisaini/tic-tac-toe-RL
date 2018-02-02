#include <iostream>
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

class state { // inorder to describe  a state with the positions of X and O and its value function
public:
	int gridSize;
	Matrix mat;
	float val;
	state(int value){
		gridSize = value;
		mat.resize(gridSize);
		for (int i = 0; i < gridSize; i++)
		    mat[i].resize(gridSize);
		val = 0.5;
	}
	~state(void){}
};

int rowSum(state const& S, int rowIndex) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    sum = sum + S.mat[rowIndex][i]
  }
  return sum;
}

int colSum(state const& S, int colIndex) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    sum = sum + S.mat[i][colIndex]
  }
  return sum;
}

int diaSum(state const& S, int direction) {
  if (direction == 1) return S.mat[0][0] + S.mat[1][1] + S.mat[2][2];
  else return S.mat[0][2] + S.mat[1][1] + S.mat[2][0];
}

int winClassifier(state const& S, int player) {

  for (int i = 0; i < 3; i++) {
    if ((rowSum(S, i) == 2*player)||(colSum(S, i) == 2*player) {
      return 1;
    }
  }

  if ((diaSum(S, 1)==2*player)||(diaSum(S, -1)==2*player)) return 1;

  return 0;

}

int blockClassifier(state const& S, int player) {

  for (int i = 0; i < 3; i++) {
    if ((rowSum(S, i) == -2*player)||(colSum(S, i) == -2*player) {
      return 1;
    }
  }

  if ((diaSum(S, 1) == -2*player)||(diaSum(S, -1) == -2*player)) return 1;

  return 0;

}

int forkedClassifier(state const& S, int player) {

  winCount = 0;
  for (int i = 0; i < 3; i++) {
    if (rowSum(S, i) == 2*player) winCount++;
    if (colSum(S, i) == 2*player) winCount++;
  }
  if (diaSum(S, 1) == 2*player) winCount++;
  if (diaSum(S, -1) == 2*player) winCount++;

  if (winCount > 1) return 1;
  else return 0;

}

int centerClassifier(state const& S, int player) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        sum = sum + S.mat[i][j];
    }
  }

  if ((sum == player)&&(S.mat[1][1] == player)) return 1;
  else return 0;
}

int cornerClassifier(state const& S, int player) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        sum = sum + S.mat[i][j];
    }
  }
  if (((S.mat[0][0] == player)||(S.mat[0][2] == player)||(S.mat[2][0] == player)||(S.mat[2][2] == player))&&(sum == player)) return 1;
  else return 0;

}

int emptyClassifier(state const& S, int player) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        sum = sum + S.mat[i][j];
    }
  }
  if (sum == 0) return 1;
  else return 0;
}

int emptySideClassifier(state const&, int player) {

  for (int i = 0; int < 3; i++){
    if (rowSum(S, i) == 0||colSum(S, i) == 0) return 1;
  }

  return 0;
}

// to classify a state in any of the above models
int modelClassifier(state const& S, int player) {

  if (winClassifier(S, player)) return WIN;
  else if (blockClassifier(S, player)) return BLOCK;

}

int main(int argc, char** argv) {



}
