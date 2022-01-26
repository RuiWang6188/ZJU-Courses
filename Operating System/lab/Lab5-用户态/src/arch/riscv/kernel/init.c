#include "stdlib.h"
#include "stdio.h"
#include "device.h"
#include "mm.h"
#include "vm.h"
#include "symbol.h"
#include "sched.h"

extern int main();

memmap_t __mmio[] = {
    [UART_MMIO] 	= { 0x10000000	, 0x100		},
    [CLINT_MMIO] 	= { 0x2000000	, 0x10000	},
    [PLIC_MMIO] 	= { 0xc000000	, 0x4000000	},
    [POWEROFF_MMIO] = { 0x100000	, 0x1000	},
    [DRAM_MMIO] 	= { 0x80000000	, 0x0		},
	[NULL_MMIO]		= { 0x0			, 0x0		}
};

void device_init() {
    register_console(&console_ns16550a);
    register_poweroff(&poweroff_sifive_test);
}

int start_kernel() {

	memset(&bss_start, 0, &bss_end - &bss_start);

	device_init();
	Log("Finish device_init");

	kinit();
	Log("Finish kinit");

	kvminit();
	Log("Finish kvminit");

	printf("rinux start...\n");

	// Only for lab 3
    task_init();
	Log("Finish task_init");

	swtch_begin();

	//main();
	
	exit(0);

	return 0;
}
