ASM_SRC		= $(sort $(wildcard *.S))
C_SRC       = $(sort $(wildcard *.c))
OBJ		    = $(patsubst %.S,%.o,$(ASM_SRC)) $(patsubst %.c,%.o,$(C_SRC))

all: $(OBJ)

%.o:%.c
	${CC} ${CFLAG} -c $<

clean:
	@rm *.o 2>/dev/null

