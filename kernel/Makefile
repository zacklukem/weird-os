include ../scripts/globals.mk

MOD := kernel

BUILD_DIR := build

KERNEL_SRC := .

LDFLAGS += -T../scripts/link.ld

# Find all the C and C++ files we want to compile
KERNEL_C_SRCS := $(shell find $(KERNEL_SRC) -name "*.cpp")
KERNEL_C_DEPS := $(KERNEL_C_SRCS:%=$(BUILD_DIR)/%.d)

KERNEL_SRCS := $(shell find $(KERNEL_SRC) -name "*.asm") $(KERNEL_C_SRCS)
KERNEL_OBJS := $(KERNEL_SRCS:%=$(BUILD_DIR)/%.o)

# For some reason static linking was being weird so here we go:
TEST_SRCS := $(shell cd ../tests && find -name "*.cpp")
TEST_OBJS := $(TEST_SRCS:%=../tests/build/%.o)

DEFS :=

CPPFLAGS += -I../includes
CFLAGS += -I../includes

$(BUILD_DIR)/kernel.elf: DEFS=
$(BUILD_DIR)/kernel.elf: $(KERNEL_OBJS)
	$(ld_banner)
	$(LD) $(LDFLAGS) $(KERNEL_OBJS) -o $@

$(BUILD_DIR)/test_kernel.elf: DEFS=-DTEST_RUN_MODE
$(BUILD_DIR)/test_kernel.elf: $(KERNEL_OBJS)
	$(MAKE) -C ../tests
	$(ld_banner)
	$(LD) $(LDFLAGS) $(KERNEL_OBJS) $(TEST_OBJS) -o $@

# Build C++ sources
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(cpp_banner)
	mkdir -p $(dir $@)
	$(CPP) $(DEFS) $(CPPFLAGS) -c $< -o $@

# Build NASM sources
$(BUILD_DIR)/%.asm.o: %.asm
	$(as_banner)
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

#../tests/build/libtests.a:
#	$(MAKE) -C ../tests

.PHONY: clean
clean:
	echo "Cleaning kernel..."
	rm -rf build

-include $(KERNEL_C_DEPS)
