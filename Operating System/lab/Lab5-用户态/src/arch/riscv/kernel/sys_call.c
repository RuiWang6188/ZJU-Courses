#include "syscall.h"
#include "stdio.h"
#include "device.h"
#include "rand.h"
#include "sched.h"
#include "mm.h"
#include "symbol.h"
#include "stdio.h"

const char * temp_buf;
size_t temp_count;

void  sys_write(unsigned int fd, const char* buf, size_t count)
{	
	temp_buf = buf;
	temp_count = count;

	write_csr(satp,MAKE_SATP(current->mm->upgtbl));
	asm volatile("sfence.vma");

	for(int i = 0; i < temp_count; i++)
		putchar(temp_buf[i]);
	
	write_csr(satp,0x8000000000080ffe);
	asm volatile("sfence.vma");
	asm volatile("mv a0, %0"::"r"(count));
}

void  sys_getpid(void)
{
	asm volatile("mv a0, %0"::"r"(current->pid));
}

