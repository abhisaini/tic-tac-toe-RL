#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;


//function to check status of game at certain state
int GameOver(int state[3][3])
{
    //checking for 3 1s or 3 -1s in a row
    for(int i=0;i<3;i++){
        if((state[i][1]==1)&&(state[i][2]==1)&&(state[i][3]==1)){
            return 1;
        }

        if((state[i][1]==-1)&&(state[i][2]==-1)&&(state[i][3]==-1)){
            return 0;
        }
    }

    //checking for 3 1s or 3 -1s in a column
    for(int i=0;i<3;i++){
        if((state[1][i]==1)&&(state[2][i]==1)&&(state[3][i]==1)){
            return 1;
        }
        
        if((state[3][i]==-1)&&(state[2][i]==-1)&&(state[3][i]==-1)){
            return 0;
        }
    }
    //checking for 3 1s in both the diagonals and return 1 for win
    if(((state[1][1]==1)&&(state[2][2]==1)&&(state[3][3]==1))||((state[3][1]==1)&&(state[2][2]==1)&&(state[1][3]==1))){
        return 1;
    }
    //checking for 3 1s in both the  and return 0 for lose
    if(((state[1][1]==-1)&&(state[2][2]==-1)&&(state[3][3]==-1))||((state[3][1]==-1)&&(state[2][2]==-1)&&(state[1][3]==-1))){
        return 0;
    }
    int vacancy=spaceRandom();
    if(vacancy==0){
          return 4;    //4 stands for draw
   }

    return 2; //2 stands for game not over

}
