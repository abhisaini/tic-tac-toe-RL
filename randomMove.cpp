#include <stdio.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>     /* srand, rand */

using namespace std;

//function returns the total no possible moves for random player at certain state 
int spaceRandom(int state[3][3]){
    int val=1;
    for(int i=0;i<3;i++){
       for(int j=0;j<3;j++){
           if(state[i][j]!=0){
               val++;
           }
       }
   }
    return 10-val;
}
//to get the random move from possible moves
int randomInput(int arr[],int size){
    int input=rand()%size;
        return arr[input];
    

}
//to make the move i.e. to change the value from 0 to -1 for possible move
void randomChanged(int state[3][3],int move){
     int movex=move/3,movey=move%3;       
     state[movex][movey]= -1;
     return ;
 }
 

//the main function to call to decide random player's move.It will play the move by itself
void randomMove(int state[3][3]){
    int space=spaceRandom(state);
    int i=0,j=0,index=0;
    int freespace[space];
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            if(state[i][j]==0){
                freespace[index]=3*i+j;
                index++;
            }
        }
    }
    int move=randomInput(freespace,space);
    randomChanged(state,move);
    return ;
}
 
int main( ) {

   int state[3][3];
   for(int i=0;i<3;i++){
       for(int j=0;j<3;j++){
           cin>>state[i][j];
       }
   }
    randomMove(state);
   for(int i=0;i<3;i++){
       for(int j=0;j<3;j++){
           cout<<state[i][j]<<" ";
       }
       cout<<endl;
   }
   

   return 0;
}
