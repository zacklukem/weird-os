include ../scripts/globals.mk

BUILD_DIR := build

MOD := tests

CPPFLAGS += -I../includes
CFLAGS += -I../includes

TEST_SRC := .

TEST_SRCS := $(shell find $(TEST_SRC) -name "*.cpp")
TEST_OBJS := $(TEST_SRCS:%=$(BUILD_DIR)/%.o)
TEST_DEPS := $(TEST_SRCS:%=$(BUILD_DIR)/%.d)

all: test_setup $(TEST_OBJS)

# Build C++ sources
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(cpp_banner)
	mkdir -p $(dir $@)
	$(CPP) $(CPPFLAGS) -c $< -o $@

.PHONY: test_setup
test_setup:
	$(gen_banner)
	test/setup.sh

.PHONY: clean
clean:
	echo "Cleaning tests..."
	rm test/list_gen.h
	rm test/head_gen.h
	rm -rf build

-include $(TEST_DEPS)
