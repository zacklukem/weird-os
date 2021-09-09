BUILD_DIR := ./build
BOOT_SRC := ./boot
KERNEL_SRC := ./kernel

# Find all the C and C++ files we want to compile
BOOT_SRCS := $(shell find $(BOOT_SRC) -name "*.asm")
BOOT_OBJS := $(BOOT_SRCS:%=$(BUILD_DIR)/%.o)

KERNEL_C_SRCS := $(shell find $(KERNEL_SRC) -name "*.c")
KERNEL_C_DEPS := $(KERNEL_C_SRCS:%=$(BUILD_DIR)/%.d)

KERNEL_SRCS := $(shell find $(KERNEL_SRC) -name "*.asm") $(KERNEL_C_SRCS)
KERNEL_OBJS := $(KERNEL_SRCS:%=$(BUILD_DIR)/%.o)

LD := i386-elf-ld
LDFLAGS := -melf_i386

CC := i386-elf-gcc
CFLAGS := -Werror -m32 -ffreestanding -g3 -F dwarf -MMD -MP -Iincludes

AS := nasm
ASFLAGS := -f elf32 -g3 -F dwarf

OBJCOPY := i386-elf-objcopy

$(BUILD_DIR)/disk.img: $(BUILD_DIR)/boot_loader.img $(BUILD_DIR)/kernel.img
	printf "[$<] Building final image...\r"
	cat $(BUILD_DIR)/boot_loader.img $(BUILD_DIR)/kernel.img > $@
	./utils/pad5s $@
	printf "[$<] Done.                        \n"

$(BUILD_DIR)/boot_loader.img: $(BUILD_DIR)/boot_loader.elf
	printf "[$<] Converting to binary image...\r"
	$(OBJCOPY) -O binary $< $@
	./utils/pad $@
	printf "[$<] Done.                        \n"

$(BUILD_DIR)/kernel.img: $(BUILD_DIR)/kernel.elf
	printf "[$<] Converting to binary image...\r"
	$(OBJCOPY) -O binary $< $@
	printf "[$<] Done.                        \n"

$(BUILD_DIR)/kernel.elf: $(KERNEL_OBJS)
	printf "[$<] Linking...\r"
	$(LD) -Ttext=0x1000 $(LDFLAGS) $(KERNEL_OBJS) -o $@
	printf "[$<] Done.     \n"

$(BUILD_DIR)/boot_loader.elf: $(BOOT_OBJS)
	printf "[$<] Linking...\r"
	$(LD) -Ttext=0x7c00 $(LDFLAGS) $(BOOT_OBJS) -o $@
	printf "[$<] Done.     \n"

$(BUILD_DIR)/%.c.o: %.c
	printf "[$<] Compiling...\r"
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
	printf "[$<] Done.       \n"

$(BUILD_DIR)/%.asm.o: %.asm
	printf "[$<] Assembling...\r"
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@
	printf "[$<] Done.        \n"

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

-include $(KERNEL_C_DEPS)
