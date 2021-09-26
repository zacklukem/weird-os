
BUILD_DIR := build

SOURCES := $(shell find . -name "*.c" -or -name "*.cpp" -or -name "*.asm")

OBJECTS := $(SOURCES:%=$(BUILD_DIR)/%.o)

CFLAGS := -Werror -Wall -m32 -fno-exceptions -fno-rtti -std=c++1z -O0          \
					-ffreestanding -Iincludes

$(BUILD_DIR)/clib.a: $(OBJECTS)
	i386-elf-ar ru -o $@ $^

$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	i386-elf-gcc $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	i386-elf-gcc -nostdlib -Iincludes -c $< -o $@

$(BUILD_DIR)/%.asm.o: %.asm
	mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@

.PHONY: clean
clean:
	rm -r build
