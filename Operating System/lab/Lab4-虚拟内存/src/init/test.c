#include "test.h"
#include "put.h"
#include "../arch/riscv/include/sched.h"

int os_test()
{
	const char *msg = "ZJU OS LAB 4             GROUP-32\n";

    puts(msg);

    task_init();

	while(1) ;

    return 0;
}
