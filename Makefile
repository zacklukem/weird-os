BUILD_DIR := build
KERNEL_SRC := kernel clib

# Find all the C and C++ files we want to compile
KERNEL_C_SRCS := $(shell find $(KERNEL_SRC) -name "*.c")
KERNEL_C_DEPS := $(KERNEL_C_SRCS:%=$(BUILD_DIR)/%.d)

KERNEL_SRCS := $(shell find $(KERNEL_SRC) -name "*.asm") $(KERNEL_C_SRCS)
KERNEL_OBJS := $(KERNEL_SRCS:%=$(BUILD_DIR)/%.o)

LD := i386-elf-ld
LDFLAGS := -T link.ld

CC := i386-elf-gcc
CFLAGS := -Werror -Wall -m32 -ffreestanding -g3 -F dwarf -MMD -MP -Iincludes
#CFLAGS := -m32 -MMD -MP -g3 -F dwarf -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Iincludes

AS := nasm
ASFLAGS := -f elf32 -g3 -F dwarf

$(BUILD_DIR)/kernel.elf: $(KERNEL_OBJS)
	printf "[$@] Linking...\r"
	$(LD) $(LDFLAGS) $(KERNEL_OBJS) -o $@
	printf "[$@] Done.     \n"

$(BUILD_DIR)/%.c.o: %.c
	printf "[$@] Compiling...\r"
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	printf "[$@] Done.       \n"

$(BUILD_DIR)/%.asm.o: %.asm
	printf "[$@] Assembling...\r"
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@
	printf "[$@] Done.        \n"

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(KERNEL_C_DEPS)
