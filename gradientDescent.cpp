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

int w1 = 0.5;
int w2 = 0.5;
int w3 = 0.5;
int w4 = 0.5;
int w5 = 0.5;
int w6 = 0.5;
int w7 = 0.5;
int w8 = 0.5;

int main(int argc, char** argv) {

  for (int i = 0; i < 100; i++) {
    int gameResult = game(gridSize);
    if (gameResult == WIN) gamesWin++;
    else if (gameResult == LOSE) gamesLose++;
    else if (gameResult == DRAW) gamesDraw++;
  }

}
