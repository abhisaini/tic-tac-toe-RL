# tic-tac-toe-RL

## Problem Statement

To simulate a tic-tac-toe game between a reinforcement-learning agent and
a random agent with varying numbers of training games, with different policies 
for learning. And, to see the trade-off between the exploration and expoitation
policy by varying the number of grid-size.


##  How to Run

Installation
-----
- Clone the directory from github,and navigate to the directory tic-tac-toe-rl 

            git clone https://github.com/abhisaini/tic-tac-toe-rl
            
 -Create Directories to store your outputs by running following commands
 
            mkdir Text-Results Plot-Results Plot-CompEp-Results Text-CompEp-Results
      
 * Now in each of the above directories create sub-directories to store results sorted by gridsize of tic-tac-toe.
       So in each Directory 
    
            mkdir Grid-3 Grid-4 Grid-5 Grid-6 Grid-7
            
            


 Running the game
 ----
 - If u have python 2.7 installed and have C++11,Run following command (in tic-tac-toe-rl directory) :
            
            g++ minimal.cpp -std=c++11 -I/usr/include/python2.7 -lpython2.7
            
