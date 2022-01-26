#include "put.h"

int count = 0;

void print_message(void){
    const char *msg =" [S] Supervisor Mode Timer Interrupt ";
    puts(msg);
    puti(count);
	count++;
    
    const char *enter = "\n";
    puts(enter);
}

void print_inst_pgfault(void){
	puts("ERROR: Instruction Page Fault\n");
}

void print_ld_pgfault(void){
	puts("ERROR: Load Page Fault\n");
}

void print_st_pgfault(void){
	puts("ERROR: Store Page Fault\n");
}



