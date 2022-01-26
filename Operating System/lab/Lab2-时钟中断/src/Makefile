export
CROSS_= riscv64-unknown-elf-
AR=${CROSS_}ar
AS=${CROSS_}as
GCC=${CROSS_}gcc
LD=${CROSS_}ld
OBJCOPY=${CROSS_}objcopy

ISA ?= rv64imafd
ABI ?= lp64

INCLUDE = -I ../include
CF = -c -g -march=$(ISA) -mabi=$(ABI) -mcmodel=medany -ffunction-sections -fdata-sections -nostartfiles -nostdlib -nostdinc -static -lgcc -Wl,--nmagic -Wl,--gc-sections
CFLAG = ${CF} ${INCLUDE}

all:
	make -C lib
	make -C init
	make -C arch/riscv	
	
run:
	qemu-system-riscv64 -nographic -machine virt -kernel vmlinux

debug:
	qemu-system-riscv64 -nographic -machine virt -kernel vmlinux -S -s

.PHONY: clean
clean:
	cd lib && make clean
	cd init && make clean 
	cd arch/riscv && make clean
	
