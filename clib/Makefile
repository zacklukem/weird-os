
BUILD_DIR := build
OBJECTS := $(BUILD_DIR)/crt0.asm.o

$(BUILD_DIR)/clib.a: $(OBJECTS)
	i386-elf-ar ru -o $@ $^

$(BUILD_DIR)/%.asm.o: %.asm
	mkdir -p $(dir $@)
	nasm -f elf32 $< -o $@

.PHONY: clean
clean:
	rm -r build
