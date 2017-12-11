#include <bits/stdc++.h>
#include "matplotlibcpp.h"  //library for plotting
#define GREEDY 0
#define EXPLORATORY 1
#define PLAYER_X 1
#define PLAYER_O -1
#define EMPTY 0
#define WIN 1
#define LOSE 0
#define DRAW 4

using namespace std;
namespace plt = matplotlibcpp; //namespace for plotting
typedef std::vector<int> Vector;
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
    	d = rand();
    	d = rand();
    	d = rand();
   	d = rand();
	 	d = rand();

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
		if (player == PLAYER_X) {S1.val = getVal(S1);}
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

void printMat(int gridSize, Matrix mat){
    for (int i = 0; i < gridSize; i++){
        for (int j = 0; j < gridSize; j++){
            cout << mat[i][j] << " ";
        }
        cout << " " << endl;
    }

    cout << "-----------------------" << endl;
}

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
			if (!(j%turns)){ policy = EXPLORATORY;}
			else {policy = GREEDY;}
		}
		else {policy = GREEDY;}
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
		//j++;
		nextMove(xState, oState, EXPLORATORY, PLAYER_O);
		if ((GameOver(oState) == WIN)||(GameOver(oState) == LOSE)||(GameOver(oState) == DRAW)){
				if (GameOver(oState)== LOSE){
				loss++;}
				else {draw++;}
		    break;
		}

	}
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


//Below snippet plotts the graphs
/*************************************************************************************
****************************************************************************************/
//the below function gives destination for plotting image

string plotFile(){
	string epsilon, alpha,gridSize,trains,checkCount;
	cout<<"What was the epsilon ?"<<endl;
	std::cin >> epsilon;
	cout<<"Enter the Alpha again :-)"<<endl;
	std::cin >> alpha;
	cout<<"What was the size of tic-tac-toe?"<<endl;
	std::cin >> gridSize;
	cout<<"How many times you trained the Agent?"<<endl;
	std::cin >> trains;
	std::cout << "Enter value of checkCount again :-)" << '\n';
	std::cin >> checkCount;

    string plotfile =
    "Results/Grid-" + gridSize+ "/ Alpha-"+alpha+"_Epsilon-" +epsilon + "_Trainings-"
    + trains + "CheckCount" +checkCount;

    return plotfile;
}

//function to plot the graphs;
void plotGraph(std::vector<float> Trainings,std::vector<float> winPercent,std::vector<float> drawPercent,std::vector<float> notLost,std::vector<float> lostPercent,string plotfileName){
	string plotTitle="Percent vs Trains \n"+plotfileName;
	using namespace plt;
	named_plot("Draw",Trainings,drawPercent,"pink");
	named_plot("Win",Trainings,winPercent,"r");
	named_plot( "Win+Draw",Trainings,notLost,"b--");
	named_plot("Lost",Trainings,lostPercent,"g--");
	legend();
	ylim(-5, 105);
	title(plotTitle);
	save(plotfileName);
	cout<<"The Graph is stored in file named : "<<plotfileName<<endl;

}

/*************************************************************************************
****************************************************************************************/



int main(int argc, char **argv){


	float epsilon, alpha;
	int gridSize,trains;
	float checkCount;
	cout<<"What epsilon u want,choose any number between 0 to 1 : "<<endl
	<<"But for better training prefer lesser value of epsilon"<<endl;
	std::cin >> epsilon;
	cout<<"What alpha u want,choose any number between 0 to 1"<<endl;
	std::cin >> alpha;
	cout<<"choose gridsize of tic-tac-toe"<<endl;
	std::cin >> gridSize;
	cout<<"How many times do you want to train the Agent"<<endl;
	std::cin >> trains;
	cout<<"For how many games u want to calculate probablity after each Training i.e checkCount :"<<endl;
	cout<<"If u thoose both no of trains and checkCount a high no,it may take a lot of time to calculate "<<endl;
	std::cin >> checkCount;
	cout<<"Please enter the data again that u entered now !"<<endl;


	std::vector<float> winPercent;
	std::vector<float> drawPercent;
	std::vector<float> Trainings;
	std::vector<float> notLost;
	std::vector<float> lostPercent;
	string fileName=plotFile();
	string plotfileName = "Plot-" + fileName + ".png";
	string textfileName = "Text-" + fileName + ".txt";
	ofstream fout(textfileName.c_str());
	fout<<"No of Trainings | Percentage";

	for (int i = 0; i < trains; i++) {
		playGame(epsilon, alpha, gridSize);
        float gamesWin = 0;
      	float gamesDraw = 0;
      	float gamesLose = 0;
		for (int j = 0; j < int(checkCount); j++) {

			int gameResult = game(gridSize);
			if (gameResult == WIN) gamesWin++;
			else if (gameResult == LOSE) gamesLose++;
			else if (gameResult == DRAW) gamesDraw++;

		}
        winPercent.push_back(100*gamesWin/checkCount);
		drawPercent.push_back(100*gamesDraw/checkCount);
        notLost.push_back(100*( gamesDraw/checkCount + gamesWin/checkCount) );
        lostPercent.push_back(100*gamesLose/checkCount);
		Trainings.push_back(i);
		cout<<i<<"th training :=  Win Percent : "<<winPercent[i]<<" | Draw Percent :"<<drawPercent[i]<<" | Lost Percent :"<<lostPercent[i]<<endl;
		fout<<i<<"th training :=  Win Percent :"<<winPercent[i]<<" | Draw Percent :"<<drawPercent[i]<<" | Lost Percent :"<<lostPercent[i]<<endl;
	}

	plotGraph(Trainings,winPercent,drawPercent,notLost,lostPercent,plotfileName);
	fout.close();
	cout<<"And the stats are stored in file named : "<<textfileName<<endl;

 return 0;

}
