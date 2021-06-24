#include <stdio.h>
#include <time.h>

 
clock_t start,stop;
double duration1,duration2,duration3;//define duration
double toltime1,toltime2,toltime3; //define toltime

//define Iterations
#define MAXK1 5e3
#define MAXK2 5e6
#define MAXK3 5e6

double f1(double x,int N);//this function uses Algorithm 1
double f2_iter(double x,int N);//this function uses iterative version of Algorithm 2
double f2_recur(double x,int N);//this function uses recursive version of Algorithm 2

int main(){	
	int N;
	double x;
	double result1,result2,result3;
	printf("Please input base: "); //explain of input
	scanf("%lf",&x);
	printf("Please input exponent: ");//explain of input
	scanf("%d",&N);
		start= clock();//time initialize
		for(int i=0;i<MAXK1;i++)// run MAXK1 times
		result1 = f1(x,N); //call function f1
		stop = clock();
		duration1 = ((double)(stop - start))/CLK_TCK/MAXK1; //compute durations
		toltime1 =  ((double)(stop - start))/CLK_TCK;
	printf("Algorithm 1 : \n"); // output initialize
	printf(" Iterations: %.0e\n",MAXK1);// ouput iterations
	printf(" ticks1 = %f\n",(double)(stop - start)); 
	printf(" tolal time = %5lf\n",toltime1); //output toltime
	printf(" duration1 = %6.2e\n",duration1);
		start= clock();//time initialize
		for(int i=0;i<MAXK2;i++)// run MAXK2 times
			result2 = f2_iter(x,N);	//call function f2
		stop = clock();
		duration2 = ((double)(stop - start))/CLK_TCK/MAXK2;//compute durations
		toltime2 =  ((double)(stop - start))/CLK_TCK;
	printf("Algorithm 2,iterative form : \n");// output initialize
	printf(" Iterations: %.0e\n",MAXK2);// ouput iterations
	printf(" ticks2 = %f\n",(double)(stop - start));
	printf(" tolal time = %5lf\n",toltime2);//output toltime
	printf(" duration2 = %6.2e\n",duration2);
	
		start= clock();//time initialize
		for(int i=0;i<MAXK3;i++)
			result3 = f2_recur(x,N);	
		stop = clock();
		duration3 = ((double)(stop - start))/CLK_TCK/MAXK3;
		toltime3 =  ((double)(stop - start))/CLK_TCK;
	printf("Algorithm 2,recursive form : \n");// output initialize
	printf(" Iterations: %.0e\n",MAXK3);
	printf(" ticks3 = %f\n",(double)(stop - start));
	printf(" tolal time = %5lf\n",toltime3);
	printf(" duration3 = %6.2e\n",duration3);
	return 0;
}

double f1(double x,int N){
 	 double result=1;
	for(int i=1;i<=N;i++){
		result = result * x;
	}
	return result;
} 


double f2_iter(double x,int N){
	double result = 1;
	for(int i = N;i!=0;i/=2){ //iterative
		if(i%2 !=0){
			result *=x;	
		}
		x*=x;
	}
	return result;
} 

double f2_recur(double x,int N){
	//recursive
	if(N==0)
		return 1;
	if(N==1)
		return x;
	if(N%2==1)
		return f2_recur(x*x,N/2)*x;
	else
		return f2_recur(x*x,N/2);
}

