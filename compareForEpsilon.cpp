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


//Below snippet plotts the graphs
/*************************************************************************************
****************************************************************************************/
//the below function gives destination for plotting image

string plotFile(){
	string epsilon1,epsilon2, alpha,gridSize,trains,checkCount;
	cout<<"What was  epsilon 1 ?"<<endl;
	std::cin >> epsilon1;
	cout<<"What was  epsilon 2 ?"<<endl;
	std::cin >> epsilon2;
	cout<<"Enter the Alpha again :-)"<<endl;
	std::cin >> alpha;
	cout<<"What was the size of tic-tac-toe?"<<endl;
	std::cin >> gridSize;
	cout<<"How many times you trained the Agent?"<<endl;
	std::cin >> trains;
	std::cout << "Enter value of checkCount again :-)" << '\n';
	std::cin >> checkCount;

    string fileName =
    "Results/Grid-" + gridSize+ "/ Alpha-"+alpha+"_Ep1-" +epsilon2 +"_Ep1-" +epsilon2 + "_Trainings-"
    + trains + "CheckCount" +checkCount;
    cout<<fileName;
    return fileName;
}

//function to plot the graphs;
void plotGraph(std::vector<float> Trainings,std::vector<float> winPercent1,std::vector<float> lostPercent1,std::vector<float> winPercent2,std::vector<float> lostPercent2,string plotfileName,string plotTitle){
	using namespace plt;
	named_plot("Win-1",Trainings,winPercent1,"b--");
	named_plot("Win-2",Trainings,winPercent2,"g");
	named_plot( "Lost-1",Trainings,lostPercent1 ,"r--");
	named_plot("Lost-2",Trainings,lostPercent2);
	//legend();
	ylim(0, 100);
    plotTitle="Percent vs Trains \n "+plotfileName;
	title("Percent vs Trains  ");
	save(plotfileName);
	cout<<"The Graph is stored in file named : "<<plotfileName<<endl;

}

/*************************************************************************************
****************************************************************************************/


int main(int argc, char **argv){


	float epsilon1,epsilon2, alpha;
	int gridSize,trains;
	float checkCount;
	cout<<"What 1st epsilon to compare,choose any number between 0 to 1 : "<<endl;
	std::cin >> epsilon1;
    cout<<"What 2nd epsilon to compare,choose any number between 0 to 1 : "<<endl;
	std::cin >> epsilon2;
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


	std::vector<float> winPercent1;
	std::vector<float> winPercent2;
	std::vector<float> Trainings;
	std::vector<float> lostPercent1;
	std::vector<float> lostPercent2;
	string fileName=plotFile(),plotTitle;
    string plotfileName = "Plot-CompEp-" + fileName + ".png";
    string textfileName = "Text-CompEp-" + fileName + ".txt";
	ofstream fout(textfileName.c_str());
    fout<<"**********EPSILON1***************"<<endl;
    fout<<"___________________________________"<<endl;
	fout<<"No of Trainings | Percentage"<<endl;

	for (int i = 0; i < trains; i++) {
		playGame(epsilon1, alpha, gridSize);
        float gamesWin = 0;
      	float gamesDraw = 0;
      	float gamesLose = 0;
		for (int j = 0; j < int(checkCount); j++) {

			int gameResult = game(gridSize);
			if (gameResult == WIN) gamesWin++;
			else if (gameResult == LOSE) gamesLose++;
			else if (gameResult == DRAW) gamesDraw++;

		}
        winPercent1.push_back(100*gamesWin/checkCount);
        lostPercent1.push_back(100*gamesLose/checkCount);
		Trainings.push_back(i);
		cout<<"Ep1"<<i<<"th training :=  Win Percent : "<<winPercent1[i]<<" | Lost Percent :"<<lostPercent1[i]<<endl;
		fout<<"Ep1"<<i<<"th training :=  Win Percent :"<<winPercent1[i]<<" | Lost Percent :"<<lostPercent1[i]<<endl;
	}

    stateArray.clear();

    fout<<"**********EPSILON1***************"<<endl;
    fout<<"___________________________________"<<endl;
    fout<<"No of Trainings | Percentage"<<endl;

    for (int i = 0; i < trains; i++) {
        playGame(epsilon2, alpha, gridSize);
        float gamesWin = 0;
        float gamesDraw = 0;
        float gamesLose = 0;
        for (int j = 0; j < int(checkCount); j++) {

            int gameResult = game(gridSize);
            if (gameResult == WIN) gamesWin++;
            else if (gameResult == LOSE) gamesLose++;
            else if (gameResult == DRAW) gamesDraw++;

        }
        winPercent2.push_back(100*gamesWin/checkCount);
        lostPercent2.push_back(100*gamesLose/checkCount);
        Trainings.push_back(i);
        cout<<"Ep2"<<i<<" th training :=  Win Percent : "<<winPercent2[i]<<" | Lost Percent :"<<lostPercent2[i]<<endl;
        fout<<"Ep2"<<i<<" th training :=  Win Percent :"<<winPercent2[i]<<" | Lost Percent :"<<lostPercent2[i]<<endl;
    }






	plotGraph(Trainings,winPercent1,lostPercent1,winPercent2,lostPercent2,plotfileName,plotTitle);
	fout.close();
	cout<<"And the stats are stored in file named : "<<textfileName<<endl;

 return 0;

}
