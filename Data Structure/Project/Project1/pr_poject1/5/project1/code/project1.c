#include <stdlib.h>
#include <stdio.h>
#include <time.h>

clock_t start, stop;							//clock_t is a built-in type for processor time(ticks)
double total_time, duration; 					//records the run time(seconds) of a function

double mul(double x, int n)						//Implement Algorithm 1				
{
	int i;
	double ans = 1;
	for (i = 0; i< n; i++) ans*= x;				//Use N-1 multiplications to get X^N
	return ans;
}

double pow(double x, int n)						//Implement an recursive version of Algorithm 2
{
	if (n == 0) return 1;						//Boundary condition
	if (n == 1) return x;						//Boundary condition
	if (n%2 == 0)
		return pow(x*x, n/2);					//If N is even, X^N = X^(N/2)*X^(N/2)
	else
		return pow(x*x, n/2)*x;					//If N is odd, X^N = X^((N-1)/2)*X^((N-1)/2)*X
}

double ite(double x, int n)						//Implement an iterative version of Algorithm 2
{
	if (n == 0) return 1;						//Boundary condition
	int s[100], i = 0, m = n;
	double ans = x;
	while (m> 0)								//S is an array that stores the remainder of n successive division by 2 for final iteration
	{											
		s[i] = m;								
		m/= 2;
		i++;
	}
	i--;										
	while (i> 0)								//This is part of the iteration
	{
		if (s[i-1] == 2* s[i]){					//It's equal to 'N is even'. 
			ans = ans* ans;
		}
		else{ 									//It's equal to 'N is odd'. 
			ans = ans* ans* x;
		}
		i--; 									//With S, ans can go from X to X^N
	}
	return ans;
} 
 
int main()
{
	int n, K, i;
	double x, ans;		 
	printf("请输入底数、指数和运算次数K:\n"); 
	scanf("%lf %d %d", &x, &n, &K);				//Read in x, n, K according to the question
	
	start = clock();							//records the ticks at the beginning of the function call
	for (i = 0; i < K; i++)
	{				
		ans = mul(x, n);						//Implement Algorithm 1
	}						
	stop = clock();								//records the ticks at the end of the function call
	printf("\nAlgorithm 1:\n");
	printf("Ticks: %d\n", stop - start);
	total_time= ((double)(stop- start))/CLK_TCK;	//CLK_TCK is a built-in constant = ticks per second
	duration = total_time / K;
	printf("Total time: %.10lf\n",total_time);	
	printf("Duration: %.10lf\n",duration);			//Output run time    T(N) = O(N)	

	start = clock();							//Same as above
	for (i = 0; i < K; i++)
	{
		ans = pow(x, n);						//Implement an recursive version of Algorithm 2
	}
	stop = clock();								
	printf("\nAlgorithm 2(recursive):\n");
	total_time= ((double)(stop- start))/CLK_TCK;	
	duration = total_time / K;
	printf("Ticks: %d\n", stop - start);
	printf("Total time: %.10lf\n",total_time);	
	printf("Duration: %.10lf\n",duration);			//T(N) = O(log(N))

	start = clock();
	for (i = 0; i < K; i++)
	{	
		ans = ite(x, n);							//Implement an iterative version of Algorithm 2
	}
	stop = clock();
	printf("\nAlgorithm 2(iterative):\n");
	total_time= ((double)(stop- start))/CLK_TCK;	
	duration = total_time / K;
	printf("Ticks: %d\n", stop - start);
	printf("Total time: %.10lf\n",total_time);	
	printf("Duration: %.10lf\n",duration);			//T(N) = O(log(N))

	system("pause");
	return 0;
}
