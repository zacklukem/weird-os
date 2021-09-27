MAKEFLAGS += --silent

CROSS_BUILD_PREFIX := i386-elf-

LD := $(CROSS_BUILD_PREFIX)ld
LDFLAGS :=

CPP := $(CROSS_BUILD_PREFIX)gcc
CPPFLAGS := -Werror -Wall -m32 -fno-exceptions -fno-rtti -std=c++1z -O0        \
						-ffreestanding -g3 -F dwarf -MMD -MP -Iincludes

CC := $(CROSS_BUILD_PREFIX)gcc
CFLAGS := -Werror -Wall -m32 -nostdlib -ffreestanding -g3 -F dwarf -MMD -MP    \
					-Iincludes

AS := nasm
ASFLAGS := -f elf32 -g3 -F dwarf

AR := $(CROSS_BUILD_PREFIX)ar

ifeq ($(MOD),)
MOD := undefined
endif

define task_banner
	printf "\033[0;34m%-8s\033[0m \033[0;32m%-3s\033[0m %s\n" "[$(MOD)]" "$1" "$@" 
endef

define as_banner
	$(call task_banner,"AS")
endef

define cc_banner
	$(call task_banner,"CC")
endef

define cpp_banner
	$(call task_banner,"CPP")
endef

define ld_banner
	$(call task_banner,"LD")
endef

define gen_banner
	$(call task_banner,"GEN")
endef

