#include <time.h>
#include <stdio.h>
#include <stdlib.h>

clock_t start, stop; //clock_t是处理器时间(计时)的内置类型
double duration;  //记录函数的运行时间(秒)


double Algorithm_1(double x, int n)//算法1
{
	double result = 1;
	for (int i = 0; i < n; i++)
	{
		result *= x;
	};
	return result;
};

double Algorithm_2_r(double x, int n)//算法2(递归)
{
	double result = 1;

	if (n == 1)//递归跳出点
		return x;
	else//递归主体
	{
		if ((int)n % 2 == 0)
			return Algorithm_2_r(x, n / 2) * Algorithm_2_r(x, n / 2);
		else
			return Algorithm_2_r(x, (n - 1) / 2) * Algorithm_2_r(x, (n - 1) / 2) * x;
	};
};

double Algorithm_2_i(double x, int n)//算法2(迭代)
{
	double result = 1;
	if (n == 0)
		return result;
	while (n != 0)
	{
		if ((n & 1) == 1)//n的二进制表达该位是否为1
			result *= x;
		x *= x;
		n >>= 1;//位运算：将n的二进制表达右移1位
	}
	return result;
};

void run(int ch)
{
	double x, n, num = 0, time, UnitTime, ticks;
	printf("Input X(变量), N(指数), Time(运行次数)\n");
	scanf("%lf %lf %lf", &x, &n, &time);

	//选择运行3个算法并输出
	if (ch == 1)
	{
		//run Algorithm 1 
		start = clock();//计时器1
		for (int i = 0; i < time; i++)//运行Time次
		{
			num = Algorithm_1(x, n);
		};
		stop = clock();
		duration = ((double)(stop - start)) / CLK_TCK;
		UnitTime = duration / time;
		printf("算法一答案为： %f\n", num);
		printf("     总运行时间： %lf\n", duration);
		printf("     平均单次运行时间： %lf\n\n", UnitTime);

	}
	else if (ch == 2)
	{
		//run Algorithm 2(迭代)
		start = clock();//计时器1
		for (int i = 0; i < time; i++)//运行Time次
		{
			num = Algorithm_2_i(x, n);
		};
		stop = clock();
		duration = ((double)(stop - start)) / CLK_TCK;
		UnitTime = duration / time;
		printf("算法二(迭代)答案为： %f\n", num);
		printf("          总运行时间： %lf\n", duration);
		printf("          平均单次运行时间： %lf\n\n", UnitTime);
	}
	else
	{
		//run Algorithm 2(递归)
		start = clock();//计时器2
		for (int i = 0; i < time; i++)//运行Time次
		{
			num = Algorithm_2_r(x, n);
		};
		stop = clock();
		duration = ((double)(stop - start)) / CLK_TCK;
		UnitTime = duration / time;
		printf("算法二(递归)答案为： %f\n", num);
		printf("          总运行时间： %lf\n", duration);
		printf("          平均单次运行时间： %lf\n\n", UnitTime);
	};
	printf("Ticks = %f\n", duration * CLK_TCK);
};

int main()
{
	int next = 0;
	while (next == 0)
	{
		int ch;
		printf("------1: 算法一  2:算法二(迭代)  3:算法二(递归)------\n");
		scanf("%d", &ch);
		run(ch);
		printf("0: Run Again   1: Exit\n");
		scanf("%d", &next);
	};
};