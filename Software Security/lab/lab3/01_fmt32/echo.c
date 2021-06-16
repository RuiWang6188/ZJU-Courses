#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>

#define MAX_LEN 512
#include "target.h"

extern long int id;

void target_XXX() 
{
    target_function_XXX();
    return;
}

void echo()
{
    printf("Remeber that &id = %p\n", &id);
    printf("You can exactly 256 charecters ...\n");
    char buffer[256];
    read(STDIN_FILENO, buffer, 256);
    printf(buffer);
    puts("done");
    return;
}

void prepare()
{
    setvbuf(stdin, 0LL, 2, 0LL);
    setvbuf(stdout, 0LL, 2, 0LL);
    setvbuf(stderr, 0LL, 2, 0LL);
}

int main(int argc, char* argv[])
{
    prepare();
    echo();
    exit(1);
}
