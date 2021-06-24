#include<stdio.h>
#include<stdlib.h>

#define MAX 101		// the maximum number of nodes

int N, M, S;
int weight[MAX], parent[MAX] = { -1 }, sum[MAX], treeMap[MAX][MAX], childNumber[MAX], result[MAX][MAX];
int stack[MAX], stackPointer = -1, resultPointer = -1;
/*************************************************************************
					Variables Used

* N: The number of nodes in the tree.

* M: The number of non-leaf nodes in the tree

* S: The given weight number we are going to search

* weight[]: An array that stores the weight of each node

* parent[]: An array that stores the parent of each node (used in storing
			the result)

* sum[]: An array that stores the sum of weights from the root to each node

* treeMap[][]: A matrix that store the tree. treeMap[i][] is an array that 
			stores all the children of node i.  

* childNumber[]: An array that stores the number of children of each node.

* result[]: An array that stores the result.

* stack[]: A stack that stores each sequence of result (to get the reverse order)

* stackPointer: the pointer of stack[].

* resultPointer: the pointer of result[].
*************************************************************************/


void depthFirstSearch(int currentId);
void push(int* stack, int val);
void pop(int* stack);
int top(int* stack);
int isEmpty(int* stack);
void swap(int* arr1, int* arr2);
int check(int* array1, int* array2);
void storeResult(int id);
void printResult(void);

/*********************************************************
				Functions

* depthFirstSearch: Using depth first search to find the 
			equal paths.

* push; pop; top; isEmpty: Basic operations in stack.

* check: To determine two arrays which is larger.

* swap: Swap the two arrays (exchange all the elements)

* storeResult: store one sequence of result. 

* printResult: Sort the result array and output the 
			results in specific format.

*********************************************************/


int main()
{
	int i = 0, j = 0;
	int id = 0, k = 0, childId = 0;

	scanf("%d %d %d", &N, &M, &S);		// read in the N,M,S

	for (i = 0; i < N; i++)				//read in the weight of each node
		scanf("%d", &weight[i]);

	if (M == 0) {						// there's only one node
		if (weight[0] == S) printf("%d\n", weight[0]);
		return 0;
	}

	for (i = 0; i < N; i++)				// initialize the sum[] array
		sum[i] = weight[i];

	for (i = 0; i < M; i++) {
		scanf("%d %d", &id, &k);		// read in the id and the number of children
		childNumber[id] = k;
		for (j = 0; j < k; j++) {
			scanf("%d", &childId);
			parent[childId] = id;		// initialize the parent[] array
			treeMap[id][j] = childId;	// initialize the treeMap[][] matrix 
		}

	}

	depthFirstSearch(0);	// using depth first search to find the equal path, starting from the root

	printResult();			// output the result in specific format

	return 0;
}

void depthFirstSearch(int currentId)
{
	int nextChild = 0, i = 0;
	for (i = 0; i < childNumber[currentId]; i++) {				// traverse all the children of current node
		nextChild = treeMap[currentId][i];						// get the child node id
		sum[nextChild] = sum[currentId] + weight[nextChild];	// update the sum[nextChild] 

		if (sum[nextChild] == S) {			// if the path weight is equal to S
			if (childNumber[nextChild] != 0)	// not a leaf node, ignore
				continue;
			else  storeResult(nextChild);		// a leaf node, store result
		}
		else if (sum[nextChild] < S)			// if the sum is less than S, start searching next level
			depthFirstSearch(nextChild);
	}
}

void push(int* stack, int val)
{
	stack[++stackPointer] = val;
}

void pop(int* stack)
{
	stackPointer--;
}

int top(int* stack)
{
	return stack[stackPointer];
}

int isEmpty(int* stack)
{
	return stackPointer == -1;
}

// since the DFS find the leaf node, we need to store the result starting from the root, we need to use the stack to reverse it
void storeResult(int id)
{
	int i = 0;
	while (id != -1) {		// push the path to the stack
		push(stack, weight[id]);
		id = parent[id];
	}
	resultPointer++;
	while (!isEmpty(stack)) {
		result[resultPointer][i++] = top(stack);  // store the path nodes from root to leaf
		pop(stack);
	}
}

void swap(int* arr1, int* arr2)
{
	int i = 0;
	int* temp = (int*)malloc(MAX * sizeof(int));
	for (i = 0; i < MAX; i++) {
		temp[i] = arr1[i];
		arr1[i] = arr2[i];
		arr2[i] = temp[i];
	}
	free(temp);
}

int check(int* array1, int* array2)
{
	int i = 0;
	for (i = 0; i < MAX; i++) {
		if (array1[i] == array2[i])
			continue;
		else if (array1[i] > array2[i])
			return 1;
		else return -1;
	}
	return 0;
}

void printResult(void) // sort the result sequence by insertion sort
{
	int i = 0, j = 0, k = 0;
	int* temp = (int*)malloc(sizeof(int) * MAX), * key = (int*)malloc(sizeof(int) * MAX);
	for (i = 1; i <= resultPointer; i++) {
		for (k = 0; k < MAX; k++)
			key[k] = result[i][k];
		j = i - 1;
		while (j >= 0 && check(key, result[j]) == 1) {
			swap(result[j + 1], result[j]);
			j--;
		}
		for (k = 0; k < MAX; k++)
			result[j + 1][k] = key[k];
	}
	for (i = 0; i <= resultPointer; i++) {
		printf("%d", result[i][0]);
		j = 1;
		while (result[i][j] != 0) {
			printf(" %d", result[i][j++]);
		}
		printf("\n");
	}
}
