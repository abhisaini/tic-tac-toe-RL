#include <bits/stdc++.h>
using namespace std;
typedef std::vector<int> Vector;
typedef std::vector<std::vector<int> > Matrix;
class state { // inorder to describe  a state with the positions of X and O and its value function
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
struct NODE
{
    int index;
    struct NODE* val_X;
    struct NODE* val_N;
    struct NODE* val_O;
    state St(3);
     
};
struct NODE* newNode(int index){
     struct NODE* nod = new NODE ;
     nod->index = index;
     nod->val_X = NULL;
     nod->val_N = NULL;
     nod->val_O = NULL;
 
     return(nod);
}
Vector CopyToVec(Matrix m){
    int size = m.size();
    Vector copied;
    int ctr = 0;
    for (int i = 0 ; i < size ; i++){
        for (int j = 0 ; j < size ; j++){
            copied.push_back(m[i][j]);
        }
    }
    return copied;
}
NODE* root = newNode(0);
int getVal(Matrix mat){
    Vector address = CopyToVec(newState.mat);
    int len = address.size();
    for (int i=0 ; i < len ; i++){
        
        
    }
    
}

int main() {
	// your code goes here
	state newState(3);
	newState.mat[0][1] = 1 ;
	newState.mat[1][1] = -1;
	cout << newState.mat.size();
	Vector some = CopyToVec(newState.mat);
	cout << some[1] ;
	cout << some[4];
	cout << some[8];
	return 0;
}
