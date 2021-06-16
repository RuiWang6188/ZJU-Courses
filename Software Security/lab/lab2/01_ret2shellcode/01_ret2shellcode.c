#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LENGTH 256

void hear()
{
	char str[LENGTH];
	gets(str);
}

int main(void)
{
	setbuf(stdin,NULL);
	setbuf(stdout,NULL);
	setbuf(stderr,NULL);
	printf("[*] ZJUSSEC HW02: ret2shellcode\n");
	printf("[*] Please input your ID:\n");

	long long int id=0;
	scanf("%lld", &id);

	getchar();

	printf("[*] Hi, %lld. Your ID is stored at:0x%016llX\n", id, &id);
	printf("[*] Now, give me something to overflow me!\n");
	hear();
	return 0;
}

