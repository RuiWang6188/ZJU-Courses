#include<stdio.h>
#include<time.h>
clock_t start, stop;					// record the time when a function start/stop
double duration;						// record the run time (seconds) of a function

void testAlgorithm1(double X, double N);
void testAlgorithm2_rec(double X, double N);
void testAlgorithm2_ite(double X, double N);
double Algo_1(double x, int n);
double Algo_2_rec(double x, int n);
double Algo_2_ite(double x, int n);

int main(void)
{
	const double X = 1.0001;		//X is the base number (1.0001 according to the instruction)
	int N = 0;						// N is the exponent, which can be an arbitrary value
	

	printf("Please input the value of N：");
	scanf("%d", &N);
	printf("\n");

	testAlgorithm1(X, N);
	testAlgorithm2_ite(X, N);
	testAlgorithm2_rec(X, N);

	//  TEST PART
	//double k = Algo_1(X, N);
	//double l = Algo_2_rec(X, N);
	//double m = Algo_2_ite(X, N);
	//printf("%f\n%f\n%f\n", k,l,m);

	getchar();
	getchar();

	return 0;

}

double Algo_1(double x, int n) {		// Algorithm 1 is to use N−1 multiplications.
	double result = x;
	for (int i = 1; i < n; i++) {
		result *= x;					// N−1 multiplications
	}
	return result;
}

double Algo_2_rec(double x, int n) {	//the recursive version, Figure 2.11 in the textbook
	if (n == 0)
		return  1;
	if (n == 1)
		return x;
	if (n % 2 == 0)						// n is odd
		return Algo_2_rec(x * x, n / 2);
	else								// n is even
		return Algo_2_rec(x * x, n / 2) * x;
}

double Algo_2_ite(double x, int n) {	// the iterative version
	double result = 1;
	while (n > 0) {
		if (n % 2 == 1) {
			result *= x;
		}
		n = n / 2;						// reduce n to half of its original value
		x = x * x;						// each x to its original square.
	}
	return result;
}

void testAlgorithm1(double X, double N)
{
	{
		int k = 0;						//iteration nmber
		printf("Please input the iteration number of Algorithm1:");
		scanf("%d", &k);
		start = clock();					// start timing
		while (k--)
			Algo_1(X, N);
		stop = clock();
		// stop timing
		duration = ((double)(stop - start)) / CLK_TCK;
		printf("The duration of Algorithm1 (k times) is %f s\n\n", duration);
	}
}

void testAlgorithm2_ite(double X, double N)
{
	{
		int k = 0;						//iteration nmber
		printf("Please input the iteration number of Algorithm2 (iterative version):");
		scanf("%d", &k);
		start = clock();					// start timing
		while (k--)
			Algo_2_ite(X, N);
		stop = clock();
		// stop timing
		duration = ((double)(stop - start)) / CLK_TCK;
		printf("The duration of Algorithm2_ite (k times) is %f s\n\n", duration);
	}
}

void testAlgorithm2_rec(double X, double N)
{
	{
		int k = 0;						//iteration nmber
		printf("Please input the iteration number of Algorithm2 (recursive version):");
		scanf("%d", &k);
		start = clock();					// start timing
		while (k--)
			Algo_2_rec(X, N);
		stop = clock();
		// stop timing
		duration = ((double)(stop - start)) / CLK_TCK;
		printf("The duration of Algorithm2_rec (k times) is %f s\n\n", duration);
	}
}

