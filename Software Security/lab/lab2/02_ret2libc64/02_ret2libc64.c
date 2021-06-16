#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LENGTH 256

char id[20];

int read_num(char* buf, unsigned int length)
{
    int index=0;
    char temp;
    while(index < length)
    {
        read(STDIN_FILENO, &temp, 1);
        if(temp>='0'&&temp<='9')
        {
            buf[index]=temp;
        }else
        {
            buf[index]=0;
            return index;
        }
        index++; 
    }
    buf[index-1]=0;
    return index-1;
}
void hear(void)
{
    char str[LENGTH];
    read(STDIN_FILENO, str, LENGTH+0x48);
}

int main(int argc, char **argv)
{
	setbuf(stdin,NULL);
    setbuf(stdout,NULL);
    setbuf(stderr,NULL);

    puts("[*] ZJUSSEC HW2: Ret2libc64\n[*]");
    puts("[*] Now, please input your ID:");
    read_num(id, 20);
    void *addr=(void *)0x601018;
    printf("[*] And function puts()'s GOT is:0x%016llx (you should already know...\n", (unsigned long long)addr);
    printf("[*] Hi, %s. You should leak the libc_base. Give me something to overflow me!\n", id);
    hear();
}
