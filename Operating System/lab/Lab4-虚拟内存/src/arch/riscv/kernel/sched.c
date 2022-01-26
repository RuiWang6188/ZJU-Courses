#include "sched.h"
#include "vm.h"
#include "../../../include/rand.h"
#include "put.h"

uint64 space_offset = (uint64)HIGHBASE - (uint64)KERNBASE;

/* 当前进程 */
struct task_struct *current;

/* 进程指针数组 */
struct task_struct * task[NR_TASKS];


/* 将dead_loop赋值给sepc寄存器 */
void sepc_assign(void)		
{
	unsigned long long temp = dead_loop;
	asm volatile("csrw sepc, %0" : : "r"(temp));
	asm volatile("sret": :);
}


/* 进程初始化 创建四个dead_loop进程 */ 
void task_init(void)
{
	int i = 0;
	//printf("task init...\n");
	puts("task init...\n");
	//1. initialize current & task[0]
	// 1.1 set current point to Task0 Space base address
	current = (uint64)0x80010000 + space_offset;

	// 1.2 set task[0] = current
	task[0] = current;

	// 1.3 initialize member variables in task[0] 
	task[0]->state = TASK_RUNNING;
	task[0]->counter = 0;
	task[0]->priority = 5;
	task[0]->blocked = 0;
	task[0]->pid = 0;

	// 1.4 set task[0]->thread.sp = Task0 Space base address + 4KB offset
	task[0]->thread.sp = task[0] + TASK_SIZE;

	#ifdef SJF
	//2. initialize task[1-4]
	// 2.1 intialize task[1]
	task[1] = (uint64)0x80011000 + space_offset;
	task[1]->state = TASK_RUNNING;
	task[1]->counter = rand();
	task[1]->priority = 5;
	task[1]->blocked = 0;
	task[1]->pid = 1;

	task[1]->thread.ra = sepc_assign;
	task[1]->thread.sp = task[1] + TASK_SIZE;


	// 2.2 intialize task[2]
	task[2] = (uint64)0x80012000 + space_offset;
	task[2]->state = TASK_RUNNING;
	task[2]->counter = rand();
	task[2]->priority = 5;
	task[2]->blocked = 0;
	task[2]->pid = 2;

	task[2]->thread.ra = sepc_assign;
	task[2]->thread.sp = task[2] + TASK_SIZE;

	// 2.3 intialize task[3]
	task[3] = (uint64)0x80013000 + space_offset;
	task[3]->state = TASK_RUNNING;
	task[3]->counter = rand();
	task[3]->priority = 5;
	task[3]->blocked = 0;
	task[3]->pid = 3;

	task[3]->thread.ra = sepc_assign;
	task[3]->thread.sp = task[3] + TASK_SIZE;

	// 2.4 intialize task[4]
	task[4] = (uint64)0x80014000 + space_offset;
	task[4]->state = TASK_RUNNING;
	task[4]->counter = rand();
	task[4]->priority = 5;
	task[4]->blocked = 0;
	task[4]->pid = 4;

	task[4]->thread.ra = sepc_assign;
	task[4]->thread.sp = task[4] + TASK_SIZE;

	for(i = 1; i < 5; i++){
		//printf("[PID = %d] Process Create Successfully! counter = %d\n", i, task[i]->counter);
		puts("[PID = ");
		puti(i);
		puts("] Process Create Successfully! counter = ");
		puti(task[i]->counter);
		puts("\n");
	}
	

	#else    //define PRIORITY
	//2. initialize task[1-4]
	// 2.1 intialize task[1]
	task[1] = (uint64)0x80011000 + space_offset;
	task[1]->state = TASK_RUNNING;
	task[1]->counter = 7;
	task[1]->priority = 5;
	task[1]->blocked = 0;
	task[1]->pid = 1;

	task[1]->thread.ra = sepc_assign;
	task[1]->thread.sp = task[1] + TASK_SIZE;

	// 2.2 intialize task[2]
	task[2] = (uint64)0x80012000 + space_offset;
	task[2]->state = TASK_RUNNING;
	task[2]->counter = 6;
	task[2]->priority = 5;
	task[2]->blocked = 0;
	task[2]->pid = 2;

	task[2]->thread.ra = sepc_assign;
	task[2]->thread.sp = task[2] + TASK_SIZE;

	// 2.3 intialize task[3]
	task[3] = (uint64)0x80013000 + space_offset;
	task[3]->state = TASK_RUNNING;
	task[3]->counter = 5;
	task[3]->priority = 5;
	task[3]->blocked = 0;
	task[3]->pid = 3;

	task[3]->thread.ra = sepc_assign;
	task[3]->thread.sp = task[3] + TASK_SIZE;

	// 2.4 intialize task[4]
	task[4] = (uint64)0x80014000 + space_offset;
	task[4]->state = TASK_RUNNING;
	task[4]->counter = 4;
	task[4]->priority = 5;
	task[4]->blocked = 0;
	task[4]->pid = 4;

	task[4]->thread.ra = sepc_assign;
	task[4]->thread.sp = task[4] + TASK_SIZE;	

	for(i = 1; i < 5; i++){
		//printf("[PID = %d] Process Create Successfully! counter = %d priority = %d\n", i, task[i]->counter, task[i]->priority);
		puts("[PID = ");
		puti(i);
		puts("] Process Create Successfully! counter = ");
		puti(task[i]->counter);
		puts(" priority = ");
		puti(task[i]->priority);
		puts("\n");
	}

	#endif		
}

/* 在时钟中断处理中被调用 */
void do_timer(void)
{
	#ifdef SJF    //only SJF outputs "[PID = X] Context Calculation: counter = X\n"
	//printf("[PID = %d] Context Calculation: counter = %d\n", current->pid, current->counter);
	puts("[PID = ");
	puti(current->pid);
	puts("] Context Calculation: counter = ");
	puti(current->counter);
	puts("\n");
	#endif

	// 1. subtract current process counter by 1
	--current->counter;		

	// 2. schedule according to SJF or PRIORITY
	#ifdef SJF
	if(current->counter > 0)		// counter>0, continue this process
		return;
	current->counter = 0;

	#else
	if(current->counter == 0){
		switch(current->pid){
			case 1:
				current->counter = 7;
				break;
			case 2:
				current->counter = 6;
				break;
			case 3:
				current->counter = 5;
				break;
			case 4:
				current->counter = 4;
				break;
			default:
				//printf("ERROR:current->pid=0, counter=0\n");
				puts("ERROR: pid = 0\n");
		}
	}
	#endif

	schedule();

}

/* 调度程序 */
void schedule(void)
{
	int next_pid, i;
	#ifdef SJF
	// reset if all zero
	if(task[1]->counter == 0 && task[2]->counter == 0 &&
		task[3]->counter == 0 && task[4]->counter == 0)
		for(int i = 1; i < 5; i++){
			task[i]->counter = rand();
			//printf("[PID = %d] Reset counter = %d\n", i, task[i]->counter);
			puts("[PID = ");
			puti(i);
			puts("] Reset counter = ");
			puti(task[i]->counter);
			puts("\n");
		}

	for(i = 4, next_pid = -1; i > 0; i--){
		if(task[i]->state != TASK_RUNNING || task[i]->counter == 0)
			continue;
		if(next_pid == -1)
			next_pid = i;

		if(task[i]->counter < task[next_pid]->counter)
			next_pid = i;
	}
	//printf("[!] Switch from task %d to task %d, prio: %d, counter: %d\n", current->pid, next_pid, task[next_pid]->priority, task[next_pid]->counter);
	puts("[!] Switch from task ");
	puti(current->pid);
	puts(" [task struct: ");
	putx(current);
	puts(", sp: ");
	putx(current->thread.sp);
	puts("]");
	puts(" to task ");
	puti(next_pid);
	puts(" [task struct: ");
	putx(task[next_pid]);
	puts(", sp: ");
	putx(task[next_pid]->thread.sp);
	puts("]");
	puts(", prio: ");
	puti(task[next_pid]->priority);
	puts(", counter: ");
	puti(task[next_pid]->counter);
	puts("\n");


	#else
	for(i = 4, next_pid = 4; i > 0; i--){
		if(task[i]->state != TASK_RUNNING)
			continue;

		if(task[i]->priority < task[next_pid]->priority)
			next_pid = i;
		else if(task[i]->priority == task[next_pid]->priority &&
			task[i]->counter < task[next_pid]->counter)
			next_pid = i;
	}

	//printf("[!] Switch from task %d to task %d, prio: %d, counter: %d\n", current->pid, next_pid, task[next_pid]->priority, task[next_pid]->counter);
	puts("[!] Switch from task ");
	puti(current->pid);
	puts(" [task struct: ");
	putx(current);
	puts(", sp: ");
	putx(current->thread.sp);
	puts("]");
	puts(" to task ");
	puti(next_pid);
	puts(" [task struct: ");
	putx(task[next_pid]);
	puts(", sp: ");
	putx(task[next_pid]->thread.sp);
	puts("]");
	puts(", prio: ");
	puti(task[next_pid]->priority);
	puts(", counter: ");
	puti(task[next_pid]->counter);
	puts("\n");

	//printf("tasks' priority changed\n");
	puts("tasks' priority changed\n");
	for(i = 1; i < 5; i++){
		task[i]->priority = rand();
		//printf("[PID = %d] counter = %d priority = %d\n", i, task[i]->counter, task[i]->priority);
		puts("[PID = ");
		puti(i);
		puts("] counter = ");
		puti(task[i]->counter);
		puts(" priority = ");
		puti(task[i]->priority);
		puts("\n");
	}

	#endif
	switch_to(task[next_pid]);
}

/* 切换当前任务current到下一个任务next */
void switch_to(struct task_struct* next)
{
	if(current == next)
		return;
/*	C version
	// context switch
	// 1. preserve current process's regs
	unsigned long long temp;
	asm volatile("mv %0, ra": "=r"(temp) :);
	current->thread.ra = temp;

	asm volatile("mv %0, sp": "=r"(temp) :);
	current->thread.sp = temp;

	asm volatile("mv %0, s0": "=r"(temp) :);
	current->thread.s0 = temp;
	asm volatile("mv %0, s1": "=r"(temp) :);
	current->thread.s1 = temp;
	asm volatile("mv %0, s2": "=r"(temp) :);
	current->thread.s2 = temp;
	asm volatile("mv %0, s3": "=r"(temp) :);
	current->thread.s3 = temp;
	asm volatile("mv %0, s4": "=r"(temp) :);
	current->thread.s4 = temp;
	asm volatile("mv %0, s5": "=r"(temp) :);
	current->thread.s5 = temp;
	asm volatile("mv %0, s6": "=r"(temp) :);
	current->thread.s6 = temp;	
	asm volatile("mv %0, s7": "=r"(temp) :);
	current->thread.s7 = temp;
	asm volatile("mv %0, s8": "=r"(temp) :);
	current->thread.s8 = temp;
	asm volatile("mv %0, s9": "=r"(temp) :);
	current->thread.s9 = temp;
	asm volatile("mv %0, s10": "=r"(temp) :);
	current->thread.s10 = temp;
	asm volatile("mv %0, s11": "=r"(temp) :);
	current->thread.s11 = temp;

	// 2. load next process's regs
	temp = next->thread.ra;
	asm volatile("mv ra, %0": :"r"(temp));

	temp = next->thread.sp;
	asm volatile("mv sp, %0": :"r"(temp));

	temp = next->thread.s0;
	asm volatile("mv s0, %0": :"r"(temp));
	temp = next->thread.s1;
	asm volatile("mv s1, %0": :"r"(temp));
	temp = next->thread.s2;
	asm volatile("mv s2, %0": :"r"(temp));	
	temp = next->thread.s3;
	asm volatile("mv s3, %0": :"r"(temp));	
	temp = next->thread.s4;
	asm volatile("mv s4, %0": :"r"(temp));
	temp = next->thread.s5;
	asm volatile("mv s5, %0": :"r"(temp));	
	temp = next->thread.s6;
	asm volatile("mv s6, %0": :"r"(temp));	
	temp = next->thread.s7;
	asm volatile("mv s7, %0": :"r"(temp));	
	temp = next->thread.s8;
	asm volatile("mv s8, %0": :"r"(temp));
	temp = next->thread.s9;
	asm volatile("mv s9, %0": :"r"(temp));
	temp = next->thread.s10;
	asm volatile("mv s10, %0": :"r"(temp));	
	temp = next->thread.s11;
	asm volatile("mv s11, %0": :"r"(temp));	
*/
	struct task_struct* prev = current;
	current = next;
	__switch_to(prev, next);
	return;
}

/* 死循环 */
void dead_loop(void)
{
	while(1)
		;
}
