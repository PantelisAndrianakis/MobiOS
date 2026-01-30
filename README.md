```
  ███╗   ███╗ ██████╗ ██████╗ ██╗ ██████╗ ███████╗  
  ████╗ ████║██╔═══██╗██╔══██╗██║██╔═══██╗██╔════╝  
  ██╔████╔██║██║   ██║██████╔╝██║██║   ██║███████╗  
  ██║╚██╔╝██║██║   ██║██╔══██╗██║██║   ██║╚════██║  
  ██║ ╚═╝ ██║╚██████╔╝██████╔╝██║╚██████╔╝███████║  
  ╚═╝     ╚═╝ ╚═════╝ ╚═════╝ ╚═╝ ╚═════╝ ╚══════╝  

         A Lightweight x64 Operating System         
```

## 🚀 What Makes MobiOS Special

MobiOS is a simple 64-bit operating system written in C and Assembly that can:
- ✅ **Read and write to real hard drives** via ATA disk driver
- ✅ **Access FAT32 file systems** on physical disks
- ✅ **Store files persistently** across reboots
- ✅ **Boot on real hardware** (not just emulators)
- ✅ **Handle hardware interrupts** (disk, keyboard, timer)

## 📋 Features

### Hardware Support
- **x86-64 Architecture** - Full 64-bit kernel
- **ATA/IDE Disk Driver** - Read/write to hard drives via PIO mode
- **Interrupt Handling** - IDT with 32 exception handlers + 16 IRQ handlers
- **PIC Configuration** - Programmable Interrupt Controller for hardware IRQs
- **PIT Timer** - 100Hz system timer
- **PS/2 Keyboard** - Full keyboard input with line editing
- **VGA Text Console** - 80x25 color text display

### File System
- **MBR Partition Table** - Read and parse partition tables
- **FAT32 Support** - Full FAT32 file system implementation
  - Read directories
  - Read file contents
  - Write file contents
  - Follow cluster chains
  - 8.3 filename support

### User Interface
- **Interactive Shell** - DOS-style command prompt
- **Line Editing** - Arrow keys, backspace, insert mode
- **Available Commands:**
  - `DIR` - List files in directory
  - `TYPE <file>` - Display file contents
  - `HELP` - Show available commands
  - `CLS` - Clear screen

### System Features
- **String Library** - strlen, strcmp, strcpy, strcat, itoa
- **Memory Management** - Basic heap allocator (kmalloc/kfree)
- **GRUB Bootloader** - Standard multiboot2 boot
- **Clean Architecture** - Modular driver system

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                         MobiOS                               │
├─────────────────────────────────────────────────────────────┤
│  Shell Layer (Commands: DIR, TYPE, HELP, CLS)               │
├─────────────────────────────────────────────────────────────┤
│  File System Layer (FAT32)                                   │
│  - Directory Navigation  - File Read/Write                   │
├─────────────────────────────────────────────────────────────┤
│  Partition Layer (MBR Parser)                                │
├─────────────────────────────────────────────────────────────┤
│  Disk Driver Layer (ATA PIO)                                │
│  - Sector Read  - Sector Write  - Drive Detection           │
├─────────────────────────────────────────────────────────────┤
│  Interrupt Layer (IDT, PIC, Timer)                           │
├─────────────────────────────────────────────────────────────┤
│  Hardware Layer (Port I/O, Console, Keyboard)                │
└─────────────────────────────────────────────────────────────┘
```

## 📁 Project Structure

```
MobiOS/
├── boot/                   # Bootloader configuration
│   └── grub/
│       ├── grub.cfg       # GRUB config
│       └── themes/        # Boot theme
├── kernel/
│   ├── include/           # Header files
│   │   ├── console.h
│   │   ├── keyboard.h
│   │   ├── idt.h         # Interrupt descriptor table
│   │   ├── pic.h         # Interrupt controller
│   │   ├── timer.h       # System timer
│   │   ├── string.h      # String library
│   │   ├── heap.h        # Memory allocator
│   │   ├── commands.h    # Shell commands
│   │   ├── drivers/
│   │   │   └── ata.h     # Disk driver
│   │   └── fs/
│   │       ├── mbr.h     # Partition table
│   │       └── fat32.h   # File system
│   ├── src/
│   │   ├── asm/
│   │   │   ├── boot.asm       # Kernel entry
│   │   │   └── interrupts.asm # ISR/IRQ stubs
│   │   ├── kernel/
│   │   │   ├── main.c         # Kernel main
│   │   │   ├── idt.c          # Interrupts
│   │   │   ├── pic.c          # PIC driver
│   │   │   ├── timer.c        # Timer driver
│   │   │   ├── console.c      # Display
│   │   │   ├── keyboard.c     # Input
│   │   │   ├── shell.c        # Shell
│   │   │   ├── commands.c     # Commands
│   │   │   ├── string.c       # String lib
│   │   │   ├── heap.c         # Allocator
│   │   │   ├── hardware.c     # Port I/O
│   │   │   └── memory.c       # Memory ops
│   │   ├── drivers/
│   │   │   └── ata.c          # Disk driver
│   │   ├── fs/
│   │   │   ├── mbr.c          # MBR parser
│   │   │   └── fat32.c        # FAT32 impl
│   │   └── linker.ld          # Linker script
│   └── Makefile
├── LICENSE.txt
├── README.md
```

## 🛠️ Prerequisites

### Ubuntu/Debian
```bash
sudo apt update
sudo apt install -y \
  nasm \
  gcc \
  binutils \
  grub-common \
  grub-pc-bin \
  xorriso \
  qemu-system-x86 \
  dosfstools \
  mtools
```

### Arch Linux
```bash
sudo pacman -S nasm gcc binutils grub xorriso qemu dosfstools mtools
```

## 🔨 Building MobiOS

```bash
# Clone the repository
git clone https://github.com/yourusername/MobiOS.git
cd MobiOS/kernel

# Build the kernel
make clean
make

# Create bootable ISO
make iso

# The following files are created:
# - build/mobios.bin  (kernel binary)
# - mobios.iso        (bootable ISO image)
```

## 🚀 Running MobiOS

### In QEMU (Emulator)

```bash
# Simple QEMU run with virtual disk
make qemu

# This will:
# 1. Create a 64MB FAT32 virtual disk (disk.img)
# 2. Boot MobiOS from ISO
# 3. Mount the virtual disk as hard drive
```

### Manual QEMU Commands

```bash
# Create virtual disk (first time only)
qemu-img create -f raw disk.img 64M
mkfs.fat -F32 disk.img

# Add test files to disk
echo "Hello from MobiOS!" > test.txt
mcopy -i disk.img test.txt ::

# Run with GUI
qemu-system-x86_64 -cdrom mobios.iso -hda disk.img -m 512M

# Run in console mode (no GUI)
qemu-system-x86_64 -cdrom mobios.iso -hda disk.img -m 512M \
  -nographic -serial stdio
```

### On Real Hardware

#### Create Bootable USB Drive

```bash
# WARNING: This will ERASE the USB drive!
# Replace /dev/sdX with your USB device

# Method 1: Direct ISO write
sudo dd if=mobios.iso of=/dev/sdX bs=4M status=progress
sync

# Method 2: Create FAT32 partition
sudo parted /dev/sdX mklabel msdos
sudo parted /dev/sdX mkpart primary fat32 1MiB 100%
sudo parted /dev/sdX set 1 boot on
sudo mkfs.fat -F32 /dev/sdX1

# Mount and install
sudo mkdir -p /mnt/mobios
sudo mount /dev/sdX1 /mnt/mobios
sudo mkdir -p /mnt/mobios/boot/grub
sudo cp build/mobios.bin /mnt/mobios/boot/
sudo cp ../boot/grub/grub.cfg /mnt/mobios/boot/grub/
sudo grub-install --target=i386-pc --boot-directory=/mnt/mobios/boot /dev/sdX
sudo umount /mnt/mobios

# Boot from USB
```

## 📖 Usage

Once MobiOS boots, you'll see:

```
MobiOS
Initializing...

Setting up interrupts... OK
Configuring PIC... OK
Initializing timer... OK
Enabling interrupts... OK
Initializing keyboard... OK
Initializing disk driver... OK
ATA: Primary master drive initialized
Reading MBR... OK
MBR: Read successfully
Looking for FAT32 partition... OK
MBR: Found FAT32 partition at index 0
Initializing FAT32... OK
FAT32: Initialized successfully
File system ready!

Type HELP for available commands

C:\>
```

### Available Commands

```bash
# List files
C:\> DIR

# Display file contents
C:\> TYPE readme.txt

# Show help
C:\> HELP

# Clear screen
C:\> CLS
```

## 🎯 Coding Style

MobiOS follows strict coding standards:

- **Allman Style** - Opening braces on new lines
- **Tabs** - Use tabs for indentation (not spaces)
- **Brackets** - Always use brackets, even for single statements
- **Comments** - All comment sentences end with a period

Example:
```c
void function_name(void)
{
	if (condition)
	{
		do_something();  // Single statement still needs brackets.
	}
	else
	{
		do_other();
	}
}
```

## 🧪 Testing

### Test Files on Virtual Disk

```bash
# Create test files
echo "This is a test file" > test.txt
echo "MobiOS can read FAT32!" > readme.txt

# Add to disk image
mcopy -i disk.img test.txt ::
mcopy -i disk.img readme.txt ::

# Verify
mdir -i disk.img ::

# Run MobiOS and use DIR and TYPE commands
```

## 🔍 Debugging

### Debug in QEMU with GDB

```bash
# Terminal 1: Start QEMU with GDB server
qemu-system-x86_64 -cdrom mobios.iso -hda disk.img -m 512M \
  -s -S -nographic

# Terminal 2: Connect GDB
gdb build/mobios.bin
(gdb) target remote localhost:1234
(gdb) continue
```

### View Boot Messages

All initialization steps print status messages:
- "OK" means success
- "FAILED" means error

If boot fails, check the last successful initialization step.

## 🛣️ Roadmap

### Completed ✅
- [x] 64-bit kernel
- [x] GRUB bootloader
- [x] VGA text console
- [x] PS/2 keyboard driver
- [x] Interrupt handling (IDT, PIC, Timer)
- [x] ATA disk driver (PIO mode)
- [x] MBR partition table support
- [x] FAT32 file system (read/write)
- [x] Interactive shell
- [x] Basic commands (DIR, TYPE, HELP, CLS)
- [x] String library
- [x] Memory allocator

### Planned 🚧
- [ ] Additional commands (CD, MKDIR, RMDIR, DEL, COPY)
- [ ] Long filename (LFN) support
- [ ] File creation and deletion
- [ ] Directory navigation
- [ ] DMA transfers for faster disk I/O
- [ ] AHCI driver (modern SATA)
- [ ] USB storage support
- [ ] ext2/ext4 file system
- [ ] Process scheduling
- [ ] User mode programs
- [ ] System calls
- [ ] Virtual memory (paging)

## 📄 License

This project is licensed under the MIT License - see [LICENSE.txt](LICENSE.txt) for details.

## 🙏 Acknowledgments

- GRUB bootloader project
- OSDev Wiki and community
- Intel x86 architecture documentation
- FAT32 specification by Microsoft
