-include config.mk

.DEFAULT_GOAL := all

.PHONY: all
all: build

K = kernel
BUILDDIR = build
KERNEL_IMG = $(BUILDDIR)/kernel.img

TOOLPREFIX ?= riscv64-unknown-elf-
CC ?= $(TOOLPREFIX)gcc
AS ?= $(TOOLPREFIX)gcc
LD ?= $(TOOLPREFIX)ld
GDB ?= $(TOOLPREFIX)gdb
OBJCOPY = $(TOOLPREFIX)objcopy
OBJDUMP ?= $(TOOLPREFIX)objdump

C_SRCS = $(wildcard $K/*.c)
AS_SRCS = $(wildcard $K/*.S)
C_OBJS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(C_SRCS))))
AS_OBJS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(basename $(AS_SRCS))))
OBJS = $(C_OBJS) $(AS_OBJS)

HEADER_DEP = $(addsuffix .d, $(basename $(C_OBJS)))

-include $(HEADER_DEP)

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

$(C_OBJS): $(BUILDDIR)/$K/%.o : $K/%.c $(BUILDDIR)/$K/%.d
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(HEADER_DEP): $(BUILDDIR)/$K/%.d : $K/%.c
	@mkdir -p $(@D)
	@set -e; rm -f $@; $(CC) -MM $< $(INCLUDEFLAGS) > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

.PHONY: build
build: $(KERNEL_IMG)

$(KERNEL_IMG): $(OBJS)
	$(LD) $(LDFLAGS) -T $K/kernel.ld -o $(KERNEL_IMG) $(OBJS)
	@if which $(OBJDUMP) >/dev/null 2>&1; then \
		$(OBJDUMP) -S $(KERNEL_IMG) > $(BUILDDIR)/kernel.asm; \
		$(OBJDUMP) -t $(KERNEL_IMG) | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' | awk '$$2 != "" && $$2 !~ /\./ && $$2 !~ /__/ {print $$1,$$2}' > $(BUILDDIR)/kernel.sym; \
	fi
	@echo 'Build $(KERNEL_IMG) done'

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)

.PHONY: bootstrap
bootstrap:
	cp zcc.tmpl zcc
	cp config.mk.tmpl config.mk

# QEMU
ifndef CPUS
CPUS := 1
endif
BIOS = default

QEMU ?= qemu-system-riscv64
QEMUEXTRA =
QEMUOPTS = \
	-nographic \
	-machine virt \
	-m 128M \
	-smp $(CPUS) \
	-bios $(BIOS) \
	-kernel $(KERNEL_IMG) \
	$(QEMUEXTRA)

.PHONY: qemu
qemu: $(KERNEL_IMG)
	$(QEMU) $(QEMUOPTS)
