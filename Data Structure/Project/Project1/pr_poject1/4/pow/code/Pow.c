#include <stdio.h>
#include <time.h>
clock_t start, stop;	//clock_t is a built-in type for processor time(ticks）
double duration;	//records the run time (seconds)of a function
double Algo_1(double x, int n);
double Algo_2(double x, int n);
double Algo_3(double x, int n);
double Pow(double x, int n);

int main() {
	double x,ans;
	int n,mode,i;
    int MAXK;
    while(printf("please input testing mode(1/2/3/other for exit):"),~scanf("%d",&mode)){
        if(mode!=1 && mode!=2 && mode!=3) break;

        printf("please input the X(base number):");
        scanf("%lf", &x);						//get the base of the power
        printf("please input the N(index number):");
        scanf("%d", &n);						//get the oder of the power
        printf("please input the K(iteration times):");
        scanf("%d",&MAXK);
        printf("the answer is:");
        start = clock();
        switch(mode){
            case 1: {
                for(i = 0;i < MAXK;++i) ans = Algo_1(x,n);  //excute MAXK times Algorithm 1
                break;
            }
            case 2: {
                for(i = 0;i < MAXK;++i) ans = Algo_2(x,n); //excute MAXK times Algorithm 2
                break;
            }
            case 3: {
                for(i = 0;i < MAXK;++i) ans = Algo_3(x,n); //excute MAXK times Algorithm 3
                break;
            }
        }
        printf("%lf\n",ans);
        stop = clock();//records the ticks at the end of the function call
        duration = ((double)(stop - start)) / CLK_TCK / MAXK;//CLK_TCK is a built-in constant = ticks per second
        printf("ticks = %f\n", (double)(stop - start));
        printf("total_time = %f\n", duration * MAXK);
        printf("duration = %6.2e\n", duration);

    }
	
}

// ALGORITHM 1: use N−1 multiplications
double Algo_1(double x, int n) {	
	int i = 1;
	double ans;
	if (n == 0)ans = 1;
	else {
		ans = x;
		for (i = 1; i < n; i++) {
			ans = ans * x;
		}
	}
	return ans;
}

// ALGORITHM 2:interative version
//get the array connected with n,n/2,n/4……then multiply the arry elements in reverse order
double Algo_2(double x, int n) {		
	int a[20];
	double b[20];
	int i = 0;
	int m = n;
	if (n == 0)b[0] = 1;		//b[0] will be the final output,so if n=0 the answer will always be 1
	else {
		while (m != 1) {
			a[i] = m;			//a[i] stores n,n/2,n/2/2……
			m = m / 2;
			i++;
		}
		a[i] = m;
		b[i] = x;
		i--;
		while (i >= 0) {
			if (a[i] == 2 * a[i + 1]) {				//if so, a[i+1] is odd, then b[i] is the square of b[i+1]
				b[i] = b[i + 1] * b[i + 1];
			}
			else {									//else,a[i+1] is even
				b[i] = x * b[i + 1] * b[i + 1];
			}
			i--;
		}
	}
	return b[0];
}

// ALGORITHM 3: recursive version
double Algo_3(double x, int n) {	
	return Pow(x,n);
}

//recursive function
//distinguish the parity of n and look for its relation to the previous term
double Pow(double x, int n) {
	if (n == 1)return x;
	else if (n == 0)return 1;
	else {
		if (n % 2 == 1)return Pow(x*x,n / 2)*x;		//if n is even,x^n=(x^2)^(n/2)*x
		else return Pow(x*x, n / 2);				//if n is odd,x^n=(x^2)^(n/2)
	}
}