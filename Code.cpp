#include "pch.h"
#include <iostream>
#include<cmath>
using namespace std;
#define N 4 //the size of a board
#define goalstate TNode->state[1][1] == 1 && TNode->state[2][1] == 2 && TNode->state[3][1] == 3
#define okleft (TNode->location[1] - 1 >= 0 && TNode->his != 3 && !(TNode->location[1] - 1 == obstacleLocation[1]&&TNode->location[0] == obstacleLocation[0]))
#define okright (TNode->location[1] + 1 < N && TNode->his != 4 && !(TNode->location[1] + 1 == obstacleLocation[1]&&TNode->location[0] == obstacleLocation[0]))
#define okup (TNode->location[0] - 1 >= 0 && TNode->his != 1 && !(TNode->location[0] - 1 == obstacleLocation[0]&&TNode->location[1] == obstacleLocation[1]))
#define okdown (TNode->location[0] + 1 < N && TNode->his != 2 && !(TNode->location[0] + 1 == obstacleLocation[0]&&TNode->location[1] == obstacleLocation[1]))
int NodeNumber = 0;//record how many nodes are generated during a search
int obstacleLocation[2] = { 2,2 };
//Node Class
class pathnode {
public:
	int depth = 0;//The depth of the node
	int state[N][N] = { {0,0,0,0},{0,0,0,0},{0,0,5,0},{1,2,3,4} };//the board statement,1 means A,2 means B,3 means C,4 means man,5 means obstacle.
	int location[2] = { 3,3 };//the location of man
	int his = 0;//Record the history step
	int vaild = 1;//useful in Astar reasearch,to make node be 'dark'
	int hvalue = 1000;//useful in Astar research
	pathnode* father = NULL;//father pointer
	pathnode* left = NULL;//left child pointer
	pathnode* right = NULL;
	pathnode* up = NULL;
	pathnode* down = NULL;
	pathnode* stepnext = NULL;//useful in printing the complete route
	pathnode* quenext = NULL;//useful in breadfirst research and Astar research
};
//Copy the board statement
void copystate(pathnode* A, pathnode* B) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			B->state[i][j] = A->state[i][j];
		}
	}
}
//Generate Child Node.
//For the parameter operate, 1 means down, 2 means up, 3 means right, 4 means left. 
void updateChildNode(pathnode* TNode, int operate) {
	//if the operate is go down
	if (operate == 1) {
		//generate new node
		TNode->down = new pathnode();
		NodeNumber += 1;
		TNode->down->father = TNode;
		//Assign the value to a new node
		//Record the previous step
		TNode->down->his = 1;
		//Record the depth of the node
		TNode->down->depth = TNode->depth + 1;
		cout << "go down and " << TNode->depth + 1 << endl;
		//Change the location of man
		TNode->down->location[0] = TNode->location[0] + 1;
		TNode->down->location[1] = TNode->location[1];
		//Change the board statement
		copystate(TNode, TNode->down);
		int Tem = TNode->down->state[TNode->location[0]][TNode->location[1]];
		TNode->down->state[TNode->location[0]][TNode->location[1]] = TNode->down->state[TNode->location[0] + 1][TNode->location[1]];
		TNode->down->state[TNode->location[0] + 1][TNode->location[1]] = Tem;
	}
	//if the operate is go up
	if (operate == 2) {
		TNode->up = new pathnode();
		NodeNumber += 1;
		TNode->up->father = TNode;
		TNode->up->depth = TNode->depth + 1;
		cout << "go up and " << TNode->depth + 1 << endl;
		TNode->up->his = 2;
		TNode->up->location[0] = TNode->location[0] - 1;
		TNode->up->location[1] = TNode->location[1];
		copystate(TNode, TNode->up);
		int Tem = TNode->state[TNode->location[0]][TNode->location[1]];
		TNode->up->state[TNode->location[0]][TNode->location[1]] = TNode->up->state[TNode->location[0] - 1][TNode->location[1]];
		TNode->up->state[TNode->location[0] - 1][TNode->location[1]] = Tem;
	}
	//if the operate is go right
	if (operate == 3) {
		TNode->right = new pathnode();
		NodeNumber += 1;
		TNode->right->father = TNode;
		TNode->right->his = 3;
		TNode->right->depth = TNode->depth + 1;
		cout << "go right and  " << TNode->depth + 1 << endl;
		TNode->right->location[1] = TNode->location[1] + 1;
		TNode->right->location[0] = TNode->location[0];
		copystate(TNode, TNode->right);
		int Tem = TNode->right->state[TNode->location[0]][TNode->location[1]];
		TNode->right->state[TNode->location[0]][TNode->location[1]] = TNode->right->state[TNode->location[0]][TNode->location[1] + 1];
		TNode->right->state[TNode->location[0]][TNode->location[1] + 1] = Tem;
	}
	//if the operate is go left
	if (operate == 4) {
		TNode->left = new pathnode();
		NodeNumber += 1;
		TNode->left->father = TNode;
		TNode->left->his = 4;
		TNode->left->depth = TNode->depth + 1;
		cout << "go left and " << TNode->depth + 1 << endl;
		copystate(TNode, TNode->left);
		TNode->left->location[1] = TNode->location[1] - 1;
		TNode->left->location[0] = TNode->location[0];
		int Tem = TNode->left->state[TNode->location[0]][TNode->location[1]];
		TNode->left->state[TNode->location[0]][TNode->location[1]] = TNode->left->state[TNode->location[0]][TNode->location[1] - 1];
		TNode->left->state[TNode->location[0]][TNode->location[1] - 1] = Tem;
	}
}
//Compute the Cost of path: node depth+Manhattan distance
int costValue(pathnode* TNode) {
	int AlocationX,AlocationY,BlocationX,BlocationY,ClocationX,ClocationY,score = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			//Find A's position
			if (TNode->state[i][j] == 1) {
				AlocationX = i;
				AlocationY = j;
			}
			//Find B's positon
			if (TNode->state[i][j] == 2) {
				BlocationX = i;
				BlocationY = j;
			}
			//Find C's position
			if (TNode->state[i][j] == 3) {
				ClocationX = i;
				ClocationY = j;
			}
		}
	}
	//compute the cost function
	score = abs(AlocationX - 1) + abs(AlocationY - 1) + abs(BlocationX - 2) + abs(BlocationY - 1) + abs(ClocationX - 3) + abs(ClocationY - 1)+TNode->depth;
	return score;
}

//Deep First 
pathnode* deepfirst(pathnode* TNode) {
	if (goalstate)
		return TNode;
	else {
		//Determine if i can go left
		if (okleft) {
			//generate child node of going left
			updateChildNode(TNode, 4);
			//determine whether there is a resolution
			pathnode* L=deepfirst(TNode->left);
			if (L != NULL) {
				return L;
			}
		}
		//generate the right step child node
		if (okright) {
			updateChildNode(TNode, 3);
			pathnode* R = deepfirst(TNode->right);
			if (R != NULL) {
				return R;
			}
		}
		//generate the up step child node
		if (okup) {
			updateChildNode(TNode, 2);
			pathnode* U=deepfirst(TNode->up);
			if (U != NULL) {
				return U;
			}
		}
		//generate the down step child node
		if (okdown) {
			updateChildNode(TNode, 1);
			pathnode* D=deepfirst(TNode->down);
			if (D != NULL) {
				return D;
			}
		}
		//release node
		delete TNode;
		return NULL;
	}
}
//Breadth First 
pathnode* breadthFirst(pathnode* TNode) {
	pathnode* start = TNode;
	pathnode* end = TNode;
	if (TNode == NULL)
		return NULL;
	if (goalstate)
		return TNode;
	while(true) {
		//determine whether the node is the aim node
		if (goalstate) {
			break;
		}
		//generate the left step child node
		if (okleft) {
			updateChildNode(TNode, 4);
			//put the node into a queue
			end->quenext = TNode->left;
			end = TNode->left;
			
		}
		//generate the right step child node
		if (okright) {
			updateChildNode(TNode, 3);
			end->quenext = TNode->right;
			end = TNode->right;
			
		}
		//generate the up step child node
		if (okup) {
			updateChildNode(TNode, 2);
			end->quenext = TNode->up;
			end = TNode->up;
			
		}
		//generate the down step child node
		if (okdown) {
			updateChildNode(TNode, 1);
			end->quenext = TNode->down;
			end = TNode->down;
		}
		//Chang the queue head pointer
		TNode = TNode->quenext;
		start = TNode;
	}
	return TNode;
} 
pathnode* Astar(pathnode* TNode) {
	int min = 1000;
	pathnode* start = TNode;
	pathnode* head = TNode;
	pathnode* end = TNode;
	//point the node which has the lowest cost value
	pathnode* minNode = NULL;
	//point the node which is the aim node
	pathnode* resultnode = NULL;
	if (goalstate)
		return TNode;
	while (true) {
		if (goalstate) {
			break;
		}
		min = 1000;
		start = head;
		//make the vaild bit of the node to zero (Make the node be 'Dark')
		TNode->vaild = 0;
		//generate the left step child node
		if (okleft) {
			updateChildNode(TNode, 4);
			//compute the cost value of the node
			TNode->left->hvalue = costValue(TNode->left);
			//put the node into a queue
			end->quenext = TNode->left;
			end = TNode->left;

		}
		//generate the right step child node
		if (okright) {
			updateChildNode(TNode, 3);
			TNode->right->hvalue = costValue(TNode->right);
			end->quenext = TNode->right;
			end = TNode->right;

		}
		//generate the up step child node
		if (okup) {
			updateChildNode(TNode, 2);
			TNode->up->hvalue = costValue(TNode->up);
			end->quenext = TNode->up;
			end = TNode->up;

		}
		//generate the down step child node
		if (okdown) {
			updateChildNode(TNode, 1);
			TNode->down->hvalue=costValue(TNode->down);
			end->quenext = TNode->down;
			end = TNode->down;
		}
		//find the node which has the lowest cost value
		while (start != end) {
			if (start->vaild == 1) {
				if (min > start->hvalue) {
					min = start->hvalue;
					minNode = start;
				}
			}
			start = start->quenext;
		}
		TNode = minNode;
	}
	return TNode;
}
pathnode* deeplimited(pathnode* TNode, int k) {
	if (goalstate)
		return TNode;
	else {
		if (TNode->depth > k)
			return NULL;
		if (okleft) {
			updateChildNode(TNode, 4);
			pathnode* L = deeplimited(TNode->left, k);
			if (L != NULL) {
				return L;
			}
		}
		//generate the right step child node
		if (okright) {
			updateChildNode(TNode, 3);
			pathnode* R = deeplimited(TNode->right, k);
			if (R != NULL) {
				return R;
			}
		}
		//generate the up step child node
		if (okup) {
			updateChildNode(TNode,2);
			pathnode* U = deeplimited(TNode->up, k);
			if (U != NULL) {
				return U;
			}
		}
		//generate the down step child node
		if (okdown) {
			updateChildNode(TNode, 1);
			pathnode* D = deeplimited(TNode->down, k);
			if (D != NULL) {
				return D;
			}
		}
		return NULL;
	}
}
pathnode* iterativeDeepening(pathnode* TNode) {
	pathnode* result = NULL;
	for (int m = 0;; m++) {
		result = deeplimited(TNode, m);
		if (result != NULL)
			break;
	}
	return result;
}
//print a node's board
void printstate(pathnode* TNode) {
	cout << "---------" << endl;
	//print different sign
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (TNode->state[i][j] == 0)
				cout << "| ";
			if (TNode->state[i][j] == 1)
				cout << "|A";
			if (TNode->state[i][j] == 2)
				cout << "|B";
			if (TNode->state[i][j] == 3)
				cout << "|C";
			if (TNode->state[i][j] == 4)
				cout << "|M";
			if (TNode->state[i][j] == 5) {
				cout << "|*";
			}
			if ((j + 1) % N == 0) {
				cout <<"|"<<endl;
				cout << "---------" << endl;
			}
		}
	}
}
//print all the steps through a goal node
void printprocess(pathnode* A,pathnode* Head) {
	//Find the complete route to reach the aim node
	while (A != Head) {
		A->father->stepnext = A;
		A = A->father;
	}
	//Print the route
	while (A->state[1][1] != 1 || A->state[2][1] != 2 || A->state[3][1] != 3) {
		cout << "The " << A->depth << " step is :" << endl;
		printstate(A);
		A = A->stepnext;
	}
	cout << "The " << A->depth << " step is :" << endl;
	printstate(A);
}
int main()
{
	pathnode* Head = new pathnode();
	pathnode*A = Astar(Head);
	if (A == NULL)
		cout << "this way is in infinited loop" << endl;
	else
		printprocess(A, Head);
	cout << "The number of generated node is " << NodeNumber << "." << endl;
	cout << "The search is end"<<endl;
}

