#include<stdio.h>
#include<stdlib.h>

#define MAX 101	

typedef enum { false, true }bool;

/*********************************************************************
						  Global Variables

@ int* tips[]: An array used to store the pointers which
		point to the tip array of each fruit. For example,
		tips[i] points to an array whose elements cannot 
		be eaten with i. We use the function "realloc" to 
		minimize the space.

@ tipNum[]: The number of unsafe fruits associated with each fruit.
		That is, tipNum[i] is equal to the number of elements 
		in tips[i].

@ price[]: An array used to store the price of fruit. The 
		index of this array is fruit id.

@ index[]: An array used to convert the index(range from 0 to M-1)
		to the fruit ID. (since the fruit ID may not start from 0 and 
		be continuous, we need this array for index conversion)

@ res[]: An array used to store the final result(the maximum fruit
		which can be eaten at same time). 

@ resPointer: an integer which is the pointer of the array res[]

@ currRes[]: An array used to store the current result during the 
		backtracking. We can copy the elements in the currRes[]
		to the res[] to update the search result. ( currRes[] stores
		the fruit we currently decide to eat)

@ currResPointer: an integer which is the pointer of the array
		currRes[].

@ cost: the final total cost for the maximum fruit.(those in the res[])

@ currCost: the current total cost for the current fruits. (those in
		the currRes[]).

@ M: the number of fruits in the basket.

@ N: the number of tips.

*************************************************************************/

int* tips[MAX];
int price[MAX], index[MAX], tipNum[MAX];
int res[MAX], currRes[MAX];
int resPointer = -1, currResPointer = -1;
int cost, currCost;
int M, N;

/*********************************************************************
						  Functions

@ initialize(): A function used to store the input tips and the prices
		of each fruit.

@ BackTracking(): Using backtracking to find the maximum fruits.

@ check(): check if the current fruit can be eaten with the fruits
		we have searched so far (those in the currRes[])

@ output(): Output the result in the required format.

@ quickSort(): Using quicksort to sort the res[] into ascending order.

@ partition(): the partition function used in the quickSort().

@ swap(): swap the position of two values in an array.

*************************************************************************/


void initialize(void);
void BackTracking(int index);
bool check(int index);
void output(void);
void quickSort(int array[], int low, int high);
int partition(int array[], int low, int high);
void swap(int* a, int* b);

int main()
{
	scanf("%d %d", &N, &M);

	initialize();

	BackTracking(0);

	output();

	return 0;

}

void initialize(void)
{
	int i;
	int id1, id2;
	int id, money;

	for (i = 0; i < MAX; i++) // first initialize the tipNum[] to 0
		tipNum[MAX] = 0;

	for (i = 0; i < N; i++) {
		scanf("%d %d", &id1, &id2); // input 2 unsafe fruits id1 and id2
		tips[id1] = realloc(tips[id1], sizeof(int) * (tipNum[id1] + 1)); // increase the space of array tips[id1] by 1
		tips[id2] = realloc(tips[id2], sizeof(int) * (tipNum[id2] + 1)); // increase the space of array tips[id2] by 1	
		tips[id1][tipNum[id1]++] = id2; // store the tip id1 cannot be eaten with id2 to the both array:tips[id1] and tips[id2]
		tips[id2][tipNum[id2]++] = id1;
	}

	for (i = 0; i < M; i++) {
		scanf("%d %d", &id, &money);	
		price[id] = money; // store the price of each fruit(the index is ID)
		index[i] = id;	
		// the ID of each fruit is indexed from 0 t0 M-1 (since the fruit ID may not start from 0 and be continuous, we need this array for index conversion)
	}
}


void BackTracking(int k)
{
	int id, i;
	bool OK;

	// even if we search all the unsearched fruit, the number of fruits qualified is still less than those in the res[], pruning
	if (resPointer > currResPointer + M - k) 
		return;

	if (k == M) { // we have reached the leaf of the game tree
		if ((resPointer < currResPointer) || (resPointer == currResPointer && cost > currCost)) { // check if we need to update the res[] and cost
			for (i = 0; i <= currResPointer; i++)
				res[i] = currRes[i];
			resPointer = currResPointer;
			cost = currCost;
		}
		return;
	}
	OK = check(k); // check if kth fruit can be eaten with those in the currRes[] 
	id = index[k]; // get the id of the kth fruit
	if (OK) {	   // if it is safe, push it into currRes[], and search the next fruit
		currRes[++currResPointer] = id;
		currCost += price[id];	// update the price
		BackTracking(k + 1);
		currCost -= price[id];  // pop the kth fruit from currRes[](we don't eat it), and search the next fruit
		currResPointer--;
	}
	BackTracking(k + 1); 
}

bool check(int k)
{
	int id = index[k], i, j;
	bool flag = 0;

	for (i = 0; i <= currResPointer; i++) { // for each fruit in currRes[] (the fruit we currently decide to eat)
		for(j=0;j<tipNum[id]; j++)  // check all the fruits that cannot be eaten with kth fruit
			if (tips[id][j] == currRes[i]) { // the kth fruit cannot be eaten with the fruits in the currRes[]
				flag = 1;
				break;
			}
	}
	if (flag)
		return false;
	return true;

}

void output(void)
{
	int i;

	quickSort(res, 0, resPointer);

	printf("%d\n%03d", resPointer + 1, res[0]);
	for (i = 1; i <= resPointer; i++)
		printf(" %03d", res[i]);
	printf("\n%d\n", cost);
}

void quickSort(int array[], int low, int high)
{
	if (low < high) {
		int pi = partition(array, low, high);
		quickSort(array, low, pi - 1);
		quickSort(array, pi + 1, high);
	}
}

int partition(int array[], int low, int high)
{
	int pivot = array[high];  // always set the last element as the pivot.
	int i = low - 1, j;

	for (j = low; j <= high - 1; j++) { // low-i: smaller than pivot; i-j: bigger than pivot; j-high: to be examined 
		if (array[j] < pivot) {
			i++;
			swap(&array[i], &array[j]);
		}
	}
	swap(&array[i + 1], &array[high]);
	return (i + 1);
}

void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}