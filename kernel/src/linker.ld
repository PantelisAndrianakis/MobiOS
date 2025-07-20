/* MobiOS x64 Kernel Linker Script */

ENTRY(_start)

SECTIONS
{
    /* The kernel will be loaded at 1 MiB (conventional for many kernels) */
    . = 1M;

    /* Multiboot2 header first to ensure it's within the first 8K */
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot2)
        *(.multiboot)
        *(.text)
    }

    /* Read-only data */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
        *(.rodata.*)
    }

    /* Read-write data (initialized) */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
        *(.data.*)
    }

    /* Read-write data (uninitialized) and stack */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    }

    /* End of kernel marker */
    end = .;
}