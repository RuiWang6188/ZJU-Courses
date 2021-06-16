#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 50

void hear(){
    char p1 = 'N';
    char p2 = 'Y';
    char str[LENGTH];
    gets(str);
    if (p1==p2) {
	printf("[HACKED]\n");
	system("/bin/sh");
    }	
}

int main(int argc, char* argv[]){
        setvbuf(stdin, 0LL, 2, 0LL);
	setvbuf(stdout, 0LL, 2, 0LL);
        setvbuf(stderr, 0LL, 2, 0LL);
	printf("ZJUSSEC HW1: Buffer Overflow Baby\n");
	printf("Tell me something, less than %d characters:\n", LENGTH);
	hear();
	printf("You can`t hack me,please try again!\n");
        return 0;
}

