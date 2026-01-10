bits 32

section .text

; This macro creates a stub for an ISR which does NOT pass it's own
; error code (adds a dummy errcode byte).
%macro ISR_NOERRCODE 1
  global isr%1
  isr%1:
    push byte 0                 ; Push a dummy error code.
    push %1                     ; Push the interrupt number.
    jmp isr_common_stub         ; Go to our common handler code.
%endmacro

; This macro creates a stub for an ISR which passes it's own
; error code.
%macro ISR_ERRCODE 1
  global isr%1
  isr%1:
    push %1                ; Push the interrupt number
    jmp isr_common_stub
%endmacro

; This macro creates a stub for an IRQ - the first parameter is
; the IRQ number, the second is the ISR number it is remapped to.
%macro IRQ 2
  global irq%1
  irq%1:
    push byte 0
    push %2
    jmp irq_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE   17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_ERRCODE   21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

IRQ   0,    32                ; Timer
IRQ   1,    33                ; Keyboard
IRQ   2,    34                ; Cascade (used internally by the two PICs. never raised)
IRQ   3,    35                ; COM2
IRQ   4,    36                ; COM1
IRQ   5,    37                ; LPT2
IRQ   6,    38                ; Floppy disk
IRQ   7,    39                ; LPT1
IRQ   8,    40                ; CMOS real-time clock
IRQ   9,    41                ; Free
IRQ  10,    42                ; Free
IRQ  11,    43                ; Free
IRQ  12,    44                ; PS2 Mouse
IRQ  13,    45                ; FPU
IRQ  14,    46                ; Primary ATA Hard Disk
IRQ  15,    47                ; Secondary ATA Hard Disk

; system call handling exception
ISR_NOERRCODE 64

; In isr.c
extern isr_handler

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
    pusha         ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds    ; Lower 16-bits of eax = ds.
    push eax      ; save the data segment descriptor

    mov ax, 0x10  ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp  ; pass structure by reference, by value will break ABI spec
                  ; as callee is allowed to modify stack space reserved for
                  ; arguments
    push esp

    call isr_handler

    add esp, 4

    pop eax        ; reload the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa           ; Pops edi,esi,ebp...
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

; In isr.c
extern irq_handler

; This is our common IRQ stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
irq_common_stub:
    pusha         ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

    mov ax, ds    ; Lower 16-bits of eax = ds.
    push eax      ; save the data segment descriptor

    mov ax, 0x10  ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp
    push esp

    call irq_handler

    add esp, 4

global task_ret
task_ret:          ; void task_ret(void); 
    pop eax        ; reload the original data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa           ; Pops edi,esi,ebp...
    add esp, 8     ; Cleans up the pushed error code and pushed ISR number
    iret           ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

; Context switch
;
;   void switch_ctx(context_t **old, context_t *new);
;
; Save current register context in old context
; and then load register context from new context.
global switch_ctx
switch_ctx:
    mov eax, [esp + 4]      ; old context
    mov edx, [esp + 8]      ; new context

    ; If current task is trying to switch to itself then, that will
    ; not work as, context would be pushed to its stack, but restored
    ; from older stack pointer reference from second argument.
    ; Ideally this should be handled in scheduler code itself, but
    ; this is just to be on safer side.
    cmp [eax], edx
    je .exit

    ; Save old callee-save registers
    push ebp
    push ebx
    push esi
    push edi

    ; Switch stacks
    mov [eax], esp
    mov esp, edx

    ; Load new callee-save registers
    pop edi
    pop esi
    pop ebx
    pop ebp
.exit:
    ret

