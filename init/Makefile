# init makefile
include ../scripts/globals.mk

MOD := init

CLIB := ../clib/build/clib.a

../initrd/hi: $(CLIB) build/hi.c.o
	$(ld_banner)
	$(LD) $^ -o ../initrd/hi

build/hi.c.o: hi.c
	$(cc_banner)
	mkdir -p $(dir $@)
	$(CC) -I../clib/includes -nostdlib -c $< -o $@

$(CLIB):
	$(MAKE) -C ../clib

.PHONY: clean
clean:
	echo "Cleaning init..."
	rm -f ../initrd/hi
	rm -rf build
