#include "stdio.h"

int main()
{

	while(1) {
        for (int i = 0; i < 0x50000000; i++);
        puts("In main Loop\n");
    };

    return 0;
}
