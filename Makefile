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

INITRD_OBJS := $(shell find initrd/ -type f) initrd/hi

DEFS :=

LD := i386-elf-ld
LDFLAGS := -T link.ld

CC := i386-elf-g++
CFLAGS := -Werror -Wall -m32 -fno-exceptions -fno-rtti -std=c++1z -O0\
					-ffreestanding -g3 -F dwarf -MMD -MP -Iincludes -fno-inline-functions
#CFLAGS := -m32 -MMD -MP -g3 -F dwarf -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -Iincludes

AS := nasm
ASFLAGS := -f elf32 -g3 -F dwarf

os.iso: $(BUILD_DIR)/test_kernel.elf iso/boot/initrd.img
	printf "%-40s \033[0;33mCreating bootimage...\033[0m\r" "$@"
	cp $(BUILD_DIR)/test_kernel.elf iso/boot/kernel.elf
	genisoimage -R                                \
                -b boot/grub/stage2_eltorito    \
                -no-emul-boot                   \
                -boot-load-size 4               \
                -A os                           \
                -input-charset utf8             \
                -quiet                          \
                -boot-info-table                \
                -o os.iso                       \
                iso
	printf "%-40s \033[0;32mCreated.               \033[0m\n" "$@"

iso/boot/initrd.img: $(BUILD_DIR)/makefs $(INITRD_OBJS)
	printf "%-40s \033[0;33mCreating initrd...\033[0m\r" "$@"
	$(BUILD_DIR)/makefs initrd iso/boot/initrd.img
	printf "%-40s \033[0;32mCreated.            \033[0m\n" "$@"

initrd/hi: initrd/hi.c
	printf "%-40s \033[0;33mBuilding...\033[0m\r" "$@"
	i386-elf-gcc initrd/hi.c -nostdlib -o initrd/hi_elf
	objcopy -O binary -j .text initrd/hi_elf initrd/hi
	printf "%-40s \033[0;32mBuilt.            \033[0m\n" "$@"


$(BUILD_DIR)/makefs: makefs.c
	printf "%-40s \033[0;33mCompiling...\033[0m\r" "$@"
	gcc makefs.c -o $(BUILD_DIR)/makefs
	printf "%-40s \033[0;32mCompiled.            \033[0m\n" "$@"
	

$(BUILD_DIR)/kernel.elf: $(KERNEL_OBJS)
	printf "%-40s \033[0;33mLinking...\033[0m\r" "$@"
	$(LD) $(LDFLAGS) $(KERNEL_OBJS) -o $@
	cp $(BUILD_DIR)/kernel.elf iso/boot/kernel.elf
	printf "%-40s \033[0;32mLinked.            \033[0m\n" "$@"

$(BUILD_DIR)/%.cpp.o: %.cpp
	printf "%-40s \033[0;33mCompiling...\033[0m\r" "$@"
	mkdir -p $(dir $@)
	$(CC) $(DEFS) $(CFLAGS) -c $< -o $@
	printf "%-40s \033[0;32mCompiled.            \033[0m\n" "$@"

$(BUILD_DIR)/%.asm.o: %.asm
	printf "%-40s \033[0;33mAssembling...\033[0m\r" "$@"
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@
	printf "%-40s \033[0;32mAssembled.            \033[0m\n" "$@"

$(BUILD_DIR)/test_kernel.elf: tests/test/list_gen.h $(KERNEL_OBJS) $(TEST_OBJS)
	printf "%-40s \033[0;33mLinking...\033[0m\r" "$@"
	$(LD) $(LDFLAGS) $(KERNEL_OBJS) $(TEST_OBJS) -o $@
	printf "%-40s \033[0;32mLinked.            \033[0m\n" "$@"

.PHONY: kernel
kernel: DEFS=
kernel: $(BUILD_DIR)/kernel.elf

.PHONY: test
test: DEFS=-DTEST_RUN_MODE
test: test_setup os.iso
	./test.sh

.PHONY: test_debug
test_debug: DEFS=-DTEST_RUN_MODE
test_debug: test_setup os.iso
	./test.sh -S -s

.PHONY: qemu
qemu: kernel
	./qemu.sh

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

.PHONY: test_setup
test_setup:
	printf "%-40s \033[0;33mGenerating...\033[0m\r" "$@"
	./tests/test/setup.sh
	printf "%-40s \033[0;32mGenerated.            \033[0m\n" "$@"

-include $(KERNEL_C_DEPS)
-include $(TEST_DEPS)
