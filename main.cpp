#include <bits/stdc++.h>
#define GREEDY 0
#define EXPLORATORY 1
#define PLAYER_X 1
#define PLAYER_O -1
#define EMPTY 0
#define WIN 1
#define LOSE -1
#define DRAW 0

using namespace std;

typedef std::vector<std::vector<int> > Matrix;

class state {
public:
	int gridSize;
	Matrix mat;
	float val;
	state(int value){
		gridSize = value;
		mat.resize(gridSize);
		for (int i = 0; i < gridSize; i++)
		    mat[i].resize(gridSize);
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

std::vector<state> stateArray;

//function to check value of states which are not stored
/********************************************************************************************
**********************************************************************************************/
float getValUnknown(Matrix arr)
{   int sz=arr.size();
	float val=0.5;
    //checking for sz 1s or sz -1s in every row
    for(int i=0;i<sz;i++){
        int sum=0;
        for(int j=0;j<sz;j++){
            sum=sum+arr[i][j];
        }
        if(sum==sz){
            return 1;
        }
        if(sum==-sz){
            return 0;
        }
    }

    //checking for sz 1s or sz -1s in a column
    for(int i=0;i<sz;i++){
        int sum=0;
        for(int j=0;j<sz;j++){
            sum=sum+arr[j][i];
        }
        if(sum==sz){
            return 1;
        }
        if(sum==-sz){
            return 0;
        }
    }


    //checking sz 1s or -1s in  diagonal
    int sumd1=0,sumd2=0;
    for(int i=0;i<sz;i++){
        sumd1=sumd1+arr[i][i];
        sumd2=sumd2+arr[i][sz-1-i];
    }
    if(sumd1==sz||sumd2==sz) return 1;
    if(sumd1==sz||sumd2==sz) return 0;


    return val;
}
/**********************************************************************************************************
***********************************************************************************************************/

// checks the state array if the given state already exist or not
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
//the below snippet represents function randomMove(),whose input are a const state state1,a state state 2,player(1 or -1).And the function will change state2 such that state2 is next random move for state1 and player given
/******************************************************************************************************************************
********************************************************************************************************************************/
//will return possible space at any state and takes state as input
int spaceRandom(state const &s) {
    int sz=s.mat.size();
    int val=0;
    for(int i=0;i<sz;i++) {
       for(int j=0;j<sz;j++) {
           if(s.mat[i][j]!=0) {
               val++;
           }
       }
   }
    return sz*sz-val;
}

//function will take the array containing the position of vacancies as input and returns a random vacancy
int randomInput(int arr[], int length){
    int input=rand()%length;
    return arr[input];
}

//it will make the move i.e it takes the state,vacancy and player(1 or -1) as input and changes the vacancy to player
void randomChanged(state const &S1, state &S2, int move, int player){

	int sz = S1.mat.size();
	int moveX=move/sz, moveY=move%sz;
    equate(S1, S2);

    S2.mat[moveX][moveY]= player;
  return;
}


void randomMove(state const &S, state &S1, int player){
    int space=spaceRandom(S);
    int sz=S.mat.size();
    int i=0,j=0,index=0;
    int freespace[space];
    for(i=0;i<sz;i++){
        for(j=0;j<sz;j++){
            if(S.mat[i][j]==0){
                freespace[index]=sz*i+j;
                index++;
            }
        }
    }
    int move=randomInput(freespace,space);

    randomChanged(S, S1, move, player);
    return ;
}

/******************************************************************************************************************************
********************************************************************************************************************************/



//Must be placed below functions spaceRandom and getValUnknown
//input will be the state
//will return 1 for win,0 for loose,2 for game not over,4 for draw
/************************************************************************************
***************************************************************************************/
int gameOver(state &arr){
    float val=getValUnknown(arr.mat);
    int vacancy=spaceRandom(arr);
    if(vacancy==0&&val==0.5) return 4; //4 stands for draw
    if(val!=0.5) return val;//will return 0 if lost and 1 if won;
    else if(vacancy>0&&val==0.5) return 2;//2 stands for game not over yet

}
/******************************************************************************************************
*****************************************************************************************************/

void nextMove (state &currState, state &nextState, int policy, int player){

	float largestValue = 0;
	int greedSize = currState.mat.size();
	state dummyState(greedSize);
	if (policy == GREEDY){
		for (int i = 0; i < greedSize; i++) {
			for (int j = 0; j < greedSize; j++) {
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

int getStateIndex(state &State){

	for (int i = 0; i < stateArray.size(); i++) {
		if (compareMatrices(State.mat, stateArray[i].mat)) return i;
	}

	return -1;

}

void backUp(state &prevState, state &currState, float alpha){ // assuming prevState and currState has already been pushed in the stateArray
	prevState.val = prevState.val + alpha*(currState.val - prevState.val);
}

void pushBack(state const &S){

	int greedSize = S.mat.size();
	state S1(greedSize);
	equate(S, S1);
	stateArray.push_back(S1);
}

bool alreadyExist(state S){
	for (int i = 0; i < stateArray.size(); i++){
		if (compareMatrices(stateArray[i], S)) return false;
	}
	return true;
}

void playGame(float epsilon, float alpha, int greedSize){ // plays a game, objective is to update the stateArray and value function table

	state oState(greedSize);
	state xState(greedSize);
	state dummyState(greedSize);

	int turns = int (1/epsilon);
	int j = 0;

	while(1){

		j++;
		if (!j%turns) policy = EXPLORATORY;
		else policy = GREEDY;
		nextState(oState, xState, policy, PLAYER_X);
		if (!alreadyExist(xState)) pushBack(xState);
		int i = getStateIndex(dummyState);
		if (i != -1 && policy == GREEDY ) backUp(stateArray[i], xState, alpha);
		if ((GameOver(xState) == WIN)||(GameOver(xState) == LOSE)||(GameOver(xState == DRAW))) break;
		equate(xState, dummyState);
		nextState(xState, oState, EXPLATORY, PLAYER_O);
		if ((GameOver(oState) == WIN)||(GameOver(oState) == LOSE)||(GameOver(oState == DRAW))) break;

	}
}

int game(int greedSize){ // only plays not update, output will be win or loose

	state oState(greedSize);
	state xState(greedSize);

	while(1){

		nextState(oState, xState, GREEDY, PLAYER_X);
		if ((GameOver(xState) == WIN)||(GameOver(xState) == LOSE)||(GameOver(xState == DRAW))) return GameOver(xState);
		nextState(xState, oState, GREEDY, PLAYER_O);
		if ((GameOver(oState) == WIN)||(GameOver(oState) == LOSE)||(GameOver(oState == DRAW))) return GameOver(oState);

	}

}

/*int main(int argc, char **argv){

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
			fout << "Game No. : "<<gamesCount<<" Result : Won"<<endl;
		}
		if(gameCondition==0){ //function GameOver will return 0 if loose
			gamesLose++;
			fout << "Game No. : "<<gamesCount<<" Result : Lost"<<endl;
		}
		if(gameCondition==4) {//function GameOver will return 4 if draw
			gamesDraw++;
			fout << "Game No. : "<<gamesCount<<" Result : Draw"<<endl;
		}
		if(gamesCount>=100){

		}
	}
	fout.close();
	return 0;

}	*/

int main(){

	float epsilon, alpha;
	int greedSize;
	std::cin >> epsilon;
	std::cin >> alpha;
	std::cin >> greedSize;

	int winPercent[1000];
	int gamesWin = 0;
	int gamesDraw = 0;
	int gamesLose = 0;

	for (int i = 0; i < 10000; i++) {
		playGame(epsilon, alpha, greedSize);
		for (int j = 0; j < 100; j++) {

			int gameResult = game(greedSize);
			if (gameResult == WIN) gamesWin++;
			else if (gameResult == LOSE) gamesLose++;
			else if (gameResult == DRAW) gamesDraw++;

		}
		winPercent[i] = gamesWin;
	}

	return 0;

}
