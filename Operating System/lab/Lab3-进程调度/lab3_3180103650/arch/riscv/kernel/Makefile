VPATH = ../include:../../../include

all: head.o entry.o strap.o sched.o

head.o: head.S
	$(GCC) $(CFLAG) head.S -o head.o
	
entry.o: entry.S
	$(GCC) $(CFLAG) entry.S -o entry.o
	
strap.o: put.h
	$(GCC) $(CFLAG) strap.c -o strap.o

sched.o: sched.h rand.h
	$(GCC) $(CFLAG) -I ../../../include sched.c -o sched.o

.PHONY:clean
clean:
	rm -f *.o
