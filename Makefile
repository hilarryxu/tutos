-include config.mk

.PHONY: all
all: build

K = kernel

TOOLPREFIX ?= riscv64-unknown-elf-
CC ?= $(TOOLPREFIX)gcc
AS = $(TOOLPREFIX)gcc
LD ?= $(TOOLPREFIX)ld
GDB ?= $(TOOLPREFIX)gdb
OBJCOPY = $(TOOLPREFIX)objcopy
OBJDUMP = $(TOOLPREFIX)objdump

BUILDDIR = build

C_SRCS = $(wildcard $K/*.c)
AS_SRCS = $(wildcard $K/*.S)
C_OBJS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(C_SRCS))))
AS_OBJS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(AS_SRCS))))
OBJS = $(C_OBJS) $(AS_OBJS)

KERNEL_IMG = kernel.img

CFLAGS = -Wall -Werror -O -fno-omit-frame-pointer -ggdb
CFLAGS += -MD
CFLAGS += -mcmodel=medany
CFLAGS += -ffreestanding -fno-common -nostdlib -mno-relax
CFLAGS += -I.
CFLAGS += -I$K
CFLAGS += -fno-stack-protector

$(AS_OBJS): $(BUILDDIR)/$K/%.o : $K/%.S
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(C_OBJS): $(BUILDDIR)/$K/%.o : $K/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

.phony: build
build: $(BUILDDIR)/$(KERNEL_IMG)

$(BUILDDIR)/$(KERNEL_IMG): $(OBJS)
	$(LD) $(LDFLAGS) -T $K/kernel.ld -o $(BUILDDIR)/$(KERNEL_IMG) $(OBJS)
	@echo 'Build kernel.img done'
