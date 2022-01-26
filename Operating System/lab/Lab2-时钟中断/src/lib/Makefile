all: put.o

vpath %.h ../include

put.o:put.h
	$(GCC) $(CFLAG) -c put.c -o put.o

.PHONY:clean
clean:
	rm -f *.o

