VPATH = ../include

all: head.o entry.o strap.o

head.o: head.S
	$(GCC) $(CFLAG) head.S -o head.o
	
entry.o: entry.S
	$(GCC) $(CFLAG) entry.S -o entry.o
	
strap.o: strap.c put.h
	$(GCC) $(CFLAG) strap.c -o strap.o

.PHONY:clean
clean:
	rm -f *.o
