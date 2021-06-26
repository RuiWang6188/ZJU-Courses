#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define MAX 100001
#define DIVISOR 1000000007

int main()
{
	int N, M;
	int i, j, input[MAX], dp[MAX];
	long count = 0;

	scanf("%d %d", &N, &M);
	for (i = 0; i < N; i++) {
		scanf("%d", &input[i]);
		dp[i] = 0; 								// initialize the dp[] array
	}

	for (i = 1; i < N; i++) {
		for (j = 0; j < i; j++) {
			if (abs(input[i] - input[j]) <= M) // case1: already valid
				dp[i] += 1<<j;
			else								// case2: otherwise
				dp[i] += dp[j];
		}
	}

	for (i = 0; i < N; i++) { 					// get the result
		count += dp[i];
		count %= DIVISOR;
	}

	printf("%ld\n", count);

	return 0;
}