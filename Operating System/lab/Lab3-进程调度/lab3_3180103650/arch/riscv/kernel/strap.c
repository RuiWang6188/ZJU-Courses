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



