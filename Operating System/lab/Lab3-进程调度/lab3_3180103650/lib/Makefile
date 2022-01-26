all: put.o rand.o

vpath %.h ../include

put.o:put.h
	$(GCC) $(CFLAG) -c put.c -o put.o

rand.o:rand.h
	$(GCC) $(CFLAG) -c rand.c -o rand.o

.PHONY:clean
clean:
	rm -f *.o

