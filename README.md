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
            
- Create Directories to store your outputs by running following command
 
            bash prepare.sh
                  
            


 Running the game
 ----
 - If u have python 2.7 installed and have C++11,Run following command (in tic-tac-toe-rl directory) :
            
            g++ filename.cpp -std=c++11 -I/usr/include/python2.7 -lpython2.7

- For python 3.6 

            g++ filename.cpp -I/usr/include/python3.6 -lpython3.6
            
- For python 4

            g++ example.cpp -I/usr/local/include/fancy-python4 -L/usr/local/lib -lfancy-python4            
            
About Files
----
- Here main.cpp contains a simple tic-tac-toe learner 
