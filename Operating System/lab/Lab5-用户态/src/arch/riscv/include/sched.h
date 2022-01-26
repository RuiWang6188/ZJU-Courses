#pragma once
#ifndef _SCHED_H
#define _SCHED_H
typedef uint64 pte_t;
typedef uint64* pagetable_t;
#define TASK_SIZE   (4096)
#define THREAD_OFFSET  (5 * 0x08)

#ifndef __ASSEMBLER__

/* task的最大数量 */
#define NR_TASKS    64

#define FIRST_TASK  (task[0])
#define LAST_TASK   (task[NR_TASKS-1])

/* 定义task的状态，Lab3中task只需要一种状态。*/
#define TASK_RUNNING                0
// #define TASK_INTERRUPTIBLE       1
// #define TASK_UNINTERRUPTIBLE     2
// #define TASK_ZOMBIE              3
// #define TASK_STOPPED             4

#define PREEMPT_ENABLE  0
#define PREEMPT_DISABLE 1

/* Lab3中进程的数量以及每个进程初始的时间片 */
#define LAB_TEST_NUM        4
#define LAB_TEST_COUNTER    5  

/* 当前进程 */
extern struct task_struct *current;

/*当前进程的上下文*/
extern struct thread_struct* current_context;

/* 进程指针数组 */
extern struct task_struct * task[NR_TASKS];


// mm struct
struct mm_struct {
    pagetable_t upgtbl;     //用户顶级根页表
    uint64 kstack;          // 用户内核栈 VA
    uint64 ustack;          //用户用户栈 PA
};


/* 进程状态段数据结构 */
struct thread_struct {
    unsigned long long ra;
    unsigned long long sp;
    unsigned long long s0;
    unsigned long long s1;
    unsigned long long s2;
    unsigned long long s3;
    unsigned long long s4;
    unsigned long long s5;
    unsigned long long s6;
    unsigned long long s7;
    unsigned long long s8;
    unsigned long long s9;
    unsigned long long s10;
    unsigned long long s11; 
	unsigned long long sepc; 
	unsigned long long sscratch; 
	unsigned long long sstatus;  
	unsigned long long satp;  
};



/* 进程数据结构 */
struct task_struct {
     long state;    // 进程状态 Lab3中进程初始化时置为TASK_RUNNING
     long counter;  // 运行剩余时间 
     long priority; // 运行优先级 1最高 5最低
     long blocked;
     long pid;      // 进程标识符
    // Above Size Cost: 40 bytes

     struct thread_struct thread; // 该进程状态段
     struct mm_struct* mm; 
};


/* 进程初始化 创建四个dead_loop进程 */ 
void task_init(void); 

/* 在时钟中断处理中被调用 */
void do_timer(void) ;
//void do_timer(void);

/* 调度程序 */
void schedule(void);

/* 切换当前任务current到下一个任务next */
void switch_to(struct task_struct* next);

void swtch_begin();

extern void __switch_to(struct task_struct* prev, struct task_struct* next);

/* 死循环 */
void dead_loop(void);

#endif

#endif
