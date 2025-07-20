; MobiOS Kernel Entry Point (x64 version)
; A minimal multiboot2-compliant x64 kernel that calls into C code

; Declare external C function
extern kernel_main

[BITS 32]                   ; Start in 32-bit mode (multiboot loads us in 32-bit mode)

; Multiboot2 header constants
MB2_MAGIC    equ 0xE85250D6  ; Multiboot2 magic number
MB2_ARCH     equ 0           ; Architecture 0 (32-bit protected mode i386)
MB2_LENGTH   equ header_end - header_start
MB2_CHECKSUM equ -(MB2_MAGIC + MB2_ARCH + MB2_LENGTH)

; Multiboot2 header
section .multiboot2
align 8
header_start:
    dd MB2_MAGIC            ; Magic number
    dd MB2_ARCH             ; Architecture
    dd MB2_LENGTH           ; Header length
    dd MB2_CHECKSUM         ; Checksum
    
    ; End tag
    dw 0                    ; Type
    dw 0                    ; Flags
    dd 8                    ; Size
header_end:

; Define constants for paging setup
PAGE_PRESENT    equ 1 << 0
PAGE_WRITE      equ 1 << 1
PAGE_SIZE_2MB   equ 1 << 7  ; For 2MB pages in PDT

; Reserve space for page tables
section .bss
align 4096
page_table_l4:      ; PML4 - Page Map Level 4 Table
    resb 4096
page_table_l3:      ; PDPT - Page Directory Pointer Table
    resb 4096
page_table_l2:      ; PDT - Page Directory Table
    resb 4096

; Kernel stack
align 16
stack_bottom:
    resb 16384      ; 16 KiB stack
stack_top:

; GDT for 64-bit mode
section .data
align 16
gdt64:
    ; Null descriptor
    dq 0
    ; Code segment
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)  ; Executable, code, present, 64-bit
gdt64_pointer:
    dw $ - gdt64 - 1    ; Size of the GDT
    dq gdt64            ; Address of the GDT

; MobiOS ASCII Art Logo with proper line endings
mobios_logo:
    db "  ███╗   ███╗ ██████╗ ██████╗ ██╗ ██████╗ ███████╗  ", 0x0D, 0x0A
    db "  ████╗ ████║██╔═══██╗██╔══██╗██║██╔═══██╗██╔════╝  ", 0x0D, 0x0A
    db "  ██╔████╔██║██║   ██║██████╔╝██║██║   ██║███████╗  ", 0x0D, 0x0A
    db "  ██║╚██╔╝██║██║   ██║██╔══██╗██║██║   ██║╚════██║  ", 0x0D, 0x0A
    db "  ██║ ╚═╝ ██║╚██████╔╝██████╔╝██║╚██████╔╝███████║  ", 0x0D, 0x0A
    db "  ╚═╝     ╚═╝ ╚═════╝ ╚═════╝ ╚═╝ ╚═════╝ ╚══════╝  ", 0x0D, 0x0A
    db 0x0D, 0x0A
    db "         A Lightweight x64 Operating System         ", 0x0D, 0x0A
    db 0

; Kernel entry point
section .text
global _start
_start:
    ; Set up the stack
    mov esp, stack_top

    ; Check for CPUID
    pushfd
    pop eax
    mov ecx, eax
    xor eax, 1 << 21
    push eax
    popfd
    pushfd
    pop eax
    push ecx
    popfd
    xor eax, ecx
    jz .no_cpuid

    ; Check for long mode
    mov eax, 0x80000000  ; Extended function
    cpuid
    cmp eax, 0x80000001  ; Is extended function 0x80000001 available?
    jb .no_long_mode     ; If not, long mode not supported

    ; Check if long mode is supported
    mov eax, 0x80000001  ; Extended function 0x80000001
    cpuid
    test edx, 1 << 29    ; Test LM bit
    jz .no_long_mode     ; If not set, long mode not supported

    ; Set up paging for long mode
    ; Initialize page tables to zero
    mov eax, 0
    mov ecx, 4096*3/4    ; Size of 3 page tables in dwords
    mov edi, page_table_l4
    rep stosd

    ; Set up mappings
    ; PML4 entry (level 4)
    mov eax, page_table_l3
    or eax, PAGE_PRESENT | PAGE_WRITE
    mov [page_table_l4], eax

    ; PDPT entry (level 3)
    mov eax, page_table_l2
    or eax, PAGE_PRESENT | PAGE_WRITE
    mov [page_table_l3], eax

    ; PDT entries (level 2) - identity map first 2MB
    mov eax, PAGE_PRESENT | PAGE_WRITE | PAGE_SIZE_2MB
    mov [page_table_l2], eax

    ; Load page table
    mov eax, page_table_l4
    mov cr3, eax

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5      ; Set PAE bit
    mov cr4, eax

    ; Set long mode bit in EFER MSR
    mov ecx, 0xC0000080 ; EFER MSR
    rdmsr
    or eax, 1 << 8      ; Set LM bit
    wrmsr

    ; Enable paging (and thus long mode)
    mov eax, cr0
    or eax, 1 << 31     ; Set PG bit
    mov cr0, eax

    ; Load 64-bit GDT
    lgdt [gdt64_pointer]

    ; Jump to 64-bit code segment
    jmp 0x8:long_mode_start

.no_cpuid:
.no_long_mode:
    ; Print "ERR" to screen if something fails
    mov dword [0xB8000], 0x4F524F45 ; "ER"
    mov dword [0xB8004], 0x4F3A4F52 ; "R:"
    hlt

[BITS 64]
long_mode_start:
    ; Initialize all segment registers
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Set up a proper stack for C function calls
    mov rsp, stack_top
    
    ; Call into C code
    call kernel_main
    
    ; If kernel_main returns, hang the CPU
    cli

hang:
    hlt                           ; Halt the CPU
    jmp hang                      ; If an interrupt somehow gets through, just hang again