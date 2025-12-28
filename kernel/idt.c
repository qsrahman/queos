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
static isr_t isr_handlers[256];

void set_idt_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
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
    set_idt_gate(0, (uint32_t)isr0, SEG_KCODE << 3, 0x8E);
    set_idt_gate(1, (uint32_t)isr1, SEG_KCODE << 3, 0x8E);
    set_idt_gate(2, (uint32_t)isr2, SEG_KCODE << 3, 0x8E);
    set_idt_gate(3, (uint32_t)isr3, SEG_KCODE << 3, 0x8E);
    set_idt_gate(4, (uint32_t)isr4, SEG_KCODE << 3, 0x8E);
    set_idt_gate(5, (uint32_t)isr5, SEG_KCODE << 3, 0x8E);
    set_idt_gate(6, (uint32_t)isr6, SEG_KCODE << 3, 0x8E);
    set_idt_gate(7, (uint32_t)isr7, SEG_KCODE << 3, 0x8E);
    set_idt_gate(8, (uint32_t)isr8, SEG_KCODE << 3, 0x8E);
    set_idt_gate(9, (uint32_t)isr9, SEG_KCODE << 3, 0x8E);
    set_idt_gate(10, (uint32_t)isr10, SEG_KCODE << 3, 0x8E);
    set_idt_gate(11, (uint32_t)isr11, SEG_KCODE << 3, 0x8E);
    set_idt_gate(12, (uint32_t)isr12, SEG_KCODE << 3, 0x8E);
    set_idt_gate(13, (uint32_t)isr13, SEG_KCODE << 3, 0x8E);
    set_idt_gate(14, (uint32_t)isr14, SEG_KCODE << 3, 0x8E);
    set_idt_gate(15, (uint32_t)isr15, SEG_KCODE << 3, 0x8E);
    set_idt_gate(16, (uint32_t)isr16, SEG_KCODE << 3, 0x8E);
    set_idt_gate(17, (uint32_t)isr17, SEG_KCODE << 3, 0x8E);
    set_idt_gate(18, (uint32_t)isr18, SEG_KCODE << 3, 0x8E);
    set_idt_gate(19, (uint32_t)isr19, SEG_KCODE << 3, 0x8E);
    set_idt_gate(20, (uint32_t)isr20, SEG_KCODE << 3, 0x8E);
    set_idt_gate(21, (uint32_t)isr21, SEG_KCODE << 3, 0x8E);
    set_idt_gate(22, (uint32_t)isr22, SEG_KCODE << 3, 0x8E);
    set_idt_gate(23, (uint32_t)isr23, SEG_KCODE << 3, 0x8E);
    set_idt_gate(24, (uint32_t)isr24, SEG_KCODE << 3, 0x8E);
    set_idt_gate(25, (uint32_t)isr25, SEG_KCODE << 3, 0x8E);
    set_idt_gate(26, (uint32_t)isr26, SEG_KCODE << 3, 0x8E);
    set_idt_gate(27, (uint32_t)isr27, SEG_KCODE << 3, 0x8E);
    set_idt_gate(28, (uint32_t)isr28, SEG_KCODE << 3, 0x8E);
    set_idt_gate(29, (uint32_t)isr29, SEG_KCODE << 3, 0x8E);
    set_idt_gate(30, (uint32_t)isr30, SEG_KCODE << 3, 0x8E);
    set_idt_gate(31, (uint32_t)isr31, SEG_KCODE << 3, 0x8E);

    set_idt_gate(IRQ0, (uint32_t)irq0, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ1, (uint32_t)irq1, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ2, (uint32_t)irq2, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ3, (uint32_t)irq3, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ4, (uint32_t)irq4, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ5, (uint32_t)irq5, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ6, (uint32_t)irq6, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ7, (uint32_t)irq7, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ8, (uint32_t)irq8, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ9, (uint32_t)irq9, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ10, (uint32_t)irq10, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ11, (uint32_t)irq11, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ12, (uint32_t)irq12, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ13, (uint32_t)irq13, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ14, (uint32_t)irq14, SEG_KCODE << 3, 0x8E);
    set_idt_gate(IRQ15, (uint32_t)irq15, SEG_KCODE << 3, 0x8E);

    // 0xEE = 1 11 0 0110 (386 Interrupt Gate DPL=3)
    // Flags are set so that interrupt gate is set to DPL = 3
    set_idt_gate(ISR_SYSCALL, (uint32_t)isr64, SEG_KCODE << 3, 0x8E | 0x60);

    lidt(idt, sizeof(idt));

    // Nullify all the interrupt handlers.
    memset(&isr_handlers, 0, sizeof(isr_t) * 256);
}

void register_isr_handler(uint8_t n, isr_t handler) {
    isr_handlers[n] = handler;
    if (n >= IRQ0 && n <= IRQ15) {
        pic_enable_irq(n);
    }
}

void unregister_isr_handler(uint8_t n) {
    isr_handlers[n] = NULL;
    if (n >= IRQ0 && n <= IRQ15) {
        pic_disable_irq(n);
    }
}

void syscall_handler(registers_t* regs) {
    (void)regs;
    kprintf("syscall received.\n");
}

void isr_handler(registers_t* regs) {
    isr_t handler = isr_handlers[regs->int_no];

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

    isr_t handler = isr_handlers[regs->int_no];

    if (handler) {
        handler(regs);
    } else {
        kprintf("IRQ (0x%X) received!\n", regs->int_no - IRQ0);
    }
}
