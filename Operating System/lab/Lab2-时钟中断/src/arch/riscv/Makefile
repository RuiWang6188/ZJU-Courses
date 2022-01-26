all:
	make -C kernel
	$(LD) ../../init/test.o ../../init/main.o ./kernel/head.o ./kernel/entry.o ./kernel/strap.o ../../lib/put.o -T ./kernel/vmlinux.lds -o ../../vmlinux
	$(OBJCOPY) -O binary ../../vmlinux ./boot/Image --strip-all
	nm ../../vmlinux > ../../System.map

.PHONY:clean
clean:
	cd kernel && make clean
	rm ../../vmlinux
	rm ../../System.map
	rm ./boot/Image
