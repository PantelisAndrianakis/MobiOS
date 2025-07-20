# MobiOS Kernel Makefile (x64 version with C support)

# Compiler/Assembler/Linker settings
AS = nasm
ASFLAGS = -f elf64
CC = gcc
CFLAGS = -m64 -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -Wall -Wextra -Werror -c
LD = ld
LDFLAGS = -T src/linker.ld -m elf_x86_64 --oformat=elf64-x86-64 -nostdlib

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Source files
ASM_SOURCES = $(wildcard $(SRC_DIR)/asm/*.asm)
ASM_OBJECTS = $(patsubst $(SRC_DIR)/asm/%.asm,$(BUILD_DIR)/%.o,$(ASM_SOURCES))
C_SOURCES = $(wildcard $(SRC_DIR)/kernel/*.c)
C_OBJECTS = $(patsubst $(SRC_DIR)/kernel/%.c,$(BUILD_DIR)/%.o,$(C_SOURCES))

# Output kernel binary
KERNEL = $(BUILD_DIR)/mobios.bin

# Default target
.PHONY: all clean

all: $(BUILD_DIR) $(KERNEL)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build kernel binary
$(KERNEL): $(ASM_OBJECTS) $(C_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

# Compile assembly files
$(BUILD_DIR)/%.o: $(SRC_DIR)/asm/%.asm
	$(AS) $(ASFLAGS) -o $@ $<

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/kernel/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -o $@ $<

# Create ISO image with GRUB
iso: $(KERNEL)
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/mobios.bin
	cp ../boot/grub/grub.cfg iso/boot/grub/
	cp -r ../boot/grub/themes iso/boot/grub/
	
	# Create GRUB config specifically for the ISO
	echo "set timeout=3" > iso/boot/grub/grub.cfg
	echo "set default=0" >> iso/boot/grub/grub.cfg
	echo "menuentry \"MobiOS 64-bit\" {" >> iso/boot/grub/grub.cfg
	echo "    multiboot2 /boot/mobios.bin" >> iso/boot/grub/grub.cfg
	echo "    boot" >> iso/boot/grub/grub.cfg
	echo "}" >> iso/boot/grub/grub.cfg
	
	grub-mkrescue -o mobios.iso iso

# QEMU testing (using the ISO image)
qemu: iso
	qemu-system-x86_64 -cdrom mobios.iso -m 512M

qemu-debug: iso
	qemu-system-x86_64 -cdrom mobios.iso -m 512M -s -S

# Direct kernel testing (alternative method)
qemu-direct: $(KERNEL)
	qemu-system-x86_64 -machine q35 -kernel $(KERNEL) -cpu qemu64 -m 512M -no-reboot -append "console=ttyS0" -serial stdio

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)/* iso mobios.iso

# Help target
help:
	@echo "MobiOS Kernel Makefile (x64 version with C support)"
	@echo "------------------------------------------------"
	@echo "Targets:"
	@echo "  all        - Build the kernel (default)"
	@echo "  iso        - Create bootable ISO image (requires grub-mkrescue)"
	@echo "  qemu       - Run kernel in QEMU"
	@echo "  qemu-debug - Run kernel in QEMU with GDB server enabled"
	@echo "  clean      - Remove build artifacts"
	@echo "  help       - Display this help message"