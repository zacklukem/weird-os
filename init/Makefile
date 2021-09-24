
CLIB := ../clib/build/clib.a

../initrd/hi: $(CLIB) hi.c
	i386-elf-gcc $^ -nostdlib -o ../initrd/hi

$(CLIB):
	cd ../clib
	make
	cd ../init

