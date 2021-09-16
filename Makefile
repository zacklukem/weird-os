MAKEFLAGS += --silent

BUILD_DIR := build
KERNEL_SRC := kernel clib

TEST_SRC := tests

TEST_SRCS := $(shell find $(TEST_SRC) -name "*.cpp")
TEST_OBJS := $(TEST_SRCS:%=$(BUILD_DIR)/%.o)
TEST_DEPS := $(TEST_SRCS:%=$(BUILD_DIR)/%.d)

# Find all the C and C++ files we want to compile
KERNEL_C_SRCS := $(shell find $(KERNEL_SRC) -name "*.cpp")
KERNEL_C_DEPS := $(KERNEL_C_SRCS:%=$(BUILD_DIR)/%.d)

KERNEL_SRCS := $(shell find $(KERNEL_SRC) -name "*.asm") $(KERNEL_C_SRCS)
KERNEL_OBJS := $(KERNEL_SRCS:%=$(BUILD_DIR)/%.o)

DEFS :=

LD := i386-elf-ld
LDFLAGS := -T link.ld

CC := i386-elf-g++
CFLAGS := -Wall -m32 -ffreestanding -g3 -F dwarf -MMD -MP -Iincludes
#CFLAGS := -m32 -MMD -MP -g3 -F dwarf -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Iincludes

AS := nasm
ASFLAGS := -f elf32 -g3 -F dwarf

.PHONY: kernel
kernel: DEFS=
kernel: $(BUILD_DIR)/kernel.elf

$(BUILD_DIR)/kernel.elf: $(KERNEL_OBJS)
	printf "[$@] Linking...\r"
	$(LD) $(LDFLAGS) $(KERNEL_OBJS) -o $@
	printf "[$@] Done.     \n"

$(BUILD_DIR)/%.cpp.o: %.cpp
	printf "[$@] Compiling...\r"
	mkdir -p $(dir $@)
	$(CC) $(DEFS) $(CFLAGS) -c $< -o $@
	printf "[$@] Done.       \n"

$(BUILD_DIR)/%.asm.o: %.asm
	printf "[$@] Assembling...\r"
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@
	printf "[$@] Done.        \n"

$(BUILD_DIR)/test_kernel.elf: tests/test/list_gen.h $(KERNEL_OBJS) $(TEST_OBJS)
	printf "[$@] Linking...\r"
	$(LD) $(LDFLAGS) $(KERNEL_OBJS) $(TEST_OBJS) -o $@
	printf "[$@] Done.     \n"

.PHONY: test
test: DEFS=-DTEST_RUN_MODE
test: test_setup $(BUILD_DIR)/test_kernel.elf
	./test.sh

.PHONY: test_debug
test_debug: DEFS=-DTEST_RUN_MODE
test_debug: test_setup $(BUILD_DIR)/test_kernel.elf
	./test.sh -S -s

.PHONY: qemu
qemu: kernel
	./qemu.sh

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

.PHONY: test_setup
test_setup:
	printf "[$@] Generating...\r"
	./tests/test/setup.sh
	printf "[$@] Done.        \n"

-include $(KERNEL_C_DEPS)
-include $(TEST_DEPS)
