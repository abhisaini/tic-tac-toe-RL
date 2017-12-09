#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#define GREEDY 0
#define EXPLATORY 1

using namespace std;

typedef std::vector<std::vector<int> > Matrix;

std::vector<state> stateArray;
const int alpha;

class state {
public:
	int gridSize;
	Matrix mat;
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

bool compareMatrices(Matrix mat1, Matrix mat2){
	for (int i = 0; i < mat1.size(); i++) {
		for (int j = 0; j < mat1.size(); j++){
			if (mat1[i][j] != mat2[i][j]) return false;
		}
	}
	return true;
}


//function to check value of states which are not stored
int getValUnknown(Matrix checkval)
{
	int val=0.5;
    //checking for 3 1s or 3 -1s in a row
    for(int i=0;i<3;i++){
        if((checkval[i][1]==1)&&(checkval[i][2]==1)&&(checkval[i][3]==1)){
            val = 1;
        }

        if((checkval[i][1]==-1)&&(checkval[i][2]==-1)&&(checkval[i][3]==-1)){
            val = 0;
        }
    }

    //checking for 3 1s or 3 -1s in a column
    for(int i=0;i<3;i++){
        if((checkval[1][i]==1)&&(checkval[2][i]==1)&&(checkval[3][i]==1)){
            val = 1;
        }

        if((checkval[3][i]==-1)&&(checkval[2][i]==-1)&&(checkval[3][i]==-1)){
            val = 0;
        }
    }
    //checking for 3 1s in both the diagonals and return 1 for win
    if(((checkval[1][1]==1)&&(checkval[2][2]==1)&&(checkval[3][3]==1))||((checkval[3][1]==1)&&(checkval[2][2]==1)&&(checkval[1][3]==1))){
        val = 1;
    }
    //checking for 3 1s in both the  and return 0 for lose
    if(((checkval[1][1]==-1)&&(checkval[2][2]==-1)&&(checkval[3][3]==-1))||((checkval[3][1]==-1)&&(checkval[2][2]==-1)&&(checkval[1][3]==-1))){
        val = 0;
    }

    return val;
}

// checks the state array if the given state already exist or not
float getVal(state s){
	for (int i = 0; i < stateArray.size(); i++) {
		if (compareMatrices(s.mat, stateArray[i].mat)) {
			return stateArray[i].val;
		}
	}
	return getValUnknown(s.mat);
}

void equate(state const &state1, state &state2) {
	for (int i = 0; i < state1.size(); i++) {
		for (int j = 0; j < state1.size(); j++) {
			state2.mat[i][j] = state1.mat[i][j];
		}
	}
	state2.val = state1.val;
}

int spaceRandom(Matrix state[3][3]) {
    int val=1;
    for(int i=0;i<3;i++) {
       for(int j=0;j<3;j++) {
           if(state[i][j]!=0) {
               val++;
           }
       }
   }
    return 10-val;
}

//to get the random move from possible moves
int randomInput(int arr[], int size){
    int input=rand()%size;
    return arr[input];
}

//to make the move i.e. to change the value from 0 to -1 for possible move
void randomChanged(state const &S1, state &S2, int move, int player){

	int size = state.mat.size();
	int moveX=move/size, moveY=move%size;
	equate(S1, S2);
  S2.mat[moveX][moveY]= player;
  return;
}

//the main function to call to decide random player's move.It will play the move by itself
void randomMove(state const &S, state &S1, int player){
    int space=spaceRandom(S.mat);
    int i=0,j=0,index=0;
    int freespace[space];
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            if(S.mat[i][j]==0){
                freespace[index]=3*i+j;
                index++;
            }
        }
    }
    int move=randomInput(freespace,space);

    randomChanged(S, S1, move, player);
    return ;
}


void nextState (state const &currState, state &nextState, int policy, int player){

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
		randomMove(currState, nextState, player);
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

	return -1;

}

void backUp(state &prevState, state &currState)
	// assuming prevState and currState has already been pushed in the stateArray
	prevState.val = prevState.val + alpha*(currState.val - prevState.val);
}

void pushBack(state const &S){
	state S = new state(3);
	equate(S1, S);
	stateArray.push_back(S);
}

bool alreadyExist(state S){
	for (int i = 0; i < stateArray.size(); i++){
		if (compareMatrices(stateArray[i], S)) return false;
	}
	return true;
}

void playGame(float epsilon){ // plays a game, objective is to update the stateArray and value function table

	state oState = new state(3);
	state xstate = new state(3);
	state dummyState = new state(3);

	int turns = int (1/epsilon);
	int j = 0;

	while(1){

		j++;
		if (!j%turns) policy = 1;
		nextState(oState, xState, policy, 1);
		if (!alreadyExist(xState)) pushBack(xState);
		int i = getStateIndex(dummyState);
		if (i != -1) backUp(stateArray[i], xState);
		if (GameOver(xState)) break;
		equate(dummyState, xState);
		nextState(xState, oState, 0, 0);
		if (GameOver(oState)) break;

	}
}

bool game(){ // only plays not update, output will be win or loose

	state oState = new state(3);
	state xstate = new state(3);

	while(1){

		nextState(oState, xState, 1, 1);
		if (GameOver(xState)) return true;
		nextState(xState, oState, 0, 0);
		if (GameOver(oState)) return false;

	}

}

int main(int argc, char **argv){

	int gamesCount = 0;
	int gamesWin=0,gamesDraw=0,gamesLose=0;
	int probWin,probLose,probDraw;
	std::cin >> gamesCount;
	std::cin >> policy;
	string results = "result.txt";
	ofstream fout(results.c_str());
	if (!fout) {
      	cerr << "error: open file for results failed!" << endl;
      	abort();  // in <cstdlib> header
   }


	for (int i =0; i < gamesCount; i++) {
		int gameCondition;
		playGame(gameCondition);
		if(gameCondition==1){      //function GameOver will return 1 if won
			gamesWin++;
			fout << "Game No:"<<gamesCount<<" Result:Won"<<endl;
		}
		if(gameCondition==0){ //function GameOver will return 0 if loose
			gamesLose++;
			fout << "Game No:"<<gamesCount<<" Result:Lost"<<endl;
		}
		if(gameCondition==4) {//function GameOver will return 4 if draw
			gamesDraw++;
			fout << "Game No:"<<gamesCount<<" Result:Draw"<<endl;
		}
		if(gamesCount>=100){

		}
	}
	fout.close();

}
