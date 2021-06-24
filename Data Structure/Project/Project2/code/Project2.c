#include <stdio.h>
#include <stdlib.h>

typedef enum { false, true } bool;
#define INFINITY 1000000
#define MaxVertexNum 501  /* maximum number of vertices */
typedef int Vertex;      /* stations are numbered from 1 to MaxVertexNum */
typedef int WeightType;
typedef struct GNode* PtrToGNode;
typedef PtrToGNode MGraph;

/******************************************************
* The data structure of the Graph
* Nv:      the number of vertices of the graph(that is the number of stations puls the PBMC)
* Ne:      the number of edges of the graph
* bike[]:  an array to store the initial number of bikes of each station
* G[][]:   the adjacency matrix that stores the time needed to travel from one station to another 
*******************************************************/
struct GNode {
	int Nv;
	int Ne;
	int bike[MaxVertexNum];
	WeightType G[MaxVertexNum][MaxVertexNum];
};

/******************************************************
* Global variable 
* Capacity:   the mamximum capacity of each station
* N:          the total number of stations
* Sp:         the index of the problem station
* M:          the number of roads(edges)
* minNeed:    the the minimum number of bikes needed from the PBMC
* minBack:    the minimum number of bikes which will be sent back to the PBMC after adjustion
* resultPath: a stack which stores the result path (in reverse order), and the resultPointer is its stack pointer
* tempPath:   a stack which stores the temporary path (in reverse order), and the tempPointer is its stack pointer
******************************************************/
int Capacity, N, Sp, M;
int minNeed = INFINITY, minBack = INFINITY;
Vertex resultPath[MaxVertexNum], tempPath[MaxVertexNum];
int resultPointer = -1, tempPointer = -1;

/******************************************************
* Functions Specification
* ReadG():       read the graph in 
* Dijkstra():    using Dijkstra's Algorithm to find all the shorteset paths, and store all the shortest paths in the adjacency list path[], with size in the array size[] 
* search():      using DFS to search the qualified shortest path
* printResult(): output the result
* adjust():      store the temprary result path in the resultPath(update resultPath)
* push():        the basic stack operations needed for the search function
* pop():         the basic stack operations needed for the search function
********************************************************/
MGraph ReadG();				 
void Dijkstra(MGraph Graph, Vertex* path[], int* size);				
void search(MGraph Graph, Vertex* path[], int* size, Vertex S);		
void printResult(void);												 
void adjust(Vertex* resultPath, Vertex* tempPath);					
void push(Vertex* stack, Vertex V);									
void pop(Vertex* stack);

int main()
{
	int size[MaxVertexNum] = { 0 };			//store the number of last nodes
	Vertex* path[MaxVertexNum] = { NULL };	//store all the last nodes of the vertex£¬since the path is not unique	

	MGraph Graph = ReadG();					//read in the graph

	Dijkstra(Graph, path, size);			//find all the shotest paths and store them in tha array "path"

	search(Graph, path, size, Sp);			//find the path that qualify the definition of "shortest"
	
	printResult();							//output the result in the requested form

	return 0;
}

MGraph ReadG()
{
	int i, j, k;
	MGraph Graph = (MGraph)malloc(sizeof(struct GNode));
	WeightType Cij;
	scanf("%d %d %d %d", &Capacity, &N, &Sp, &M);
	Graph->Nv = N + 1, Graph->Ne = M;			//the number of vertices if the number of station plus the PBMC

	for (i = 1; i < Graph->Nv; i++)				// read the number of bikes for each station and initial the number of bikes of PBMC to INFINITY
		scanf("%d", &Graph->bike[i]);
	Graph->bike[0] = INFINITY;

	for (i = 0; i < Graph->Nv; i++)				//initialize the time taken from one station to another 
		for (j = 0; j < Graph->Nv; j++) {
			if (i == j)							//the time taken from one station to itself is zero
				Graph->G[i][j] = 0;
			else
				Graph->G[i][j] = INFINITY;		//otherwise initialize it to INFINITY

		}

	for (k = 0; k < Graph->Ne; k++) {			//read the time taken to move between station i and j
		scanf("%d %d %d", &i, &j, &Cij);
		Graph->G[i][j] = Graph->G[j][i] = Cij;
	}

	return Graph;
}

void Dijkstra(MGraph Graph, Vertex* path[], int* size)
{
	bool* isKnown = (bool*)calloc(Graph->Nv, sizeof(bool));		//an auxiliary array that helps to record if a station is known
	WeightType* dist = (WeightType*)calloc(Graph->Nv, sizeof(WeightType));  //an auxiliary array that helps to find the minimum unknown vertex
	int minUnknownVertex, minUnknownLen, i, flag;

	for (i = 0; i < Graph->Nv; i++)
		dist[i] = INFINITY;
	dist[0] = 0;

	while (1) {										// find the minimum unknown vertex
		minUnknownLen = INFINITY;
		minUnknownVertex = -1;
		flag = 0;
		for (i = 0; i < Graph->Nv; i++) {
			if (!isKnown[i] && dist[i] < minUnknownLen) {		//if a vertex is still unknown and the distance to PBMC is less than the current minUnknownLen, upudate all the variables and set the flag that we have found one vertex(but may not be the minUnknownVertex) 
				flag = 1;
				minUnknownLen = dist[i];
				minUnknownVertex = i;
			}
		}
		if (!flag)			//all the vertices are known
			break;

		isKnown[minUnknownVertex] = true;
		for (i = 0; i < Graph->Nv; i++) {
			int len = Graph->G[minUnknownVertex][i];
			if (len == INFINITY)											//i is not adjacent to the minUnknownVertex
				continue;
			else if (len < INFINITY) {
				if (!isKnown[i]) {
					if (dist[minUnknownVertex] + len < dist[i]) {			//the shortest path is newly found, set the size[i] to 1 and put the last node(minUnknownVertex) in the path[i]
						dist[i] = dist[minUnknownVertex] + len;
						size[i] = 1;
						path[i] = realloc(path[i], size[i] * sizeof(Vertex));	
						path[i][size[i] - 1] = minUnknownVertex;
					}
					else if (dist[minUnknownVertex] + len == dist[i]) {		//find another path with same distance, increment size[i] and append the last node to the path[i] 
						path[i] = realloc(path[i], (++size[i]) * sizeof(Vertex));
						path[i][size[i] - 1] = minUnknownVertex;
					}
				}
			}
		}
	}
	for (i = 0; i < Graph->Nv; i++)
		dist[i] = (dist[i] == INFINITY ? -1 : dist[i]);

	free(isKnown);
	free(dist);
}

void search(MGraph Graph, Vertex* path[], int* size, Vertex V)
{
	int i;
	push(tempPath, V);
	if (V == 0) {							//get to the search end
		int tempNeed = 0, tempBack = 0;
		Vertex S;
		WeightType bikeDiff;
		for (i = tempPointer - 1; i >= 0; i--) {
			S = tempPath[i];
			bikeDiff = Graph->bike[S] - Capacity / 2;
			if (bikeDiff > 0)			//more than perfect, send back the margin
				tempBack += bikeDiff;
			else {
				if (tempBack > (-1 * bikeDiff))	//if the bikes needed to send back is more than the current station's needs, then reduce the send back
					tempBack += bikeDiff;
				else {
					tempNeed += (-1 * bikeDiff) - tempBack;	//all the temprary back serve as the need for this station, and set tempBack to zero
					tempBack = 0;
				}
			}
		}
		if (tempNeed < minNeed) {						//update the resultPath
			minNeed = tempNeed;
			minBack = tempBack;
			adjust(resultPath, tempPath);
		}
		else if (tempNeed == minNeed && tempBack < minBack) {
			minBack = tempBack;
			adjust(resultPath, tempPath);
		}
		pop(tempPath);
		return;
	}
	for (i = 0; i < size[V]; i++)						//search along the path
		search(Graph, path, size, path[V][i]);			
	pop(tempPath);
}

void printResult(void)
{
	int i = 0;
	printf("%d 0", minNeed);	
	for (i = resultPointer - 1; i >= 0; i--)
		printf("->%d", resultPath[i]);
	printf(" %d\n", minBack);
}


void push(Vertex* stack, Vertex V)				//push the stack of the caller
{
	if (stack == resultPath)
		resultPath[++resultPointer] = V;
	else tempPath[++tempPointer] = V;
}

void pop(Vertex* stack)                       //pop the stack of the caller
{
	if (stack == resultPath)
		resultPointer--;
	else tempPointer--;
}

void adjust(Vertex* resultPath, Vertex* tempPath)			//copy the tempPath to the resultPath
{
	int i;
	for (i = 0; i <= tempPointer; i++)						//just store all the elements in the tempPath to resultPath 
		resultPath[i] = tempPath[i];
	resultPointer = tempPointer;
}





