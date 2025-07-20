// kernel/src/kernel/main.c
// NOTE: This code follows Allman style - all opening braces on new lines

#include "kernel.h"
#include "console.h"
#include "keyboard.h"
#include "shell.h"

// Entry point called from assembly
void kernel_main(void)
{
    // Initialize console/screen
    console_init();
    
    // Initialize keyboard
    keyboard_init();
    
    // Initialize shell
    shell_init();
    
    // Display welcome message
    console_write_line("MobiOS v0.1");
    console_write_line("Boot disk ready");
    console_putchar('\n');
    
    // Start the shell
    shell_run();
    
    // Infinite loop - should never get here
    while(1)
    {
        cpu_halt();
    }
}