#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAX_WIDTH 1000  //the MAX Width of strip

typedef struct REC* Rectangle;
struct REC {
	int width;
	int height;
	int x;
	int y;
	Rectangle next;
};

int MinHeight = 0x7FFFFFFF;
void optimization(Rectangle packed, int skyline[], int WIDTH, int placementPolicy);
void output(int placementPolicy, Rectangle packed, int skyline[], int WIDTH);


int main()
{

	int WIDTH, N;
	int skyline[MAX_WIDTH];
	int placementPolicy = 0;
	int gapLocation, gapWidth;
	Rectangle input = NULL, packed = NULL, unpacked = NULL, p = NULL, t = NULL;
	scanf("%d %d", &N, &WIDTH);
	int begintime = clock();
	
	for (int i = 0; i < N; i++) {      // read in the rectangle and sort it by decreasing width
		int width, height;
		scanf("%d %d", &width, &height);
		if (width < height) {			// modify the width and height
			int temp = width;
			width = height;
			height = temp;
		}
		p = (Rectangle)malloc(sizeof(struct REC));
		p->width = width;
		p->height = height;
		p->x = p->y = 0;
		p->next = NULL;

		if (input == NULL) {                                       
			input = (Rectangle)malloc(sizeof(struct REC));
			input->next = p;
			input->height = input->width = 0;
			input->x = input->y = 0;
		}
		else {
			Rectangle t = input;
			while (t->next && p->width < t->next->width) {
				t = t->next;
			}
			if (t->next && p->width == t->next->width) {
				while (t->next && p->width == t->next->width && p->height < t->next->height)
					t = t->next;
			}
			p->next = t->next;
			t->next = p;
		}
	}

	for (placementPolicy = 0; placementPolicy < 3; placementPolicy++) {   // using 3 different placement policy
		gapLocation = -1;
		for (int i = 0; i < WIDTH; i++)
			skyline[i] = 0;
		packed = (Rectangle)malloc(sizeof(struct REC));
		packed->next = NULL;										// store the packde rectangle list
		unpacked = (Rectangle)malloc(sizeof(struct REC));
		t = unpacked;
		t->next = NULL;
		p = input->next;
		while (p) {													// copy the input rectangles list to unpacked list
			t->next = (Rectangle)malloc(sizeof(struct REC));
			t->next->width = p->width;
			t->next->height = p->height;
			t->next->x = p->x;
			t->next->y = p->y;
			t->next->next = NULL;
			t = t->next;
			p = p->next;
		}

		while (unpacked->next) {
			if (gapLocation == -1) {														// find the lowest gap position and width
				int min_skyline = 0x7FFFFFFF;
				for (int i = 0; i < WIDTH; i++)
					if (min_skyline > skyline[i]) {
						min_skyline = skyline[i];
						gapLocation = i;
					}
				gapWidth = 0;
				for (int i = gapLocation; i < WIDTH && skyline[i] == min_skyline; i++)
					gapWidth++;
			}

			Rectangle currentBest = NULL, currentBestPrev = NULL;
			p = unpacked;

			while (p->next) {
				if (p->next->width == gapWidth) { // best fit
					currentBest = p->next;
					currentBestPrev = p;
					break;
				}
				else if (p->next->width > gapWidth) {   // width cannot fit, check height
					if (p->next->height <= gapWidth && !currentBest) { //height fit
						currentBest = p->next;
						currentBestPrev = p;
					}
				}
				else if (p->next->width < gapWidth) {//width less, terminate
					if (!currentBest) {	//not found currentBest
						currentBest = p->next;
						currentBestPrev = p;
					}
					else if (p->next->width > currentBest->width) {
						currentBest = p->next;
						currentBestPrev = p;
					}
					break;
				}
				p = p->next;
			}
			if (!currentBest) { //not found best-fitting Rectangle, raise the gap to lowest neighbor
				int lowestNeighbor;
				if (gapLocation == 0)
					lowestNeighbor = gapLocation + gapWidth;
				else if (gapLocation + gapWidth == WIDTH)
					lowestNeighbor = gapLocation - 1;
				else {
					lowestNeighbor = (skyline[gapLocation - 1] < skyline[gapLocation + gapWidth]) ? (gapLocation - 1) : (gapLocation + gapWidth);
				}
				for (int i = gapLocation; i < gapLocation + gapWidth; i++)
					skyline[i] = skyline[lowestNeighbor];

				gapLocation = -1;
			}
			else {	//found Best-Fitting Rectangle
				if (currentBest->width > gapWidth) { //adjust the width and height
					int temp = currentBest->width;
					currentBest->width = currentBest->height;
					currentBest->height = temp;
				}

				if (placementPolicy == 0) {   //place at left-most
					currentBestPrev->next = currentBest->next;
					currentBest->x = gapLocation;
					currentBest->y = skyline[gapLocation];
					t = packed->next;
					packed->next = currentBest;
					currentBest->next = t;

					for (int i = gapLocation; i < gapLocation + currentBest->width; i++)
						skyline[i] += currentBest->height;
					gapLocation += currentBest->width;
				}
				else if (placementPolicy == 1) { // place next to Tallest Neighbor
					int tallest = -1;
					if (gapLocation == 0)
						tallest = 0;
					else if (gapLocation + gapWidth == WIDTH)
						tallest = WIDTH - currentBest->width;
					else
						tallest = (skyline[gapLocation - 1] > skyline[gapLocation + gapWidth]) ? (gapLocation) : (gapLocation + gapWidth - currentBest->width);

					currentBestPrev->next = currentBest->next;
					currentBest->x = tallest;
					currentBest->y = skyline[gapLocation];
					t = packed->next;
					packed->next = currentBest;
					currentBest->next = t;

					for (int i = currentBest->x; i < currentBest->x + currentBest->width; i++)
						skyline[i] += currentBest->height;

					if (tallest == gapLocation)  //left
						gapLocation += currentBest->width;
				}
				else { // place next to Shortest Neighbor
					int shortest = -1;
					if (gapLocation == 0)
						shortest = gapWidth - currentBest->width;
					else if (gapLocation + gapWidth == WIDTH)
						shortest = gapLocation;
					else
						shortest = (skyline[gapLocation - 1] > skyline[gapLocation + gapWidth]) ? (gapLocation + gapWidth - currentBest->width) : (gapLocation);

					currentBestPrev->next = currentBest->next;
					currentBest->x = shortest;
					currentBest->y = skyline[gapLocation];
					t = packed->next;
					packed->next = currentBest;
					currentBest->next = t;

					for (int i = currentBest->x; i < currentBest->x + currentBest->width; i++)
						skyline[i] += currentBest->height;

					if (shortest == gapLocation)  //left
						gapLocation += currentBest->width;
				}

				gapWidth -= currentBest->width;
				if (gapWidth == 0)
					gapLocation = -1;
			}

		}

		optimization(packed, skyline, WIDTH, placementPolicy);

		output(placementPolicy, packed, skyline, WIDTH);
		p = packed;
		while (p) {
			Rectangle tempRec = p->next;
			free(p);
			p = tempRec;
		}
	} //end function


	//p = input;
	//while (p->next) {
	//	printf("%d %d\n", p->next->width, p->next->height);
	//	p = p->next;
	//}
	printf("The minimum height of the strip using BF Algorithm is %d\n", MinHeight);


	int endtime = clock();
	printf("Running time:%.4fs\n", (double)(endtime - begintime) / 10000);

	system("pause");

	return 0;
}


void optimization(Rectangle packed, int skyline[], int WIDTH, int placementPolicy)    //modify the tower
{
	int optimizationFinished = 0;

	while (!optimizationFinished) {
		int* oldSkyline = (int*)malloc(sizeof(int) * (WIDTH));

		int highestSkyline = -1;								//find the tallest rectangle
		for (int i = 0; i < WIDTH; i++) {
			oldSkyline[i] = skyline[i];
			if (highestSkyline < skyline[i])
				highestSkyline = skyline[i];
		}
		Rectangle highestShape = NULL, p = NULL;
		 p = packed;

		 while (p->next) {
			 if (p->next->y + p->next->height == highestSkyline) {
				 highestShape = p->next;
				 break;
			 }
			 p = p->next;
		 }

		 if (highestShape->width >= highestShape->height){
			 optimizationFinished = 1;
			 break;
		 }

		 int oldX = highestShape->x, oldY = highestShape->y;

		 p->next = highestShape->next; //Remove Highest Shape
		 for (int i = highestShape->x; i < highestShape->x + highestShape->width; i++) {	//update skyline[]
			 skyline[i] -= highestShape->height;
		 }

		 int temp = highestShape->width;    //Rotate shape by 90 degrees
		 highestShape->width = highestShape->height;
		 highestShape->height = temp;

		 int gapLocation = -1, gapWidth = 0;
		 while (1) { //try to pack the highestShape
			 if (gapLocation == -1) {
				 int min_skyline = 0x7FFFFFFF;
				 for (int i = 0; i < WIDTH; i++)
					 if (min_skyline > skyline[i]) {
						 min_skyline = skyline[i];
						 gapLocation = i;
					 }
				 gapWidth = 0;
				 for (int i = gapLocation; i < WIDTH && skyline[i] == min_skyline; i++)
					 gapWidth++;
			 }

			 if (gapWidth < highestShape->width) { //Raise gap to lowest Neighbor
				 int lowestNeighbor;
				 if (gapLocation == 0)
					 lowestNeighbor = gapLocation + gapWidth;
				 else if (gapLocation + gapWidth == WIDTH)
					 lowestNeighbor = gapLocation - 1;
				 else {
					 lowestNeighbor = (skyline[gapLocation - 1] < skyline[gapLocation + gapWidth]) ? (gapLocation - 1) : (gapLocation + gapWidth);
				 }
				 for (int i = gapLocation; i < gapLocation + gapWidth; i++)
					 skyline[i] = skyline[lowestNeighbor];

				 gapLocation = -1;
				 continue;
			 }
			 else { //Shape fits
				 if (placementPolicy == 0) { //place at left-most
					 highestShape->x = gapLocation;
					 highestShape->y = skyline[gapLocation];
					 Rectangle t = packed->next;
					 packed->next = highestShape;
					 highestShape->next = t;

					 for (int i = gapLocation; i < gapLocation + highestShape->width; i++)
						 skyline[i] += highestShape->height;
					 gapLocation += highestShape->width;
				 }
				 else if (placementPolicy == 1) { //place next to Tallest Neighbor
					 int tallest = -1;
					 if (gapLocation == 0)
						 tallest = 0;
					 else if (gapLocation + gapWidth == WIDTH)
						 tallest = WIDTH - highestShape->width;
					 else
						 tallest = (skyline[gapLocation - 1] > skyline[gapLocation + gapWidth]) ? (gapLocation) : (gapLocation + gapWidth - highestShape->width);
					 
					 highestShape->x = tallest;
					 highestShape->y = skyline[tallest];
					 Rectangle t = packed->next;
					 packed->next = highestShape;
					 highestShape->next = t;

					 for (int i = highestShape->x; i < highestShape->x + highestShape->width; i++)
						 skyline[i] += highestShape->height;

					 if (tallest == gapLocation)  //left
						 gapLocation += highestShape->width;
				 }
				 else { // place next to Shortest Neighbor
					 int shortest = -1;
					 if (gapLocation == 0)
						 shortest = gapWidth - highestShape->width;
					 else if (gapLocation + gapWidth == WIDTH)
						 shortest = gapLocation;
					 else
						 shortest = (skyline[gapLocation - 1] > skyline[gapLocation + gapWidth]) ? (gapLocation + gapWidth - highestShape->width) : (gapLocation);

					 highestShape->x = shortest;
					 highestShape->y = skyline[shortest];
					 Rectangle t = packed->next;
					 packed->next = highestShape;
					 highestShape->next = t;

					 for (int i = highestShape->x; i < highestShape->x + highestShape->width; i++)
						 skyline[i] += highestShape->height;

					 if (shortest == gapLocation)  //left
						 gapLocation += highestShape->width;
				 }
				 gapWidth -= highestShape->width;
				 if (gapWidth == 0)
					 gapLocation = -1;
				 break;
			 }
		 }

		 int newHighestSkyline = -1;
		 for (int i = 0; i < WIDTH; i++)
			 if (skyline[i] > newHighestSkyline)
				 newHighestSkyline = skyline[i];

		 if (newHighestSkyline <= highestSkyline) {
			 temp = highestShape->width;    //Rotate shape by 90 degrees back
			 highestShape->width = highestShape->height;
			 highestShape->height = temp;

			 highestShape->x = oldX;
			 highestShape->y = oldY;

			 for (int i = 0; i < WIDTH; i++)
				 skyline[i] = oldSkyline[i];
			 free(oldSkyline);
			 optimizationFinished = 1;
			 break;
		 }
	}
}

void output(int placementPolicy, Rectangle packed, int skyline[], int WIDTH)
{
	if (placementPolicy == 0)
		printf("Left-most placement policy:\n");
	else if (placementPolicy == 1)
		printf("Tallest Neighbor placement policy:\n");
	else
		printf("Shortest Neighbor placement policy:\n");

	int minimumHeight = -1;
	for (int i = 0; i < WIDTH; i++)
		if (minimumHeight < skyline[i])
			minimumHeight = skyline[i];
	if (minimumHeight < MinHeight)
		MinHeight = minimumHeight;
	printf("The minimum height:%d\n", minimumHeight);
	printf("X  Y  height  width\n");
	Rectangle p = packed->next;

	while (p) {
		printf("%d  %d  %d  %d\n", p->x, p->y, p->height, p->width);
		p = p->next;
	}
	printf("\n");
}

