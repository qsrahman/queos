# Interrupt

When an interrupt fires the cpu automatically pushes eflags, cs, eip onto the stack. the iret instruction pops these values off the stack and returns the cpu to the state it was in originally. Also interrupt gates clear the IF flag while trap gate do not. If the interrupt is an exception, the CPU will push an error code onto the stack, as a doubleword. 

```c
int32_t ctx_switch(uint32_t **old_stack, uint32_t *new_stack) {
    int32_t ret;
    __asm__ __volatile__
    (
        "int $48"
        : "=a" (ret)
        : "0"(old_stack), "b"(new_stack)
        : "memory"    // the kernel dereferences pointer args
    );
    return ret;
}
```

```c
struct interrupt_frame {
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
};

```

```asm
    ;ss
    ;esp
    ;pushfd
    ;push cs
    ;push eip

    isr:
    push eax
    push ecx
    push edx
    push ebx
    ;push esp
    push ebp
    push esi
    push edi
    cld

    ; isr code here

    pop edi
    pop esi
    pop ebp
    ;pop esp
    pop ebx
    pop edx
    pop ecx
    pop eax

    iret

    ;pop eip
    ;pop cs
    ;popfd
    ;esp
    ;ss

Borlandc interrupt
    ;pushf
    ;push cs
    ;push ip

    push ax
    push bx
    push cx
    push dx
    push es
    push ds
    push si
    push di
    push bp

    pop bp
    pop di
    pop si
    pop ds
    pop es
    pop dx
    pop cx
    pop bx
    pop ax

    ;pop ip
    ;pop cs
    ;popf

    iret
```

