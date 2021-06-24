#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MaxCharInPlateNum 8		// the maximum number of characters in a plate number
#define MaxCharInStatus 4		// the maximum number of characters in the string status			

/**********************************************
	   Data Structure Used in This Code

* Record: Used to store the information of one
		  in/out record

* Period: Used to store the total stay time on
		  campus for each car

***********************************************/

typedef struct Record* PtrToRecord;
typedef struct Period* PtrToPeriod;

struct Record {								
	char plate_number[MaxCharInPlateNum];
	int status;
	int time;
};

struct Period {
	char plate_number[MaxCharInPlateNum];
	int period;
};

/************************************************
		Compare Functions Used in qsort

* cmp_plateNum: determines the sort according to the
		plate number of the car in alphabetical	order;
		if two records has the same plate number, then 
		sort them according to the ascending time order

* cmp_time: determine the sort according to the ascend-
		ing time order.

*************************************************/

int cmp_plateNum(const void* a, const void* b) {
	if (strcmp((*(PtrToRecord)a).plate_number, (*(PtrToRecord)b).plate_number) != 0) {
		return strcmp((*(PtrToRecord)a).plate_number, (*(PtrToRecord)b).plate_number);
	}
	else {
		return (*(PtrToRecord)a).time - (*(PtrToRecord)b).time;
	}
}

int cmp_time(const void* a, const void* b) {
	return (*(PtrToRecord)a).time - (*(PtrToRecord)b).time;
}

/*********************************************************
				Functions

* Read: Read in all the information of the input 
		records

* Adjust: delete all the invalid records(those single
		in or out)

* FindLongestParkingTime: Find the longest parking time
		on campus during that day.

* CalculateCount: Calculate the count array

* FindParkingCar: Using the count array to respond to
		the querise.

* FindLongestParkingCar: Search Through the Periods and 
		output the plate number of those longest stay car
*********************************************************/

PtrToRecord Read(int N);
PtrToRecord Adjust(int N, PtrToRecord Records, int* FinalRecordsPointer);
PtrToPeriod FindLongestParkingTime(int N, int FinalRecordsPointer, int* PeriodsPointer, PtrToRecord FinalRecords, int* longest);
int* CalculateCount(int FinalRecordsPointer, PtrToRecord FinalRecords);
void FindParkingCar(int K, int* count, int FinalRecordsPointer, PtrToRecord FinalRecords);
void FindLongestParkingCar(int longest, PtrToPeriod Periods, int PeriodsPointer);

int main()
{
/*************************************************************************
					Variables Used 

* Records: An structure array which stores all the input records. 

* FinalRecords: An structure array that stores the valid records.
			(Haved deleted the single in/out records)

* Periods: An structure array that stores the total stay time of
			each car.

* count: Since all the time in the records are unique, so we can
		sort the records according to the ascending time. So "count"
		is an array that calculate the number of cars on campus at 
		a specfic time. For example, count[i] represents the number 
		of cars on campus at the the ith smallest time in the ascending
		time sequence.

* FinalRecordsPointer: A pointer for the structure array "FinalRecords".

* PeriodsPointer: A pointer for the structure array "Periods".

* index: Since the queries are given in ascending order of the times. After
		we sort the final records in ascending time order, we can record
		the index of the last search and start the next search from this 
		index to avoid redundent search.

* longest: Records the longest parking time on campus that day

*************************************************************************/

	int N, K;		// N is the number of records, K is the number of queries
	int FinalRecordsPointer = -1, PeriodsPointer = -1;
	int longest = 0;  // record the longest stay time
	int* count = NULL;
	PtrToRecord Records = NULL, FinalRecords = NULL;
	PtrToPeriod Periods = NULL;

	scanf("%d %d", &N, &K);

	// a function that reads in all the records information
	Records = Read(N);   
	// sort the records according to the plate number
	qsort(Records, N, sizeof(Records[0]), cmp_plateNum);  
	// delete all the invalid records (single in/out), get the final records
	FinalRecords = Adjust(N, Records, &FinalRecordsPointer);
	// find the longest parking time on campus
	Periods = FindLongestParkingTime(N, FinalRecordsPointer,&PeriodsPointer, FinalRecords, &longest);
	// sort the final records according to the ascending time order
	qsort(FinalRecords, FinalRecordsPointer + 1, sizeof(struct Record), cmp_time);
	// calculate the count array
	count = CalculateCount(FinalRecordsPointer, FinalRecords);
	// using the count array to get the car number of the queried time
	FindParkingCar(K, count, FinalRecordsPointer, FinalRecords);
	// Search Through the Periods and output the plate number of those longest stay car
	FindLongestParkingCar(longest, Periods, PeriodsPointer);

	return 0;
}



PtrToRecord Read(int N)
{
	int i, j, FinalRecordsPointer = -1, PeriodsPointer = -1, index = 0;
	PtrToRecord Records = (PtrToRecord)malloc(N * sizeof(struct Record));  
	
	for (i = 0; i < N; i++) {
		char temp_status[4];
		int hh, mm, ss;
		scanf("%s %d:%d:%d %s\n", Records[i].plate_number, &hh, &mm, &ss, temp_status);		// read in the information
		Records[i].time = hh * 3600 + mm * 60 + ss;
		// if the status is "in", assign value 1, otherwise -1, this makes calculate the array "count" convenient
		Records[i].status = strcmp(temp_status, "in") == 0 ? 1 : -1;			
	}

	return Records; 
}

PtrToRecord Adjust(int N, PtrToRecord Records, int* FinalRecordsPointer)
{
	int i;
	PtrToRecord FinalRecords = (PtrToRecord)malloc(N * sizeof(struct Record));	
	
	for (i = 0; i < N - 1; i++) { 
		// since all the records with the same plate number are adjacent, the qualified records are those adjacent records with one in and one out of the same plate number  
		if (strcmp(Records[i].plate_number, Records[i + 1].plate_number) == 0 && Records[i].status == 1 && Records[i + 1].status == -1) {	
			FinalRecords[++(*FinalRecordsPointer)] = Records[i];
			FinalRecords[++(*FinalRecordsPointer)] = Records[i + 1];
		}
	}

	return  FinalRecords;
}

PtrToPeriod FindLongestParkingTime(int N, int FinalRecordsPointer, int* PeriodsPointer, PtrToRecord FinalRecords, int* longest)
{
	int i;
	PtrToPeriod Periods = (PtrToPeriod)malloc(N * sizeof(struct Period));	

	for (i = 0; i < FinalRecordsPointer; i++) {
		if (strcmp(FinalRecords[i].plate_number, FinalRecords[i + 1].plate_number) == 0 && FinalRecords[i].status == 1 && FinalRecords[i + 1].status == -1) {
			strcpy(Periods[++(*PeriodsPointer)].plate_number, FinalRecords[i].plate_number);   // we assume the plate number is not exist in the Periods
			Periods[(*PeriodsPointer)].period = FinalRecords[i + 1].time - FinalRecords[i].time;	 // calculate the stay time and store it

			if ((*PeriodsPointer) >= 1 && strcmp(Periods[(*PeriodsPointer)].plate_number, Periods[(*PeriodsPointer) - 1].plate_number) == 0) {   // the car plate number already exist in the Periods
				Periods[(*PeriodsPointer) - 1].period += Periods[(*PeriodsPointer)].period;  // add the current stay time to the previous stay time 
				(*PeriodsPointer)--;	// update the PeriodsPointer
			}

			if ((*longest) < Periods[(*PeriodsPointer)].period) {		// update the longest stay time
				(*longest) = Periods[(*PeriodsPointer)].period;
			}
		}
	}

	return Periods;
}

int* CalculateCount(int FinalRecordsPointer, PtrToRecord FinalRecords)
{
	int i;
	int* count = (int*)malloc(sizeof(int) * FinalRecordsPointer);

	/* the method to calculate the count array is tricky. Since we assigned "in" as 1 and "out" as -1,
		we can calculate the number of cars on campus by adding all the record status to a specific time.
		If one car get in and not get out, the sum would add 1, if the car get out, the sum would add 0.
	*/
	for (i = 0; i <= FinalRecordsPointer; i++) {
		if (i == 0) 
			count[i] = FinalRecords[i].status;
		else 
			count[i] = count[i - 1] + FinalRecords[i].status;
	}

	return count;
}

void FindParkingCar(int K, int* count, int FinalRecordsPointer, PtrToRecord FinalRecords)
{
	int index = 0, i, j;
	/*Since the queries are given in ascending order of the times. After
	  we sort the final records in ascending time order, we can record
	  the index of the last search and start the next search from this 
	  index to avoid redundent search.*/

	for (int i = 0; i < K; i++) {
		int hh, mm, ss;
		scanf("%d:%d:%d", &hh, &mm, &ss);
		int temptime = hh * 3600 + mm * 60 + ss;
		for (j = index; j <= FinalRecordsPointer; j++) {
			if (FinalRecords[j].time > temptime) {
				printf("%d\n", count[j - 1]);
				break;
			}
			else if (j == FinalRecordsPointer) {
				printf("%d\n", count[j]);
			}
		}
		index = j;
	}
}

void FindLongestParkingCar(int longest, PtrToPeriod Periods, int PeriodsPointer)
{
	int i;

	// Since the Periods has already in alphabetical order, we can search for the plate number in O(N).
	for (i = 0; i <= PeriodsPointer; i++) {		
		if (Periods[i].period == longest) {
			printf("%s ", Periods[i].plate_number);
		}
	}
	// output the plate number in specific format
	printf("%02d:%02d:%02d", longest / 3600, (longest % 3600) / 60, longest % 60);
}


