ASM_SRC		= $(sort $(wildcard *.S))
C_SRC       = $(sort $(wildcard *.c))
OBJ		    = $(patsubst %.S,%.o,$(ASM_SRC)) $(patsubst %.c,%.o,$(C_SRC))

all: $(OBJ) 

head.o:head.S
	${CC}  ${CF_M}  -c $<
%.o:%.S
	${CC}  ${CFLAG}  -c $<
%.o:%.c
	${CC}  ${CFLAG}  -c $<

clean:
	$(shell rm *.o 2>/dev/null)
