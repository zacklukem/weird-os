include scripts/globals.mk

MOD := root

BUILD_DIR := build

FORMAT_FILES := $(shell find . \( -not -wholename "build/*" \) -and            \
	\( -name "*.h" -or -name "*.cpp" -or -name "*.c" \))

.PHONY: all
all: $(BUILD_DIR)/os.iso

.PHONY: test
test: $(BUILD_DIR)/os.iso
	./scripts/test.sh

.PHONY: test_debug
test_debug: $(BUILD_DIR)/os.iso
	./scripts/test.sh -S -s

$(BUILD_DIR)/os.iso: kernel/build/test_kernel.elf iso/boot/initrd.img
	$(gen_banner)
	cp kernel/build/test_kernel.elf iso/boot/kernel.elf
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

iso/boot/initrd.img: $(BUILD_DIR)/makefs
	$(MAKE) -C init
	$(gen_banner)
	$(BUILD_DIR)/makefs initrd iso/boot/initrd.img

$(BUILD_DIR)/makefs: scripts/makefs.c
	$(cc_banner)
	mkdir -p $(dir $@)
	gcc scripts/makefs.c -o $(BUILD_DIR)/makefs

kernel/build/test_kernel.elf:
	$(MAKE) build/test_kernel.elf -C kernel

.PHONY: format
format: $(FORMAT_FILES)
	printf "%-40s \033[0;32mFormatting...\033[0m\n" "$@"
	clang-format -i $(FORMAT_FILES)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	+$(MAKE) clean -C init
	+$(MAKE) clean -C clib
	+$(MAKE) clean -C kernel
	+$(MAKE) clean -C tests
