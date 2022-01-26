#include "stdio.h"
#include "sched.h"
#include "types.h"
#include "syscall.h"

int count = 0;

void handler_s(size_t scause, size_t sepc, uintptr_t *regs)
{
	if (scause >> 63) {		// interrupt
		if ( ( (scause << 1) >> 1 ) == 5 ) {	// supervisor timer interrupt
			asm volatile("ecall");
			do_timer();
		}
	}
	else {				// exception
		if(scause == 15){	// store pagefault exception
			puts("ERROR: Store Page Fault\n");
		}
		else if(scause == 13){	// load pagefault exception
			puts("ERROR: Load Page Fault\n");
		}
		else if(scause == 12){	// instruction pagefault exception
			puts("ERROR: Instruction Page Fault\n");
		}
		else if(scause == 8){	// ecall from U mode
			uint64 a0, a1, a2, a7;
			asm volatile("ld %0, 17*8(%1)":"+r"(a7):"r"(regs));	// read a7

			if(a7 == SYS_WRITE){
				asm volatile("ld %0, 10*8(%3)\n"
							 "ld %1, 11*8(%3)\n"
							 "ld %2, 12*8(%3)\n"
							 : "+r"(a0), "+r"(a1), "+r"(a2) :"r"(regs));
				sys_write(a0, a1, a2);
			}
			else if(a7 == SYS_GETPID){
				sys_getpid();
				asm volatile("sd a0, 10*8(%0)"::"r"(regs));
			}
		}


		asm volatile("sd %0, 32*8(%1)"::"r"(sepc + 4), "r"(regs));	// modify the sepc->sepc+4
		}
	return;
}



