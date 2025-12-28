; The BIOS loads this code from the first sector of the disk into
; memory at physical address 0x7c00 and starts executing in real mode
; with cs=0 ip=7c00. we switch the CPU to 32-bit protected mode and
; jump into C.

bits 16

CODE_SEG    equ    gdt_code - gdt_start
DATA_SEG    equ    gdt_data - gdt_start

global start
start:
    cli                 ; Disable BIOS enabled interrupts
    ; Zero data segment registers DS, ES, and SS.
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax

    ; Physical address line A20 is tied to zero so that the old PCs 
    ; with 2 MB would run software that assumed 1 MB.  Undo that.
seta20_1:
    in al, 0x64          ; Wait for not busy
    test al, 0x02
    jnz seta20_1

    mov al, 0xd1         ; 0xd1 -> port 0x64
    out 0x64, al

seta20_2:
    in al, 0x64          ; Wait for not busy
    test al, 0x2
    jnz seta20_2

    mov al, 0xdf         ; 0xdf -> port 0x60
    out 0x60, al

    ; Switch from real to protected mode. Use a bootstrap GDT that makes
    ; virtual addresses map directly to physical addresses so that the
    ; effective memory map doesn't change during the transition.
    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax

    ; Complete the transition to 32-bit protected mode by using a long jmp
    ; to reload cs and eip.  The segment descriptors are set up with no
    ; translation, so that the mapping is still the identity mapping.
    jmp CODE_SEG:start32

bits 32
start32:
    ; Set up the protected-mode data segment registers
    mov eax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax
    xor eax, eax
    mov fs, ax
    mov gs, ax

    ; Set up the stack pointer and call into C.
    mov esp, start

    extern bootmain
    call bootmain

    jmp $

align 4
; Bootstrap GDT
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

; offset 0x8
gdt_code:           ; CS SHOULD POINT TO THIS
    dw 0xffff       ; Segment limit first 0-15 bits
    dw 0            ; Base first 0-15 bits
    db 0            ; Base 16-23 bits
    db 0x9a         ; Access byte
    db 11001111b    ; High 4 bit flags and the low 4 bit flags
    db 0            ; Base 24-31 bits

; offset 0x10
gdt_data:           ; DS, SS, ES, FS, GS
    dw 0xffff       ; Segment limit first 0-15 bits
    dw 0            ; Base first 0-15 bits
    db 0            ; Base 16-23 bits
    db 0x92         ; Access byte
    db 11001111b    ; High 4 bit flags and the low 4 bit flags
    db 0            ; Base 24-31 bits

gdt_end:

gdt_desc:
    dw gdt_end - gdt_start - 1
    dd gdt_start
