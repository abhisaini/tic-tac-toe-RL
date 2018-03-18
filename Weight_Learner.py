#!/usr/bin/python
import numpy as np
import random as rand
'''
Here WEIGHTS is a dictionary defining the weights of all the possible move models.
Now lets assume that we are in state S(n), and we have a variety of possiblities for S(n+1),
Now for every possiblity of S(n+1), we will count value for that possiblity using grossVal function.
The grossVal function will calculate using 8 model classifiers.
The function isLost() is defined explicitely, which will take the indices of opponent's move and
tell whether we lost or not.
Other than isLost(), we dont need further calculations of opponent.
'''
PLAYER_X = 1
PLAYER_O = -1
GREEDY = 1
EXPLORATERY = 0
EPSILON = 0.01
WIN = 1
DRAW = 2
LOST = 4
WEIGHTS = {
    'win' : -10.0,
    'fork' : 0.0,
    'block' : 0.0,
    'center' : 0.0,
    'oppCprner' : 0.0,
    'emptyCorner' : 0.0,
    'emptySide' : 0.0,
    'oppForkBlock' : 0.0
    }
def checkCombinations(Matrix,i,j) :
    new_Matrix = Matrix.copy()
    new_Matrix[i][j] = 1
    vertical_sum = np.sum( new_Matrix[:,j])
    horizontal_sum = np.sum(new_Matrix[i])
    diag_sum = np.trace(new_Matrix)
    diag_flip_sum = np.trace( np.flip(new_Matrix, 0))
    values = [vertical_sum, horizontal_sum, diag_sum, diag_flip_sum]
    return values

def isFork(Matrix,i,j) :
    gridSize = np.shape(Matrix)[0]
    values = checkCombinations(Matrix,i,j)
    # print(values)
    if values.count(gridSize - 1) > 1 :
        return float(1)
    return float(0)

def isOppForkBlocked(Matrix,i,j) :
    gridSize = np.shape(Matrix)[0]
    values = checkCombinations(Matrix,i,j)
    # print(values)
    if values.count(-1 * gridSize + 3) > 1 :
        return float(1)
    return float(0)

def isWin(Matrix,i,j) :
    gridSize = np.shape(Matrix)[0]
    values = checkCombinations(Matrix,i,j)
    values.sort()
    if values[-1] == gridSize  :
        return float(1)
    return float(0)

def isBlock(Matrix,i,j) :
    gridSize = np.shape(Matrix)[0]
    values = checkCombinations(Matrix,i,j)
    values.sort()
    if values[0] == -gridSize + 2 :
        return float(1)
    return float(0)

def isCenter(Matrix,i,j) :
    gridSize = np.shape(Matrix)[0]
    pm = int((gridSize % 2 -1 + gridSize) /2)
    # print (pm)
    if i == pm and j == pm :
        return float(1)
    return float(0)

def isOppCorner(Matrix,i,j) :
    gridSize = np.shape(Matrix)[0]
    if Matrix[gridSize - 1 - i][gridSize - 1 - i] == 1 :
        if (i == 0 or i == gridSize-1) and (j == 0 or j == gridSize-1) :
            # print (1)
            return float(1)
    return float(0)

def isEmptyCorner(Matrix,i,j) :
    gridSize = np.shape(Matrix)[0]
    if (i == 0 or i == gridSize-1) and (j == 0 or j == gridSize-1) :
        # print (1)
        return float(1)
    return float(0)
def isEmptySide(Matrix,i,j) :
    gridSize = np.shape(Matrix)[0]
    pm = int((gridSize % 2 -1 + gridSize) /2)
    if (i == 0 and j == pm) or (i == pm and j == 0) or (i == gridSize-1 and j == pm) or (i == pm or j == gridSize-1) :
        return float(1)
    return float(0)

# to be called after opponent's move with opponent's move indices
def isLost(Matrix,i,j) :
    gridSize = np.shape(Matrix)[0]
    values = checkCombinations(Matrix,i,j)
    values.sort()
    if values[0] == -1 * gridSize  :
        return float(1)
    return float(0)

def isDraw(Matrix) :
    empty_i, empty_j = np.where(Matrix == 0)
    if len(empty_i) == 0 :
        return 1
    else:
        return 0


# # # # # # # # # # # # # # # # # # # # # # # #
def grossVal(Matrix,i,j) :
    weights_num = np.array([[ WEIGHTS["win"], WEIGHTS["fork"], WEIGHTS["block"], WEIGHTS["oppForkBlock"], WEIGHTS["center"], WEIGHTS["oppCprner"], WEIGHTS["emptyCorner"], WEIGHTS["emptySide"] ]])
    classifiers_num = np.array([[ isWin(Matrix,i,j), isFork(Matrix,i,j), isBlock(Matrix,i,j), isOppForkBlocked(Matrix,i,j), isCenter(Matrix,i,j), isOppCorner(Matrix,i,j),isEmptyCorner(Matrix,i,j),isEmptySide(Matrix,i,j) ]])
    # classifiers_num.astype(float)
    VAL = np.sum(np.multiply(weights_num, classifiers_num))
    return VAL

def randomMove(Matrix,player) :
    # indices for zeros ( empty boxes)
    empty_i, empty_j = np.where(Matrix == 0)
    if len(empty_i) >= 1 :
        move_tmp = rand.randint(0, len(empty_i) - 1)
        move_i = empty_i[move_tmp]
        move_j = empty_j[move_tmp]
        Matrix[move_i][move_j] = player
    else :
        print ("Game Over")

        # Needed arguments can be put later
    return move_i, move_j

def chooseBest(Matrix,possible_next_i,possible_next_j) :
    values = []
    total_possiblities = len(possible_next_i)
    for x in range(total_possiblities) :
        tmp_i = possible_next_i[x]
        tmp_j = possible_next_j[x]
        values.append(grossVal(Matrix,tmp_i,tmp_j))
    max_val = max(values)
    indices_max = [i for i,x in enumerate(values) if x == max_val]
    move_tmp = indices_max[rand.randint(0, len(indices_max) - 1)]
    move_i = possible_next_i[move_tmp]
    move_j = possible_next_j[move_tmp]
    Matrix[move_i][move_j] = 1
    print (values)
    return move_i,move_j

def greedyMove(Matrix) : 
    empty_i, empty_j = np.where(Matrix == 0) 
    possible_next_i =[]
    possible_next_j =[]
    if len(empty_i) >= 1 :
        for move_tmp in range(len(empty_i)) :
            # tmp = np.copy(Matrix)
            # print(Matrix,tmp)
            move_i = empty_i[move_tmp]
            move_j = empty_j[move_tmp]
            # tmp[move_i][move_j] = 1
            possible_next_i.append(move_i)
            possible_next_j.append(move_j)
            
        # possible_next_i = np.delete(possible_next,0, axis=0) 
        
        return chooseBest(Matrix,possible_next_i,possible_next_j)   
            # tmp[move_i][move_j] = 0
    print(possible_next_i,possible_next_j)
    
def nextMove(Matrix,player,policy) :
    if player == PLAYER_X and policy == EXPLORATERY :
        return randomMove(Matrix,player)
    if player == PLAYER_O :
        return randomMove(Matrix,player)
    if player == PLAYER_X and policy == GREEDY :
        return greedyMove(Matrix)

def gamePlay(Matrix,policy):
    old_Matrix = Matrix.copy()
    move_x_i, move_x_j = nextMove(Matrix,1,policy)
    print("gross value : ", grossVal(old_Matrix,move_x_i,move_x_j))
    Matrix[move_x_i][move_x_j] = 1
    if isWin(old_Matrix,move_x_i,move_x_j) == 1 :
        Matrix[move_x_i][move_x_j] = 1
        print(Matrix)
        print ("WIN")
        return WIN
    
    
    if isDraw(Matrix) == 1 :
        Matrix[move_x_i][move_x_j] = 1
        print(Matrix)
        print ("DRAW")
        return DRAW
    print(Matrix)
    old_Matrix = Matrix.copy()
    move_o_i, move_o_j = nextMove(Matrix,-1,policy)
    Matrix[move_o_i][move_o_j] = -1
    if isLost(old_Matrix,move_x_i,move_x_j) == 1 :
        Matrix[move_x_i][move_x_j] = 1
        print(Matrix)
        print ("LOST")
        return LOST
    
    print(Matrix)
    gamePlay(Matrix,policy)

def games() :
    
    game_count = 1000
    win_count = 0
    lost_count = 0
    draw_count = 0
    try :
        to_explore = 1/EPSILON 
    except : to_explore = -1
    for i in range(game_count) :
        if i%to_explore == 0 :
            policy = EXPLORATERY
        else :
            policy = GREEDY
        Matrix = np.array([
            [0, 0, 0],
            [0, 0, 0],
            [0, 0, 0]
        ])
        game_result = gamePlay(Matrix,policy)
        if game_result == WIN :
            win_count += 1
        if game_result == DRAW :
            draw_count += 1
        if game_result == LOST :
            lost_count += 1
    win_percent = win_count/game_count
    draw_percent = draw_count/game_count
    lost_percent = lost_count/game_count
    return win_percent,draw_percent,lost_percent


# any size of mat will be okay Here
# numpy doesnt require to mention size
mat = np.array([
    [0, 0, 0],
    [0, 0, 0],
    [0, 0, 0]
    ])
gamePlay(mat,1)
mat = np.array([
    [0, -1, 0],
    [0, 0, 1],
    [-1, 1, 0]
])
print(mat,"\n")
print("Is it forking at [1,1]", isFork(mat,1,1))
print("And value for move [1,1] is : ", grossVal(mat,1,1))

print("Is it winning at [2,2]", isWin(mat,2,2))
print("And value for move [2,2] is : ", grossVal(mat,2,2))

print("Is it blocking at [2,2]", isBlock(mat,1,1))
print("And value for move [2,2] is : ", grossVal(mat,2,2))

print("Is it occupiying center at [1,1]", isCenter(mat,1,1))
print("And value for move [1,1] is : ", grossVal(mat,1,1))

print("Is it occupiying opp. corner at [2,2]", isOppCorner(mat,2,2))
print("And value for move [2,2] is : ", grossVal(mat,2,2))

print("Is it occupiying empty corner at [2,2]", isEmptyCorner(mat,2,2))
print("And value for move [2,2] is : ", grossVal(mat,2,2))

print("Is it occupiying empty side at [2,1]", isEmptySide(mat,2,1))
print("And value for move [2,1] is : ", grossVal(mat,2,1))

print("Is it blocking opponent fork at [1,1]", isOppForkBlocked(mat,0,2), "\n")
print(WEIGHTS)
print(isLost(mat,2,2))

'''
class Employee:
    'Common base class for all employees'
    empCount = 0

    def __init__(self, name, salary):
        self.val = 0.5
        self.salary = salary
        Employee.empCount += 1

    def displayCount(self):
        print ("Total Employee %d" % Employee.empCount)
    def displayEmployee(self):
        return self.aa

"This would create first object of Employee class"
emp1 = Employee("Zara", 2000)
"This would create second object of Employee class"
emp2 = Employee("Manni", 5000)
emp1.displayEmployee()
print (emp2.displayEmployee())
print ("Total Employee %d" % Employee.empCount)

class State :
    def _init_(self) :
        self.Matrix = [ [0, 0, 0 ] , [0, 0, 0 ] , [0, 0, 0 ] ]
        self.value = 0.5
    def val(self) :
        return self.value
    def mat(self) :
        return self.Matrix
S = State()
print S.val()

0 y 0
x 0 y
0 x 0
'''


'''
import Weight_Learner as wl
import numpy as np
mat = np.array([ [0,0,0],[0,0,0],[0,0,0]])
wl.greedyMove(mat)

'''