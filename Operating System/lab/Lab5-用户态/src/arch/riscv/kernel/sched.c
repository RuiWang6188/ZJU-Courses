#include "stdio.h"
#include "device.h"
#include "rand.h"
#include "sched.h"
#include "mm.h"
#include "vm.h"
#include "symbol.h"
#include "stdio.h"

extern pagetable_t kpgtbl; 

union task_union {
    struct task_struct task;
    char stack[PAGE_SIZE];
};

struct task_struct* current;
struct task_struct* task[NR_TASKS];

extern void __sret(void);


#ifdef SJF

int task_init_done = 0; 

void task_init(void) {
    puts("task init...\n");

	// 进程的PCB
    current = (struct task_struct*) kalloc(); 
	current->mm = (struct mm_struct*) kalloc();

    // 用户顶级根页表
    current->mm->upgtbl = init_task_pagetable();

    //变量存储
    kvmmap(current->mm->upgtbl, PA2VA(kpgtbl), kpgtbl, PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);
    kvmmap(current->mm->upgtbl, PA2VA(current->mm->upgtbl), current->mm->upgtbl, PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);
    kvmmap(current->mm->upgtbl, PA2VA(current), current, PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);
    kvmmap(current->mm->upgtbl, PA2VA(current->mm), current->mm, PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);

	//映射内核页表
 	uint64 uart = PA2VA(get_device_addr(UART_MMIO));
	uint64 poweroff = PA2VA(get_device_addr(POWEROFF_MMIO));
    kvmmap(current->mm->upgtbl, uart, VA2PA(uart), get_device_size(UART_MMIO), (PTE_R | PTE_W ));
    kvmmap(current->mm->upgtbl, poweroff, VA2PA(poweroff), get_device_size(POWEROFF_MMIO), (PTE_R | PTE_W ));
    kvmmap(current->mm->upgtbl, (uint64)&text_start, VA2PA((uint64)&text_start), (uint64)&text_end - (uint64)&text_start, (PTE_R | PTE_X | PTE_W));
	kvmmap(current->mm->upgtbl, (uint64)&rodata_start, VA2PA((uint64)&rodata_start), (uint64)&rodata_end - (uint64)&rodata_start, (PTE_R | PTE_W));
    kvmmap(current->mm->upgtbl, (uint64)&data_start, VA2PA((uint64)&data_start), (uint64)&data_end - (uint64)&data_start, (PTE_R | PTE_W ));
    kvmmap(current->mm->upgtbl, (uint64)&bss_start, VA2PA((uint64)&bss_start), (uint64)&bss_end - (uint64)&bss_start, (PTE_R | PTE_X | PTE_W ));
   
   //用户代码映射
    uvminit(current->mm->upgtbl);

    //内核栈
	char* kstack_pa = kalloc();
    uint64 kstack_va = PA2VA(kstack_pa);
    kvmmap(current->mm->upgtbl, (uint64)kstack_va, (uint64)kstack_pa, PAGE_SIZE, PTE_R | PTE_W | PTE_X );
    current->mm->kstack = kstack_va + PAGE_SIZE;

    // 用户栈
	current->mm->ustack = kalloc();	
    kvmmap(current->mm->upgtbl, (uint64)USTACK_TOP - PAGE_SIZE, (uint64)current->mm->ustack, PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);
    
    //基本初始值设置
    current->state = TASK_RUNNING;
    current->counter = 0;
    current->priority = 5;
    current->blocked = 0;
    current->pid = 0;
	current->thread.sp = (unsigned long long) kstack_va + PAGE_SIZE;
	current->thread.ra = (unsigned long long) & swtch_begin;
	current->thread.satp = (MAKE_SATP(current->mm->upgtbl));
	current->thread.sscratch = kstack_va + PAGE_SIZE;
	current->thread.sstatus = 0x40001;
    task[0] = current; 
    
    // 转换到虚拟空间
    task[0]->mm = PA2VA(task[0]->mm);
    task[0] = PA2VA(task[0]);
    current = PA2VA(current);         
    

    /* set other 4 tasks */
    for (int i = 1; i <= LAB_TEST_NUM; ++i) {
		task[i] = (struct task_struct*) kalloc();
        task[i]->mm = (struct mm_struct*) kalloc();
        task[i]->mm->upgtbl = init_task_pagetable();


        kvmmap(task[i]->mm->upgtbl, uart, VA2PA(uart), get_device_size(UART_MMIO), (PTE_R | PTE_W ));
        kvmmap(task[i]->mm->upgtbl, poweroff, VA2PA(poweroff), get_device_size(POWEROFF_MMIO), (PTE_R | PTE_W ));
        kvmmap(task[i]->mm->upgtbl, (uint64)&bss_start, VA2PA((uint64)&bss_start), (uint64)&bss_end - (uint64)&bss_start, (PTE_R | PTE_X | PTE_W));
        kvmmap(task[i]->mm->upgtbl, (uint64)&text_start, VA2PA((uint64)&text_start), (uint64)&text_end - (uint64)&text_start, (PTE_R | PTE_X |PTE_W));
        kvmmap(task[i]->mm->upgtbl, (uint64)&rodata_start, VA2PA((uint64)&rodata_start), (uint64)&rodata_end - (uint64)&rodata_start, (PTE_R | PTE_W));
        kvmmap(task[i]->mm->upgtbl, (uint64)&data_start, VA2PA((uint64)&data_start), (uint64)&data_end - (uint64)&data_start, (PTE_R | PTE_W ));
  
        kvmmap(task[i]->mm->upgtbl, PA2VA(kpgtbl), kpgtbl, PAGE_SIZE, PTE_R | PTE_W | PTE_X |PTE_U);
        kvmmap(task[i]->mm->upgtbl, PA2VA(task[i]->mm->upgtbl), task[i]->mm->upgtbl, PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);
        kvmmap(task[i]->mm->upgtbl, PA2VA(task[i]), task[i], PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);
        kvmmap(task[i]->mm->upgtbl, PA2VA(task[i]->mm), task[i]->mm, PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);
    
        uvminit(task[i]->mm->upgtbl);


        kstack_pa = kalloc();
		uint64 kstack_va = PA2VA(kstack_pa);
        mappages(task[i]->mm->upgtbl, kstack_va, PAGE_SIZE, kstack_pa, PTE_R | PTE_W | PTE_X);
        task[i]->mm->kstack = kstack_va + PAGE_SIZE;
        
		task[i]->mm->ustack = kalloc();
        mappages(task[i]->mm->upgtbl, USTACK_TOP - PAGE_SIZE, PAGE_SIZE, task[i]->mm->ustack, PTE_R | PTE_W | PTE_X | PTE_U);


        task[i]->pid = i;
        task[i]->state = TASK_RUNNING;
        task[i]->priority = 5; // All tasks initialized with the default priority (lowest)
        task[i]->counter = rand(); 
        task[i]->blocked = PREEMPT_ENABLE;
        task[i]->thread.ra = (unsigned long long) &swtch_begin;
        task[i]->thread.sp = (unsigned long long)kstack_va + PAGE_SIZE;
		task[i]->thread.satp = (MAKE_SATP(task[i]->mm->upgtbl));
		task[i]->thread.sscratch = kstack_va + PAGE_SIZE;
		task[i]->thread.sstatus = 0x40001;
        

        printf("[PID = %d] Process Create Successfully! counter = %d\n", task[i]->pid, task[i]->counter);

		task[i]->mm = PA2VA(task[i]->mm);
		task[i] = PA2VA(task[i]);
    }

	write_csr(satp, MAKE_SATP(kpgtbl));
	asm volatile("sfence.vma");

	task_init_done = 1;
		
}


void do_timer(void) {
    if (!task_init_done) return;	


    printf("[PID = %d] Context Calculation: counter = %d\n", current->pid, current->counter);
    /* 当前进程的时间片减1 */
    (current->counter)--;
    /* 如果当前进程时间片已经使用完 则进行进程调度 */
    if ((current->counter) <= 0) {
        current->counter = 0;
        schedule(); 
    }

    return;
}



void schedule(void) {
    long cmin;
    unsigned char selector;
    unsigned char next;
    struct task_struct** ptr;
    while (1) {
        cmin = 0xFFFF;
        selector = NR_TASKS;
        ptr = &task[NR_TASKS];

        while (--selector) {
            if (!*--ptr || (*ptr)->counter == 0)
                continue;
            if ((*ptr)->state == TASK_RUNNING && (*ptr)->counter < cmin) {
                cmin = (*ptr)->counter;
                next = selector;
                // puti(next);
                // puti(cmin);
            }
        }

        if (cmin != 0xFFFF) {
            break;
        }
        else {
            // 如果task0～4都运行完毕，则重新生成新的不同运行时间长度的任务
            for (int i = 1; i <= LAB_TEST_NUM; ++i) {
                task[i]->counter = rand();
                printf("[PID = %d] Reset counter = %d\n", i, task[i]->counter);
            }
			return;
        }
    }


    printf("[!] Switch from task %d [task struct: 0x%lx, sp: 0x%lx] to task %d [task struct: 0x%lx, sp: 0x%lx], prio: %d, counter: %d\n", 
        current->pid, (unsigned long)current, current->thread.sp,
        task[next]->pid, (unsigned long)task[next], task[next]->thread.sp,
        task[next]->priority, task[next]->counter);

    switch_to(task[next]);
}

#endif

#ifdef PRIORITY

int task_init_done = 0;
void task_init(void) {

    current = (struct task_struct*) kalloc();
    current->state = TASK_RUNNING;
    current->counter = 0;
    current->priority = 5;
    current->blocked = 0;
    current->pid = 0;

    task[0] = current;
    current->mm = (struct mm_struct*) kalloc();

    // 用户顶级根页表
    current->mm->upgtbl = init_task_pagetable();

    kvmmap(current->mm->upgtbl, PA2VA(current->mm->upgtbl), current->mm->upgtbl, PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);
    kvmmap(current->mm->upgtbl, PA2VA(current), current, PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);
    kvmmap(current->mm->upgtbl, PA2VA(current->mm), current->mm, PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);

    //映射内核页表
    printf("mapping devices...\n");
    uint64 uart = PA2VA(get_device_addr(UART_MMIO));
    uint64 poweroff = PA2VA(get_device_addr(POWEROFF_MMIO));
    kvmmap(current->mm->upgtbl, uart, VA2PA(uart), get_device_size(UART_MMIO), (PTE_R | PTE_W ));
    kvmmap(current->mm->upgtbl, poweroff, VA2PA(poweroff), get_device_size(POWEROFF_MMIO), (PTE_R | PTE_W ));
    kvmmap(current->mm->upgtbl, (uint64)&text_start, VA2PA((uint64)&text_start), (uint64)&text_end - (uint64)&text_start, (PTE_R | PTE_X | PTE_W));
    kvmmap(current->mm->upgtbl, (uint64)&rodata_start, VA2PA((uint64)&rodata_start), (uint64)&rodata_end - (uint64)&rodata_start, (PTE_R | PTE_W));
    kvmmap(current->mm->upgtbl, (uint64)&data_start, VA2PA((uint64)&data_start), (uint64)&data_end - (uint64)&data_start, (PTE_R | PTE_W ));
    kvmmap(current->mm->upgtbl, (uint64)&bss_start, VA2PA((uint64)&bss_start), (uint64)&bss_end - (uint64)&bss_start, (PTE_R | PTE_X | PTE_W ));
   
   //用户代码映射
    uvminit(current->mm->upgtbl);

    //内核栈
    char* kstack_pa = kalloc();
    uint64 kstack_va = PA2VA(kstack_pa);
    kvmmap(current->mm->upgtbl, (uint64)kstack_va, (uint64)kstack_pa, PAGE_SIZE, PTE_R | PTE_W | PTE_X );
    current->mm->kstack = kstack_va + PAGE_SIZE;


    current->mm->ustack = kalloc(); 

    kvmmap(current->mm->upgtbl, (uint64)USTACK_TOP - PAGE_SIZE, (uint64)current->mm->ustack, PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);

    current->thread.sp = (unsigned long long) kstack_va + PAGE_SIZE;
    current->thread.ra = (unsigned long long) & swtch_begin;
    current->thread.satp = (MAKE_SATP(current->mm->upgtbl));
    current->thread.sscratch = kstack_va + PAGE_SIZE;
    current->thread.sstatus = 0x40001;

    task[0]->mm = PA2VA(task[0]->mm);
    task[0] = PA2VA(task[0]);
    current = PA2VA(current);         

    /* set other 4 tasks */
    for (int i = 1; i <= LAB_TEST_NUM; ++i) {
        task[i] = (struct task_struct*) kalloc();
        task[i]->mm = (struct mm_struct*) kalloc();
        task[i]->mm->upgtbl = init_task_pagetable();

        kvmmap(task[i]->mm->upgtbl, uart, VA2PA(uart), get_device_size(UART_MMIO), (PTE_R | PTE_W ));
        kvmmap(task[i]->mm->upgtbl, poweroff, VA2PA(poweroff), get_device_size(POWEROFF_MMIO), (PTE_R | PTE_W ));
        kvmmap(task[i]->mm->upgtbl, (uint64)&bss_start, VA2PA((uint64)&bss_start), (uint64)&bss_end - (uint64)&bss_start, (PTE_R | PTE_X | PTE_W));
        kvmmap(task[i]->mm->upgtbl, (uint64)&text_start, VA2PA((uint64)&text_start), (uint64)&text_end - (uint64)&text_start, (PTE_R | PTE_X |PTE_W));
        kvmmap(task[i]->mm->upgtbl, (uint64)&rodata_start, VA2PA((uint64)&rodata_start), (uint64)&rodata_end - (uint64)&rodata_start, (PTE_R | PTE_W));
        kvmmap(task[i]->mm->upgtbl, (uint64)&data_start, VA2PA((uint64)&data_start), (uint64)&data_end - (uint64)&data_start, (PTE_R | PTE_W ));
  
        kvmmap(task[i]->mm->upgtbl, PA2VA(kpgtbl), kpgtbl, PAGE_SIZE, PTE_R | PTE_W | PTE_X |PTE_U);
        kvmmap(task[i]->mm->upgtbl, PA2VA(task[i]->mm->upgtbl), task[i]->mm->upgtbl, PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);
        kvmmap(task[i]->mm->upgtbl, PA2VA(task[i]), task[i], PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);
        kvmmap(task[i]->mm->upgtbl, PA2VA(task[i]->mm), task[i]->mm, PAGE_SIZE, PTE_R | PTE_W | PTE_X | PTE_U);
    
        uvminit(task[i]->mm->upgtbl);


        kstack_pa = kalloc();
        uint64 kstack_va = PA2VA(kstack_pa);
        mappages(task[i]->mm->upgtbl, kstack_va, PAGE_SIZE, kstack_pa, PTE_R | PTE_W | PTE_X);
        task[i]->mm->kstack = kstack_va + PAGE_SIZE;
        
        task[i]->mm->ustack = kalloc();
        mappages(task[i]->mm->upgtbl, USTACK_TOP - PAGE_SIZE, PAGE_SIZE, task[i]->mm->ustack, PTE_R | PTE_W | PTE_X | PTE_U);


        task[i]->pid = i;
        task[i]->state = TASK_RUNNING;
        task[i]->priority = 8 - i; // All tasks initialized with the default priority (lowest)
        task[i]->counter = rand(); 
        task[i]->blocked = PREEMPT_ENABLE;
        task[i]->thread.ra = (unsigned long long) &swtch_begin;
        task[i]->thread.sp = (unsigned long long)kstack_va + PAGE_SIZE;
        task[i]->thread.satp = (MAKE_SATP(task[i]->mm->upgtbl));
        task[i]->thread.sscratch = kstack_va + PAGE_SIZE;
        task[i]->thread.sstatus = 0x40001;
        
        printf("[PID = %d] Process Create Successfully! counter = %d\n", task[i]->pid, task[i]->counter);

        task[i]->mm = PA2VA(task[i]->mm);
        task[i] = PA2VA(task[i]);    }
    task_init_done = 1;

}

void do_timer(void) {
    if (!task_init_done) return;
    /* 当前进程的时间片减1 */
    (current->counter)--;
    /* 如果当前进程时间片已经使用完，则从头运行该task */
    /* 当在do_timer中发现当前运行进程剩余运行时间为0（即当前进程运行结束）时，
       需重新为该进程分配其对应的运行时长。
       相当于重启当前进程（每个进程的运行时间长度和初始化的值一致）。
    */
    if ((current->counter) <= 0 && (current->pid) != 0) { // 排除task0，task0不参与后续的优先级调度
        current->counter = 8 - current->pid; // counter = 7, 6, 5, 4（分别对应 task[1-4]的运行时长）
    }
    /* 每次do_timer都进行一次抢占试优先级调度 */
    schedule();
    return;
}




void schedule(void) {
    long pmin;
    long cmin;
    unsigned char selector;
    unsigned char next;
    struct task_struct** ptr;

    while (1) {
        pmin = 5;
        cmin = 0xFFFF;
        next = 0;
        selector = NR_TASKS;
        ptr = &task[NR_TASKS];

        while (--selector) {
            if (!*--ptr)
                continue;
            if ((*ptr)->state == TASK_RUNNING && (*ptr)->priority < pmin) {
                cmin = (*ptr)->counter;
                pmin = (*ptr)->priority;
                next = selector;
            }
            if ((*ptr)->state == TASK_RUNNING && (*ptr)->priority == pmin) {
                if ((*ptr)->counter < cmin) {
                    cmin = (*ptr)->counter;
                    next = selector;
                }
            }
        }
        if (next) break;
    }

    if ((current->pid) != next) {
        printf("[!] Switch from task %d [task struct: 0x%lx, sp: 0x%lx] to task %d [task struct: 0x%lx, sp: 0x%lx], prio: %d, counter: %d\n", 
        current->pid, (unsigned long)current, current->thread.sp,
        task[next]->pid, (unsigned long)task[next], task[next]->thread.sp,
        task[next]->priority, task[next]->counter);
    }
    // 动态调整task[1-4]的优先级
    for (int i = 1; i <= LAB_TEST_NUM; ++i) {
        task[i]->priority = rand();
    }

    puts("tasks' priority changed\n");
    for (int i = 1;i <= LAB_TEST_NUM;i++) {
        printf("[PID = %d] counter = %d priority = %d\n", task[i]->pid, task[i]->counter, task[i]->priority);
    }
    
    switch_to(task[next]);
}

#endif

void switch_to(struct task_struct* next) {
    if (current == next) return;
    struct task_struct* prev = current;
    current = next;

    __switch_to(prev, next);
} 

void swtch_begin() {

    write_csr(sscratch, current->mm->kstack);

    current->thread.sp= (unsigned long long)(USTACK_TOP);


    write_csr(satp, MAKE_SATP(current->mm->upgtbl));
    asm volatile("sfence.vma");

    //设置用户的栈空间
    asm volatile("mv sp , %0"::"r"((uint64)(USTACK_TOP)));

    write_csr(sepc,(0x0));

    asm volatile("sret");
}

void dead_loop(void)
{
    while (1) {}
}
