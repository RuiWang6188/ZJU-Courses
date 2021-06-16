#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DDL_NUM (16)

struct ddl_mgr {
	char ddl_time[32];
	char ddl_content[32];
};

struct ddl_mgr **array;

void backdoor()
{
	puts("Hah! you got me");
	system("/bin/sh");
}

void prepare()
{
	setvbuf(stdin, 0LL, 2, 0LL);
	setvbuf(stdout, 0LL, 2, 0LL);
	alarm(60); // 1 minute timeout
	array = (struct ddl_mgr **)malloc(DDL_NUM * sizeof(struct ddl_mgr*));
	memset(array, 0, (DDL_NUM * sizeof(struct ddl_mgr*)));
}

void banner()
{
	printf("88  88 888888    db    88**Yb II\n");
	printf("88  88 88__     dPYb   88__dP II\n");
	printf("888888 88**    dP__Yb  88***  II\n");
	printf("88  88 888888 dP****Yb 88     II\n");
	printf("\n");
}

void showTbl()
{
	puts("You have following choices\n [1]: add a ddl\n [2]: finish a ddl\n [3]: show a ddl\n [4]: edit a ddl\n [5]: exit\nYour chocie:");
}

void get_input_custom(char* ptr, int len)
{
	if (!len)
		return;
	read(0, ptr, len);
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
	get_input_custom(ddl_ptr->ddl_content, 1024);
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
			get_input_custom(ddl_ptr->ddl_content, 1024);
			puts("done");
			return;
		}
	}
        puts("invalid ddl index");
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
			default:
				puts("bad choice here...");
				break;
		}
	}
}
