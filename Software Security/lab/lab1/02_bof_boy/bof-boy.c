#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define LENGTH 10
void target_code(void) 			
{ 					         
    printf("[HACKED]\n");
    execve("/bin/sh", NULL, NULL);
    exit(0); 
}

void func(void)
{
    char buffer[LENGTH]={'0'};
    read(0,buffer,LENGTH*3);
    if (strlen(buffer) > 10) {
        printf("[x] Invalid Password \n");
	exit(0);
    }
}

int main(int argc, char **argv)
{
    setvbuf(stdin, 0LL, 2, 0LL);
    setvbuf(stdout, 0LL, 2, 0LL);
    setvbuf(stderr, 0LL, 2, 0LL);
    printf("[*] ZJUSSEC HW1: Buffer Overflow Boy \n");
    printf("[*] Give me something to overflow me! \n");
    func();
    printf("please try again!\n");
    return 0;
}

