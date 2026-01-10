bits 32                         ; All instructions should be 32-bit.

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

section .text

align 4
global mboot                    ; Make 'mboot' accessible from C.
mboot:
    dd MBOOT_HEADER_MAGIC       ; GRUB will search for this value on each
                                ; 4-byte boundary in your kernel file
    dd MBOOT_HEADER_FLAGS       ; How GRUB should load your file / settings
    dd MBOOT_CHECKSUM           ; To ensure that the above values are correct

global _start                   ; Kernel entry point.
_start:
    mov ebp, 0                  ; nuke the stack frame
    mov esp, kstack_top         ; This points the stack to our new stack area

    ; transfer multiboot information to kmain
    push ebx

    extern kmain                ; This is the entry point of our C code
    call kmain                  ; call our main() function.

    cli
    hlt
    jmp $                       ; Enter an infinite loop, to stop the processor
                                ; executing whatever rubbish is in the memory
                                ; after our kernel!
section .bss
align 16
    resb 4096               ; This reserves 4KiB of memory here
global kstack_top
kstack_top:
