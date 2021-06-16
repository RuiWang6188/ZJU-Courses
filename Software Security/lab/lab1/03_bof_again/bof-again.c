#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define LENGTH 20
void target_code(unsigned int arg1,unsigned int arg2) 			
{
	if(arg1==0xaaaabbbb&&arg2==0xccccdddd){
	    printf("[HACKED]\n");
	    execve("/bin/sh", NULL, NULL);
	}else{
	    printf("please try again!\n");
	}	
	return;
}

void func(void)
{
    char buffer[LENGTH]={'0'};
    read(0,buffer,LENGTH*3);
}

int main(void)
{
    setvbuf(stdin, 0LL, 2, 0LL);
    setvbuf(stdout, 0LL, 2, 0LL);
    setvbuf(stderr, 0LL, 2, 0LL);
    printf("[*] ZJUSSEC HW1: Buffer Overflow Again \n");
    printf("[*] Give me something to overflow me! \n");
    func();
    printf("please try again!\n");
    return 0;
}

