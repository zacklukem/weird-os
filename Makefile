MAKEFLAGS += --silent

BUILD_DIR := build
KERNEL_SRC := kernel drivers

FORMAT_FILES := $(shell find . \( -not -wholename "build/*" \) -and            \
	\( -name "*.h" -or -name "*.cpp" -or -name "*.c" \))

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
LDFLAGS := -T scripts/link.ld

CC := i386-elf-g++
CFLAGS := -Werror -Wall -m32 -fno-exceptions -fno-rtti -std=c++1z -O0          \
					-ffreestanding -g3 -F dwarf -MMD -MP -Iincludes

AS := nasm
ASFLAGS := -f elf32 -g3 -F dwarf

$(BUILD_DIR)/os.iso: $(BUILD_DIR)/test_kernel.elf iso/boot/initrd.img
	printf "%-40s \033[0;33mCreating bootimage...\033[0m\r" "$@"
	cp $(BUILD_DIR)/test_kernel.elf iso/boot/kernel.elf
	genisoimage -R                                                               \
							-b boot/grub/stage2_eltorito                                     \
							-no-emul-boot                                                    \
							-boot-load-size 4                                                \
							-A os                                                            \
							-input-charset utf8                                              \
							-quiet                                                           \
							-boot-info-table                                                 \
							-o $(BUILD_DIR)/os.iso                                           \
							iso
	printf "%-40s \033[0;32mCreated.               \033[0m\n" "$@"

iso/boot/initrd.img: $(BUILD_DIR)/makefs $(INITRD_OBJS)
	printf "%-40s \033[0;33mCreating initrd...\033[0m\r" "$@"
	cd init
	make
	cd ..
	$(BUILD_DIR)/makefs initrd iso/boot/initrd.img
	printf "%-40s \033[0;32mCreated.            \033[0m\n" "$@"

$(BUILD_DIR)/makefs: scripts/makefs.c
	printf "%-40s \033[0;33mCompiling...\033[0m\r" "$@"
	gcc scripts/makefs.c -o $(BUILD_DIR)/makefs
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
test: test_setup $(BUILD_DIR)/os.iso
	./scripts/test.sh

.PHONY: test_debug
test_debug: DEFS=-DTEST_RUN_MODE
test_debug: test_setup $(BUILD_DIR)/os.iso
	./scripts/test.sh -S -s

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
	rm initrd/hi

.PHONY: test_setup
test_setup:
	printf "%-40s \033[0;33mGenerating...\033[0m\r" "$@"
	./tests/test/setup.sh
	printf "%-40s \033[0;32mGenerated.            \033[0m\n" "$@"

.PHONY: format
format: $(FORMAT_FILES)
	printf "%-40s \033[0;33mFormatting...\033[0m\r" "$@"
	clang-format -i $(FORMAT_FILES)
	printf "%-40s \033[0;32mFormatted.            \033[0m\n" "$@"

-include $(KERNEL_C_DEPS)
-include $(TEST_DEPS)
