# clib makefile
include ../scripts/globals.mk

MOD := clib

CLIB_BUILD_DIR := build

CLIB_SOURCES := $(shell find . -name "*.c" -or -name "*.cpp" -or -name "*.asm")

CLIB_OBJECTS := $(CLIB_SOURCES:%=$(CLIB_BUILD_DIR)/%.o)

$(CLIB_BUILD_DIR)/clib.a: $(CLIB_OBJECTS)
	$(ld_banner)
	$(AR) crs -o $@ $^

$(CLIB_BUILD_DIR)/%.cpp.o: %.cpp
	$(cpp_banner)
	mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(CLIB_BUILD_DIR)/%.c.o: %.c
	$(cc_banner)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(CLIB_BUILD_DIR)/%.asm.o: %.asm
	$(as_banner)
	mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@

.PHONY: clean
clean:
	echo "Cleaning clib..."
	rm -rf build
