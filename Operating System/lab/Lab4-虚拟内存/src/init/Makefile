all: test.o main.o

vpath %.h ../include
vpath %.h ../arch/riscv/include

test.o:test.h put.h sched.h
	$(GCC) $(CFLAG) -I ../arch/riscv/include -c test.c -o test.o
main.o:test.h
	$(GCC) $(CFLAG) -c main.c -o main.o

.PHONY:clean
clean:
	rm -f *.o
