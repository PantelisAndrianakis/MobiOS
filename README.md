```
  ███╗   ███╗ ██████╗ ██████╗ ██╗ ██████╗ ███████╗  
  ████╗ ████║██╔═══██╗██╔══██╗██║██╔═══██╗██╔════╝  
  ██╔████╔██║██║   ██║██████╔╝██║██║   ██║███████╗  
  ██║╚██╔╝██║██║   ██║██╔══██╗██║██║   ██║╚════██║  
  ██║ ╚═╝ ██║╚██████╔╝██████╔╝██║╚██████╔╝███████║  
  ╚═╝     ╚═╝ ╚═════╝ ╚═════╝ ╚═╝ ╚═════╝ ╚══════╝  

         A Lightweight x64 Operating System         
```

# MobiOS

MobiOS is a simple 64-bit operating system written in C and Assembly.

## Coding Style

MobiOS uses the Allman coding style, which places opening braces on a new line. This enhances readability by clearly delineating code blocks:

```c
// Example of Allman style
void function_name(void)
{
    if (condition)
    {
        // code
    }
}
```

The project also prioritizes moving code from assembly to C where possible, to improve:
- Readability
- Maintainability
- Portability
- Debugging capabilities

Hardware operations that were previously inline assembly are now implemented as proper C functions, though they still use minimal assembly instructions internally when necessary.

## Project Structure

```
/
├── boot/               # Boot-related files
│   └── grub/           # GRUB bootloader configuration
│       ├── grub.cfg    # Main GRUB configuration
│       ├── INSTALL.md  # Installation instructions
│       └── themes/     # GRUB themes
│           └── mobios/ # MobiOS theme
├── kernel/             # Kernel source code
│   ├── build/          # Build output directory
│   ├── include/        # Header files
│   │   ├── console.h   # Console/display functions
│   │   ├── kernel.h    # Main kernel header
│   │   └── types.h     # Type definitions
│   ├── Makefile        # Build configuration
│   └── src/            # Source code
│       ├── asm/        # Assembly code
│       │   └── boot.asm # Kernel entry point
│       ├── kernel/     # C kernel code
│       │   ├── console.c # Console/screen functions
│       │   ├── hardware.c # Hardware abstraction functions
│       │   ├── main.c    # C entry point
│       │   └── memory.c  # Memory management
│       └── linker.ld   # Linker script
└── README.md           # This file
```

## Prerequisites

- **NASM**: The Netwide Assembler (version 2.15+ recommended)
- **GCC**: GNU Compiler Collection for C code
- **GNU Binutils**: For linking (LD)
- **GRUB2**: For bootloader
- **QEMU**: For testing (qemu-system-x86_64)
- **xorriso**: For ISO image creation

# Running MobiOS in QEMU on Ubuntu

This document provides step-by-step instructions for building and running MobiOS in QEMU on Ubuntu.

## Prerequisites

Ensure you have the following dependencies installed:

```bash
sudo apt update
sudo apt install nasm gcc binutils grub-common grub-pc-bin xorriso qemu-system-x86
```

You can verify the installation with:

```bash
which nasm gcc ld grub-mkrescue qemu-system-x86_64 xorriso
```

## Building MobiOS

1. Navigate to the kernel directory:
   ```bash
   cd kernel
   ```

2. Build the kernel:
   ```bash
   make
   ```
   This compiles the kernel and produces `build/mobios.bin`.

3. Create a bootable ISO:
   ```bash
   make iso
   ```
   This creates `mobios.iso` which contains the kernel and GRUB bootloader.

## Running MobiOS in QEMU

To run MobiOS in QEMU:

```bash
make qemu
```

This will launch QEMU with the bootable ISO. You should see:
1. The GRUB bootloader menu
2. Select "MobiOS 64-bit"
3. The MobiOS logo will appear in ASCII art

## Debugging with GDB

To debug MobiOS in QEMU with GDB:

```bash
# Terminal 1: Start QEMU with GDB server
make qemu-debug

# Terminal 2: Connect GDB
gdb -ex "target remote localhost:1234" -ex "symbol-file build/mobios.bin"
```

## Alternative Direct Kernel Testing

For direct kernel testing without creating an ISO:

```bash
make qemu-direct
```

## Cleaning Build Artifacts

To clean build artifacts:

```bash
make clean
```

## Development

MobiOS is in early development. Contributions are welcome!

### Roadmap

- [x] GRUB bootloader setup
- [x] Basic 64-bit kernel implementation
- [x] C language integration
- [x] Basic console/screen functions in C
- [x] Simple memory management functions
- [ ] Advanced memory management (paging, allocation)
- [ ] Process scheduling
- [ ] Device drivers
- [ ] File system

## License

This project is open source and available under the MIT License.
