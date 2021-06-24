#include <stdio.h>
#include <time.h>
#include <limits.h>

const int cnt[] = {1000, 5000, 10000, 20000, 40000, 60000, 80000, 100000}; // cnt 为供测试用的指数
const int cases = 8;                                                       // cnt 数组的大小
const double base = 1.0001;                                                // 供测试用的底数

double algorithm1(double x, int n);                                       // 算法1，暴力计算
double algorithm2_iterative(double x, int n);                             // 算法2，迭代版
double algorithm2_recursive(double x, int n);                             // 算法2，递归版
double get_runtime(double (*f)(double x, int n), double x, int n, int k); // 计算函数 f 运行 k 次的 tick 数

int main(void)
{
    int i, j;
    int k1, k2, k3;
    double ticks;
    for (i = 0; i < cases; i++)
    {
        printf("Please input the number of executions for algorithm 1:");
        scanf("%d", &k1);                                  // 读入运行次数 k1
        ticks = get_runtime(algorithm1, base, cnt[i], k1); // 测量时间
        printf("N:%d Ticks:%.0f Total Time(sec):%6f Duration:%f\n", cnt[i], ticks, ticks / CLK_TCK,
               ticks / k1); // 输出答案
        printf("Please input the number of executions for algorithm 2(iterative):");
        scanf("%d", &k2);                                            // 读入运行次数 k2
        ticks = get_runtime(algorithm2_iterative, base, cnt[i], k2); // 测量时间
        printf("N:%d Ticks:%.0f Total Time(sec):%6f Duration:%f\n", cnt[i], ticks, ticks / CLK_TCK,
               ticks / k2); // 输出答案
        printf("Please input the number of executions for algorithm 2(recursive):");
        scanf("%d", &k3);                                            // 读入运行次数 k2
        ticks = get_runtime(algorithm2_recursive, base, cnt[i], k3); // 测量时间
        printf("N:%d Ticks:%.0f Total Time(sec):%6f Duration:%f\n", cnt[i], ticks, ticks / CLK_TCK,
               ticks / k3); // 输出答案
    }
}

double algorithm1(double x, int n)
{
    int i;
    double res = x;
    for (i = 1; i < n; i++)
    {
        res *= x; // 执行 N-1 次乘法
    }
    return x;
}

double algorithm2_iterative(double x, int n)
{
    double res = 1;
    for (; n; n /= 2, x *= x) // 迭代，缩小问题规模
        if (n % 2)
            res = res * x; // 若 N 为奇数，则要将 X 乘给答案
    return res;
}

double algorithm2_recursive(double x, int n)
{

    if (n == 1)
        return x; // 递归出口
    if (n % 2 == 0)
        return algorithm2_recursive(x * x, n / 2); // 若 N 为偶数，则直接缩小问题规模
    else
        return algorithm2_recursive(x * x, n / 2) * x; // 若 N 为奇数，则要将 X 乘给答案
}

double get_runtime(double (*f)(double x, int n), double x, int n, int k) 
{                                           //接受函数指针 f，计算其执行 k 次的 tick 数
    clock_t start, stop;
    int i;
    start = clock(); // 记录起始时间
    for (i = 0; i < k; i++)
    {
        f(x, n);
    }
    stop = clock(); // 记录终止时间
    return stop - start;
}