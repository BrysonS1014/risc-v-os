# TinyRV-OS Makefile
# Assumes a RISC-V cross toolchain named riscv64-unknown-elf-gcc.
# You can override CROSS on the command line, e.g.:
#   make CROSS=riscv64-linux-gnu-

CROSS ?= riscv64-unknown-elf-

CC      := $(CROSS)gcc
OBJDUMP := $(CROSS)objdump
OBJCOPY := $(CROSS)objcopy

CFLAGS  := -std=c11 -Wall -Wextra -O2 -g \
           -ffreestanding -fno-builtin -fno-stack-protector \
           -fno-pic \
           -march=rv64gc -mabi=lp64 -msmall-data-limit=0 \
           -mcmodel=medany

LDFLAGS := -nostdlib -nostartfiles -T linker.ld -Wl,--build-id=none \
           -mcmodel=medany

SRC_C := \
    src/kernel.c \
    src/kstring.c \
    src/uart.c \
    src/console.c \
    src/scheduler.c \
    src/sync.c \
    src/fs.c \
    src/programs.c \
    src/shell.c

SRC_S := \
    src/boot.S \
    src/context.S

OBJ   := $(SRC_C:.c=.o) $(SRC_S:.S=.o)

all: tinyrv-os.elf

tinyrv-os.elf: $(OBJ) linker.ld
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) tinyrv-os.elf tinyrv-os.dump

dump: tinyrv-os.elf
	$(OBJDUMP) -d tinyrv-os.elf > tinyrv-os.dump

run: tinyrv-os.elf
	qemu-system-riscv64 -M virt -m 128M -nographic -bios none -kernel tinyrv-os.elf

.PHONY: all clean dump run
