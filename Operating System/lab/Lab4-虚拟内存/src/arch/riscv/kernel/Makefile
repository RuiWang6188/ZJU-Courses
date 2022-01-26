VPATH = ../include:../../../include

all: head.o entry.o strap.o sched.o vm.o

head.o: head.S
	$(GCC) $(CFLAG) head.S -o head.o
	
entry.o: entry.S
	$(GCC) $(CFLAG) entry.S -o entry.o
	
strap.o: put.h
	$(GCC) $(CFLAG) strap.c -o strap.o

sched.o: sched.h rand.h
	$(GCC) $(CFLAG) -I ../../../include sched.c -o sched.o

vm.o: put.h vm.h
	$(GCC) $(CFLAG) vm.c -o vm.o	

.PHONY:clean
clean:
	rm -f *.o
