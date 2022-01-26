all: test.o main.o

vpath %.h ../include

test.o:test.h put.h
	$(GCC) $(CFLAG) -c test.c -o test.o
main.o:test.h
	$(GCC) $(CFLAG) -c main.c -o main.o

.PHONY:clean
clean:
	rm -f *.o
