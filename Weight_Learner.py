#!/usr/bin/python
import numpy as np
'''
Here WEIGHTS is a dictionary defining the weights of all the possible move models.
Now lets assume that we are in state S(n), and we have a variety of possiblities for S(n+1),
Now for every possiblity of S(n+1), we will count value for that possiblity using grossVal function.
The grossVal function will calculate using 8 model classifiers.
The function isLost() is defined explicitely, which will take the indices of opponent's move and
tell whether we lost or not.
Other than isLost(), we dont need further calculations of opponent.
'''
WEIGHTS = {
    'win' : 1,
    'fork' : 0.5,
    'block' : 0.5,
    'center' : 0.5,
    'oppCprner' : 0.5,
    'emptyCorner' : 0.5,
    'emptySide' : 0.5,
    'oppForkBlock' : 0.5
    }
def checkCombinations(Matrix,i,j) :
    vertical_sum = np.sum( Matrix[:,j])
    horizontal_sum = np.sum(Matrix[i])
    diag_sum = np.trace(Matrix)
    diag_flip_sum = np.trace( np.flip(Matrix, 0))
    values = [vertical_sum, horizontal_sum, diag_sum, diag_flip_sum]
    return values

def isFork(Matrix,i,j) :
    gridSize = np.shape(Matrix)[0]
    values = checkCombinations(Matrix,i,j)
    # print(values)
    if values.count(gridSize - 2) > 1 :
        return float(1)
    return float(0)

def isOppForkBlocked(Matrix,i,j) :
    gridSize = np.shape(Matrix)[0]
    values = checkCombinations(Matrix,i,j)
    # print(values)
    if values.count(-1 * gridSize + 2) > 1 :
        return float(1)
    return float(0)

def isWin(Matrix,i,j) :
    gridSize = np.shape(Matrix)[0]
    values = checkCombinations(Matrix,i,j)
    values.sort()
    if values[-1] == gridSize - 1 :
        return float(1)
    return float(0)

def isBlock(Matrix,i,j) :
    gridSize = np.shape(Matrix)[0]
    values = checkCombinations(Matrix,i,j)
    values.sort()
    if values[0] == -gridSize + 1 :
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
    if values[0] == -1 * gridSize +1 :
        return float(1)
    return float(0)

def grossVal(Matrix,i,j) :
    weights_num = np.array([[ WEIGHTS["win"], WEIGHTS["fork"], WEIGHTS["block"], WEIGHTS["oppForkBlock"], WEIGHTS["center"], WEIGHTS["oppCprner"], WEIGHTS["emptyCorner"], WEIGHTS["emptySide"] ]])
    classifiers_num = np.array([[ isWin(Matrix,i,j), isFork(Matrix,i,j), isBlock(Matrix,i,j), isOppForkBlocked(Matrix,i,j), isCenter(Matrix,i,j), isOppCorner(Matrix,i,j),isEmptyCorner(Matrix,i,j),isEmptySide(Matrix,i,j) ]])
    # classifiers_num.astype(float)
    VAL = np.sum(np.multiply(weights_num, classifiers_num))
    return VAL
# any size of mat will e okay Here
# numpy doesnt require to mention size
mat = np.array([
    [1, -1, 1],
    [0, 0, -1],
    [0, 0, 0]
    ])
print(mat,"\n")
print("Is it forking at [1,1]", isFork(mat,1,1))
print("And value for move [1,1] is : ", grossVal(mat,1,1))

print("Is it winning at [2,0]", isWin(mat,2,0))
print("And value for move [2,0] is : ", grossVal(mat,2,0))

print("Is it blocking at [2,2]", isBlock(mat,2,2))
print("And value for move [2,2] is : ", grossVal(mat,2,2))

print("Is it occupiying center at [1,1]", isCenter(mat,1,1))
print("And value for move [1,1] is : ", grossVal(mat,1,1))

print("Is it occupiying opp. corner at [2,2]", isOppCorner(mat,2,2))
print("And value for move [2,2] is : ", grossVal(mat,2,2))

print("Is it occupiying empty corner at [2,2]", isEmptyCorner(mat,2,2))
print("And value for move [2,2] is : ", grossVal(mat,2,2))

print("Is it occupiying empty side at [2,1]", isEmptySide(mat,2,1))
print("And value for move [2,1] is : ", grossVal(mat,2,1))

print("Is it blocking opponent fork at [1,1]", isOppForkBlocked(mat,1,1), "\n")
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
