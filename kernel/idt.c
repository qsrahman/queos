#include <gdt.h>
#include <idt.h>
#include <picirq.h>
#include <string.h>
#include <console.h>

static const char* exception_msg[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

static idt_entry_t idt[256];
static isr_t interrupt_handlers[256];

void set_idt_gate(uchar num, uint base, ushort sel, uchar flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;

    idt[num].sel = sel;
    idt[num].always0 = 0;

    // We must uncomment the OR below when we get to using user-mode.
    // It sets the interrupt gate's privilege level to 3.
    idt[num].flags = flags /* | 0x60 */;
}

void init_idt(void) {
    init_pic();

    memset(&idt, 0, sizeof(idt_entry_t) * 256);

    // Note: Interrupt gates clear the IF bit in EFLAGS, while trap gates do
    // not. 0x8E = 1 00 0 0110 (386 Interrupt Gate) 0x8F = 1 00 0 1111 (386 Trap
    // Gate)
    set_idt_gate(0, (uint)isr0, SEG_KCODE << 3, 0x8E);
    set_idt_gate(1, (uint)isr1, SEG_KCODE << 3, 0x8E);
    set_idt_gate(2, (uint)isr2, SEG_KCODE << 3, 0x8E);
    set_idt_gate(3, (uint)isr3, SEG_KCODE << 3, 0x8E);
    set_idt_gate(4, (uint)isr4, SEG_KCODE << 3, 0x8E);
    set_idt_gate(5, (uint)isr5, SEG_KCODE << 3, 0x8E);
    set_idt_gate(6, (uint)isr6, SEG_KCODE << 3, 0x8E);
    set_idt_gate(7, (uint)isr7, SEG_KCODE << 3, 0x8E);
    set_idt_gate(8, (uint)isr8, SEG_KCODE << 3, 0x8E);
    set_idt_gate(9, (uint)isr9, SEG_KCODE << 3, 0x8E);
    set_idt_gate(10, (uint)isr10, SEG_KCODE << 3, 0x8E);
    set_idt_gate(11, (uint)isr11, SEG_KCODE << 3, 0x8E);
    set_idt_gate(12, (uint)isr12, SEG_KCODE << 3, 0x8E);
    set_idt_gate(13, (uint)isr13, SEG_KCODE << 3, 0x8E);
    set_idt_gate(14, (uint)isr14, SEG_KCODE << 3, 0x8E);
    set_idt_gate(15, (uint)isr15, SEG_KCODE << 3, 0x8E);
    set_idt_gate(16, (uint)isr16, SEG_KCODE << 3, 0x8E);
    set_idt_gate(17, (uint)isr17, SEG_KCODE << 3, 0x8E);
    set_idt_gate(18, (uint)isr18, SEG_KCODE << 3, 0x8E);
    set_idt_gate(19, (uint)isr19, SEG_KCODE << 3, 0x8E);
    set_idt_gate(20, (uint)isr20, SEG_KCODE << 3, 0x8E);
    set_idt_gate(21, (uint)isr21, SEG_KCODE << 3, 0x8E);
    set_idt_gate(22, (uint)isr22, SEG_KCODE << 3, 0x8E);
    set_idt_gate(23, (uint)isr23, SEG_KCODE << 3, 0x8E);
    set_idt_gate(24, (uint)isr24, SEG_KCODE << 3, 0x8E);
    set_idt_gate(25, (uint)isr25, SEG_KCODE << 3, 0x8E);
    set_idt_gate(26, (uint)isr26, SEG_KCODE << 3, 0x8E);
    set_idt_gate(27, (uint)isr27, SEG_KCODE << 3, 0x8E);
    set_idt_gate(28, (uint)isr28, SEG_KCODE << 3, 0x8E);
    set_idt_gate(29, (uint)isr29, SEG_KCODE << 3, 0x8E);
    set_idt_gate(30, (uint)isr30, SEG_KCODE << 3, 0x8E);
    set_idt_gate(31, (uint)isr31, SEG_KCODE << 3, 0x8E);

    set_idt_gate(IRQ0, (uint)irq0, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ1, (uint)irq1, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ2, (uint)irq2, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ3, (uint)irq3, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ4, (uint)irq4, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ5, (uint)irq5, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ6, (uint)irq6, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ7, (uint)irq7, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ8, (uint)irq8, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ9, (uint)irq9, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ10, (uint)irq10, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ11, (uint)irq11, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ12, (uint)irq12, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ13, (uint)irq13, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ14, (uint)irq14, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ15, (uint)irq15, SEG_KCODE << 3, 0x8E);

    // 0xEE = 1 11 0 0110 (386 Interrupt Gate DPL=3)
    // Flags are set so that interrupt gate is set to DPL = 3
    set_idt_gate(ISR_SYSCALL, (uint)isr64, SEG_KCODE << 3, 0x8E | 0x60);

    lidt(idt, sizeof(idt));

    // Nullify all the interrupt handlers.
    memset(&interrupt_handlers, 0, sizeof(isr_t) * 256);
}

void register_interrupt_handler(uchar n, isr_t handler) {
    interrupt_handlers[n] = handler;
    if (n >= IRQ0 && n <= IRQ15) {
        pic_enable_irq(n);
    }
}

void unregister_interrupt_handler(uchar n) {
    interrupt_handlers[n] = NULL;
    if (n >= IRQ0 && n <= IRQ15) {
        pic_disable_irq(n);
    }
}

void syscall_handler(registers_t* regs) {
    (void)regs;
    kprintf("syscall received.\n");
}

void isr_handler(registers_t* regs) {
    isr_t handler = interrupt_handlers[regs->int_no];

    if (handler) {
        if (regs->int_no == ISR_SYSCALL) {
            syscall_handler(regs);
        } else {
            handler(regs);
        }
    } else if (regs->int_no < IRQ0) {
        kprintf("%s (0x%X) Exception. HALT!\n", exception_msg[regs->int_no], regs->int_no);
        while (1);
    } else {
        kprintf("ISR (0x%X) received!\n", regs->int_no);
    }
}

void irq_handler(registers_t* regs) {
    // Send an EOI (end of interrupt) signal to the PICs.
    pic_eoi(regs->int_no);

    isr_t handler = interrupt_handlers[regs->int_no];

    if (handler) {
        handler(regs);
    } else {
        kprintf("IRQ (0x%X) received!\n", regs->int_no - IRQ0);
    }
}
