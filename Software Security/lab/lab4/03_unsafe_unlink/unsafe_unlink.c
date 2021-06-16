#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DDL_NUM (16)
#define DDL_CONTENT (0x5d8)

struct ddl_mgr {
	char ddl_time[32];
	char ddl_content[DDL_CONTENT];
};

struct ddl_mgr *array[DDL_NUM];
unsigned long targetID = 0;

void prepare()
{
	setvbuf(stdin, 0LL, 2, 0LL);
	setvbuf(stdout, 0LL, 2, 0LL);
	alarm(60); // 1 minute timeout
	memset(array, 0, (DDL_NUM * sizeof(struct ddl_mgr*)));
}

void banner()
{
	printf("88  88 888888    db    88**Yb II II\n");
	printf("88  88 88__     dPYb   88__dP II II\n");
	printf("888888 88**    dP__Yb  88***  II II\n");
	printf("88  88 888888 dP****Yb 88     II II\n");
	printf("\n");
}

void showTbl()
{
	puts("You have following choices\n [1]: add a ddl\n [2]: finish a ddl\n [3]: show a ddl\n [4]: edit a ddl\n [5]: exit\n [6] check\nYour chocie:");
}

void get_input_custom(char* ptr, int len)
{
	int i = 0;
	char buf;
	if (!len)
		return;
	while ( i < len ) {
		read(0, &buf, 1u);
		if ( buf == '\n' ) {
			ptr[i] = 0;
			return;
		}
		ptr[i++] = buf;
	}
	ptr[i] = 0;
}

void add_ddl()
{
	int i;
	struct ddl_mgr* ddl_ptr;
	for(i = 0; i < DDL_NUM; i++) 
		if (!array[i])
			break;
	if (i == DDL_NUM) {
		puts("ddl is full");
		return;
	}

	ddl_ptr = malloc(sizeof(struct ddl_mgr));

	printf("creating ddl with index-%d\n", i + 1);
	puts("please input the ddl time");
	get_input_custom(ddl_ptr->ddl_time, 32);
	puts("please input the ddl content");
	get_input_custom(ddl_ptr->ddl_content, DDL_CONTENT);
	puts("done");
	array[i] = ddl_ptr;
}

void finish_ddl()
{
	int index;
	puts("please input the ddl index");
	scanf("%d", &index);
	index = index - 1;

	if (0 <= index && index < DDL_NUM) {
		if (array[index]) {
			free(array[index]);
			array[index] = NULL;
			puts("done");
			return;
		}
	}
	
	puts("invalid ddl index");
}

void show_ddl()
{
        int index;
        puts("please input the ddl index");
        scanf("%d", &index);
        index = index - 1;

        if (0 <= index && index < DDL_NUM) { 
		if (array[index]) {
                	printf("ddl time: %s\n", array[index]->ddl_time);
			printf("ddl content: %s\n", array[index]->ddl_content);
                	puts("done");
			return;
		}
        }
        
	puts("invalid ddl index");
}

void edit_ddl()
{
        int index;
        puts("please input the ddl index");
        scanf("%d", &index);
        index = index - 1;

        if (0 <= index && index < DDL_NUM) {
		if (array[index]) {
                	struct ddl_mgr* ddl_ptr = array[index];
			puts("please input the new ddl time");
			get_input_custom(ddl_ptr->ddl_time, 32);
			puts("please input the new ddl content");
			get_input_custom(ddl_ptr->ddl_content, DDL_CONTENT);
			puts("done");
			return;
        	}
	}
        
	puts("invalid ddl index");
}

void check()
{
	if (targetID) {
		printf("Successfully change id to %lu\n", targetID);
		system("/bin/sh");
		exit(0);
	}
	else {
		puts("try more");
	}
}

int main(int argc, char* argv[])
{
	int choice = 0;
	prepare();
	banner();
	while(1) {
		showTbl();
		scanf("%d", &choice);
		switch (choice) {
			case 1:
				add_ddl();
				break;
			case 2:
				finish_ddl();
				break;
			case 3:
				show_ddl();
				break;
			case 4:
				edit_ddl();
				break;
			case 5:
				puts("see you next time!");
				exit(0);
			case 6:
				check();
				break;
			default:
				puts("bad choice here...");
				break;
		}
	}
}
