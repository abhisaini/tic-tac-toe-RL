#include <bits/stdc++.h>
#define GREEDY 0
#define EXPLORATORY 1
#define PLAYER_X 1
#define PLAYER_O -1
#define EMPTY 0
#define WIN 1
#define LOSE 0
#define DRAW 4

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
{
	int sz = arr.size();
	float val = 0.5;
  //checking for sz 1s or sz -1s in every row
  for(int i = 0; i < sz; i++){
  	int sum = 0;
    for(int j = 0; j < sz; j++){
      sum = sum + arr[i][j];
    }
    if(sum == sz){
      return 1;
    }
    if(sum == -sz){
      return 0;
    }
  }

  //checking for sz 1s or sz -1s in a column
  for(int i=0;i<sz;i++){
    int sum=0;
  	for(int j = 0; j < sz; j++){
			sum=sum+arr[j][i];
    }
    if(sum == sz){
      return 1;
    }
    if(sum == -sz){
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
//the below snippet represents function randomMove(),whose input are a const state state1,a state state 2,player(1 or -1).And the function will ch
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
    int a = rand();
    int c = rand();
    int b = rand();
    int d = rand();
   // d = rand();
   // d = rand();
   // d = rand();
 //   d = rand();
// d = rand();

    int input=rand()%length;
    return arr[input];
}

//it will make the move i.e it takes the state,vacancy and player(1 or -1) as input and changes the vacancy to player
void randomChanged(state &S1, state &S2, int move, int player){

	int sz = S1.mat.size();
	int moveX=move/sz, moveY=move%sz;
    equate(S1, S2);

    S2.mat[moveX][moveY]= player;
  return;
}


void randomMove(state &S, state &S1, int player){
    int space = spaceRandom(S);
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

    randomChanged(S, S1, move, player);
		if (player == PLAYER_X) S1.val = getVal(S1);
    return;
}

/******************************************************************************************************************************
********************************************************************************************************************************/



//Must be placed below functions spaceRandom and getValUnknown
//input will be the state
//will return 1 for win,0 for loose,2 for game not over,4 for draw
/************************************************************************************
***************************************************************************************/
int GameOver(state &arr){

    float val=getValUnknown(arr.mat);
    int vacancy=spaceRandom(arr);
    if(vacancy==0&&val==0.5) return DRAW; //4 stands for draw
    if(val!=0.5) return val;//will return 0 if lost and 1 if won;
    else if(vacancy>0&&val==0.5) return 2;//2 stands for game not over yet

}
/******************************************************************************************************
*****************************************************************************************************/

void nextMove (state &currState, state &nextState, int policy, int player){

	float largestValue = 0;
	int gridSize = currState.mat.size();
	// state dummyState(gridSize);
	if (policy == GREEDY){
		for (int i = 0; i < gridSize; i++) {
			for (int j = 0; j < gridSize; j++) {
				if (currState.mat[i][j] == 0){
					equate(currState, nextState);
					nextState.mat[i][j] = 1;
					nextState.val = getVal(nextState);
					if (nextState.val >= largestValue){
						largestValue = nextState.val;
						// equate(nextState, dummyState);
					}
				}
			}
		}
		std::vector<int> array;
		int count = 0;
		for (int i = 0; i < gridSize; i++) {
			for (int j = 0; j < gridSize; j++) {
				if (currState.mat[i][j] == 0){
					equate(currState, nextState);
					nextState.mat[i][j] = 1;
					nextState.val = getVal(nextState);
					if (nextState.val == largestValue){
						array.push_back(3*i + j);
						count++;
					}
				}
			}
		}
		int freespace[count];
		for (int i = 0; i < count; i++){
		    freespace[i] = array[i];
		}
		// equate(dummyState, nextState);
		int move = randomInput(freespace, count);
        randomChanged(currState, nextState, move, PLAYER_X);
		nextState.val = getVal(nextState);

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

void pushBack(state S){

	int gridSize = S.mat.size();
	state S1(gridSize);
	equate(S, S1);
	stateArray.push_back(S1);
}

bool alreadyExist(state S){

	for (int i = 0; i < stateArray.size(); i++){
		if (compareMatrices(stateArray[i].mat, S.mat)){
		    return true;
		}
	}
	return false;
}

int win = 0;
int loss = 0;
int draw = 0;

void playGame(float epsilon, float alpha, int gridSize){ // plays a game, objective is to update the stateArray and value function table

	state oState(gridSize);
	state xState(gridSize);
	state dummyState(gridSize);

	int turns = int (1/epsilon);
	int j = 0;
	int policy;

	while(1){

		if (epsilon)
		{
			j++;
			if (!j%turns) policy = EXPLORATORY;
			else policy = GREEDY;
		}
		else policy = GREEDY;
		nextMove(oState, xState, policy, PLAYER_X);
		if (!alreadyExist(xState)) pushBack(xState);
		int i = getStateIndex(dummyState);
		if (i != -1 && policy == GREEDY ) backUp(stateArray[i], xState, alpha);
		if ((GameOver(xState) == WIN)||(GameOver(xState) == LOSE)||(GameOver(xState) == DRAW)){
		    if (GameOver(xState) == WIN) win++;
		    else draw++;
		    break;
		}
		equate(xState, dummyState);
		j++;
		nextMove(xState, oState, EXPLORATORY, PLAYER_O);
		if ((GameOver(oState) == WIN)||(GameOver(oState) == LOSE)||(GameOver(oState) == DRAW)){
		    loss++;
		    break;
		}

	}
}

void printMat(int gridSize, Matrix mat){
    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            cout << mat[i][j] << " ";
        }
        cout << " " << endl;
    }

    cout << "-----------------------" << endl;
}

int game(int gridSize){ // only plays not update, output will be win or loose

	state oState(gridSize);
	state xState(gridSize);

	while(1){

		nextMove(oState, xState, GREEDY, PLAYER_X);
		//
		if ((GameOver(xState) == WIN)||(GameOver(xState) == LOSE)||(GameOver(xState) == DRAW)){
		    // printMat(gridSize, xState.mat);
		    return GameOver(xState);
		}

		nextMove(xState, oState, EXPLORATORY, PLAYER_O);
		//
		if ((GameOver(oState) == WIN)||(GameOver(oState) == LOSE)||(GameOver(oState) == DRAW)){
	        // printMat(gridSize, oState.mat);
		    return GameOver(oState);
		}


	}

}

int main(int argc, char **argv){

    for(int i = 0; i < 2800; i++){
        playGame(1, 0.3, 3);
    }
    cout << stateArray.size() << endl;
    cout << win << endl;
    cout << loss << endl;
    cout << draw << endl;

 /*int sz=stateArray.size();
 for (int i = 0; i < sz; i++)
    cout << stateArray[i].val << endl;
 */
    int winPercent;
    int gamesWin = 0;
	int gamesDraw = 0;
	int gamesLose = 0;

    for (int i = 0; i < 100; i++){
        int gameResult = game(3);
	    if (gameResult == WIN) gamesWin++;
	    else if (gameResult == LOSE) gamesLose++;
	    else if (gameResult == DRAW) gamesDraw++;

    }
    cout << "WINS : " << gamesWin <<"," << "Loses : " << gamesLose + gamesDraw << endl;


 /*state S0(3);
 state S1(3);
 state S2(3);
 state S3(3);
 state S4(3);
 state S5(3);
 state S6(3);

 S0.mat[0][0] = 1;
 S0.mat[2][2] = -1;

 equate(S0,S1);
 equate(S0,S2);
 equate(S0,S3);
 equate(S0,S4);
 equate(S0,S5);
 equate(S0,S6);

 S1.mat[0][2] = 1;
 S1.val = 0.9;
 S2.mat[2][0] = 1;
 S2.val = 0.9;
 S3.mat[1][1] = 1;
 S3.val = 0.85;
 S4.mat[0][1] = 1;
 S4.val = 0.9;
 S5.mat[1][0] = 1;
 S5.val = 0.9;
 S6.mat[2][1] = 1;
 S6.val = 0.9;

 stateArray.push_back(S1);
 stateArray.push_back(S2);
 stateArray.push_back(S3);
 stateArray.push_back(S4);
 stateArray.push_back(S5);
 stateArray.push_back(S6);

 state S7(3);

 nextMove(S0, S7,GREEDY,PLAYER_X);

 printMat(3, S7.mat);
 */return 0;

}
