#define _CRT_SECURE_NO_WARNINGS//avoid the warn of scanf
#include<stdio.h>
#include<time.h>
#include<math.h>

//#define Times 2100000//number of times(ticks) the function runs

clock_t start, stop;//clock_t is a built_in type for processor time
double duration;//records the run time(seconds) of a function
double result = 1;

double agorithm_1(double x, int n);
double agorithm_2_iterative(double x, int n);
double agorithm_2_recursion(double x, int n);
double runtime_calculation(double duration,unsigned long Times);//caculate the run time of one execution

int main(void)
{
	while(1){
		int i=0;//Cycle control variable
		double x;
		int n;
		unsigned long Times;
	  	//the same as X,N in the problem
	
		//scan data
		printf("Please enter the power first, then the index, separated by spaces in the middle.\n");
		scanf("%lf", &x);
		scanf("%d", &n);
		scanf("%d", &Times);
		//run agorithm_1
		start = clock();//records the ticks at the end of the function
		while (i < Times)//perform K times to ensure that running time is measurable
		{
			agorithm_1(x, n);
			result = 1;
			i++;
		}
		stop = clock();
		duration = ((double)(stop - start)) / CLK_TCK;//CLK_TCK is a built_in constant = ticks per second
		//print the run time of agorithm_1 and the result
		printf("one tick is : %d\n", CLK_TCK);
		printf("The total ticks : %d\n", stop-start);
		printf("The total running time is : %lf\n", duration); 
		printf("The running time of one execution of algorithm_1 is : %e\n", runtime_calculation(duration,Times));
		printf("The result of %f^%d is: %f\n\n", x, n, agorithm_1(x, n));
		
		//run agorithm_2_iterative
		i = 0;
		start = clock();//records the ticks at the end of the function
		while (i < Times)//perform 100 times to ensure that running time is measurable
		{
			agorithm_2_iterative(x, n);
			i++;
		}
		stop = clock();
		duration = ((double)(stop - start)) / CLK_TCK;//CLK_TCK is a built_in constant = ticks per second
		//print the run time of agorithm_1 and the result 
		printf("The total ticks : %d\n", stop-start);
		printf("The total running time is : %lf\n", duration); 
		printf("The running time of one execution of agorithm_2_iterative is : %e\n", runtime_calculation(duration,Times));
		printf("The result of %f^%d is: %f\n\n", x, n, agorithm_2_iterative(x, n));
	
		//run agorithm_2_recursion
		i = 0;
		start = clock();//records the ticks at the end of the function
		while (i < Times)//perform 100 times to ensure that running time is measurable
		{
			agorithm_2_recursion(x, n);
			i++;
		}
		stop = clock();
		duration = ((double)(stop - start)) / CLK_TCK;//CLK_TCK is a built_in constant = ticks per second
		//print the run time of agorithm_1 and the result
		printf("The total ticks : %d\n", stop-start);
		printf("The total running time is : %lf\n", duration);
		printf("The running time of one execution of agorithm_2_recursion is : %e\n", runtime_calculation(duration,Times));
		printf("The result of %f^%d is: %f\n", x, n, agorithm_2_recursion(x, n));		
	}
	return 0;
}

double agorithm_1(double x, int n)
{
	if(n == 0){
		return result;
	}
	else{
		for(;n>0;n--){
			result *= x; 
		}
	} 
	return result;
}

//Iterative solution 
double agorithm_2_iterative(double x, int n)
{
	int parity[32] = { 0 };//use to record the parity
	int i = 0;//index
	double result = 1;
	//record the parity
	while (n>0)
	{
		if (n % 2 == 0)
			parity[i] = 2;
		else
			parity[i] = 1;		
		i++;
		n /= 2;
	}
	i--;//move the one more count
	for (; i >= 0; i--)
	{
		if (parity[i] == 2)
		{
			result = result * result;
		}
		else if (parity[i] == 1)
		{
			result = result * result * x;
		}
	}
	return result;
}

double agorithm_2_recursion(double x, int n)
{
	if (n == 0)
	{
		return 1;
	}
	if (n == 1)
	{
		return x;
	}
	if (n % 2 == 0)
	{
		return agorithm_2_recursion(x*x, n / 2);
	}
	else
	{
		return agorithm_2_recursion(x*x, n / 2)*x;
	}
}

double runtime_calculation(double duration,unsigned long Times)//caculate the run time of one execution
{
	return duration / Times;//caculate the true result
}
