ENTRY(_start)
OUTPUT_ARCH(i386)
OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")

SECTIONS {
    /* Link the kernel at this address: "." means the current address */
    . = 0x100000;           /* the code should be loaded at 1 MB */

    /* AT(...) gives the load address of this section, which tells
       the boot loader where to load the kernel in physical memory
    */
    .text :  AT(0x100000) {
        *(.text*)           /* all text sections from all files */
        *(.rodata*)         /* all read-only data sections from all files */
    }

    . = ALIGN(0x1000);
    _etext = .;

    .data : {
        *(.data*)           /* all data sections from all files */
    }

    _edata = .;
    .bss : {
        *(COMMON)           /* all COMMON sections from all files */
        *(.bss*)            /* all bss sections from all files */
    }
    
    . = ALIGN(0x1000);
    _end = .;
}
