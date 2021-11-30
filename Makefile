-include config.mk

.DEFAULT_GOAL := all

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

.PHONY: build
build: $(BUILDDIR)/$(KERNEL_IMG)

$(BUILDDIR)/$(KERNEL_IMG): $(OBJS)
	$(LD) $(LDFLAGS) -T $K/kernel.ld -o $(BUILDDIR)/$(KERNEL_IMG) $(OBJS)
	$(OBJDUMP) -S $(BUILDDIR)/$(KERNEL_IMG) > $(BUILDDIR)/kernel.asm
	@echo 'Build $(KERNEL_IMG) done'

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)

# QEMU
ifndef CPUS
CPUS := 1
endif
BOOTLOADER = default

QEMU ?= qemu-system-riscv64
QEMUEXTRA =
QEMUOPTS = \
	-nographic \
	-machine virt \
	-m 128M \
	-smp $(CPUS) \
	-bios $(BOOTLOADER) \
	-kernel $(BUILDDIR)/$(KERNEL_IMG) \
	$(QEMUEXTRA)

.PHONY: qemu
qemu: $(BUILDDIR)/$(KERNEL_IMG)
	$(QEMU) $(QEMUOPTS)
