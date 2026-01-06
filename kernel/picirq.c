// Intel 8259A programmable interrupt controllers.

#include <idt.h>
#include <picirq.h>
#include <x86.h>

// I/O Addresses of the two programmable interrupt controllers
#define IO_PIC1 0x20  // Master (IRQs 0-7)
#define IO_PIC2 0xA0  // Slave (IRQs 8-15)

#define IRQ_SLAVE 2  // IRQ at which slave connects to master

#define EOI 0x20      // End-of-interrupt command code
#define READ_IRR 0x0a /* OCW3 irq ready next CMD read */
#define READ_ISR 0x0b /* OCW3 irq service next CMD read */

// Current IRQ mask.
// Initial IRQ mask has interrupt 2 enabled (for slave 8259A).
static uint16_t irqmask = 0xFFFF & ~(1 << IRQ_SLAVE);

static void pic_set_mask(uint16_t mask) {
    irqmask = mask;
    outb(IO_PIC1 + 1, mask);
    outb(IO_PIC2 + 1, mask >> 8);
}

void pic_enable_irq(int irq) {
    pic_set_mask(irqmask & ~(1 << (irq - IRQ0)));
}

void pic_disable_irq(int irq) {
    pic_set_mask(irqmask | (1 << (irq - IRQ0)));
}

static uint16_t pic_get_irq_reg(int ocw3) {
    // OCW3 to PIC CMD to get the register values.  PIC2 is chained, and
    // represents IRQs 8-15.  PIC1 is IRQs 0-7, with 2 being the chain
    outb(IO_PIC1, ocw3);
    outb(IO_PIC2, ocw3);
    return (inb(IO_PIC2) << 8) | inb(IO_PIC1);
}

// Returns the combined value of the cascaded PICs irq request register
uint16_t pic_get_irr(void) {
    return pic_get_irq_reg(READ_IRR);
}

// Returns the combined value of the cascaded PICs in-service register
uint16_t pic_get_isr(void) {
    return pic_get_irq_reg(READ_ISR);
}

// returns true if irq is spurious irq
int pic_spurious_irq(int irq) {
    // For a spurious IRQ, there is no real IRQ and the PIC chip's ISR (In
    // Service Register) flag for the corresponding IRQ will not be set.
    return !(pic_get_isr() & (1 << (irq - IRQ0)));
}

void pic_eoi(int irq) {
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (irq >= IRQ8) {
        // Send reset signal to slave.
        outb(IO_PIC2, EOI);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(IO_PIC1, EOI);
}

// Initialize the 8259A interrupt controllers.
void init_pic(void) {
    // mask all interrupts
    outb(IO_PIC1 + 1, 0xFF);
    outb(IO_PIC2 + 1, 0xFF);

    // Set up master (8259A-1)

    // ICW1:  0001g0hi
    //    g:  0 = edge triggering, 1 = level triggering
    //    h:  0 = cascaded PICs, 1 = master only
    //    i:  0 = no ICW4, 1 = ICW4 required
    outb(IO_PIC1, 0x11);

    // ICW2:  Vector offset
    outb(IO_PIC1 + 1, IRQ0);

    // ICW3:  (master PIC) bit mask of IR lines connected to slaves
    //        (slave PIC) 3-bit # of slave's connection to master
    outb(IO_PIC1 + 1, 1 << IRQ_SLAVE);

    // ICW4:  000nbmap
    //    n:  1 = special fully nested mode
    //    b:  1 = buffered mode
    //    m:  0 = slave PIC, 1 = master PIC
    //      (ignored when b is 0, as the master/slave role
    //      can be hardwired).
    //    a:  1 = Automatic EOI mode
    //    p:  0 = MCS-80/85 mode, 1 = intel x86 mode
    outb(IO_PIC1 + 1, 0x3);

    // Set up slave (8259A-2)
    outb(IO_PIC2, 0x11);           // ICW1
    outb(IO_PIC2 + 1, IRQ8);       // ICW2
    outb(IO_PIC2 + 1, IRQ_SLAVE);  // ICW3
    // NB Automatic EOI mode doesn't tend to work on the slave.
    // Linux source code says it's "to be investigated".
    outb(IO_PIC2 + 1, 0x3);  // ICW4

    // OCW3:  0ef01prs
    //   ef:  0x = NOP, 10 = clear specific mask, 11 = set specific mask
    //    p:  0 = no polling, 1 = polling mode
    //   rs:  0x = NOP, 10 = read IRR, 11 = read ISR
    outb(IO_PIC1, 0x68);  // clear specific mask
    outb(IO_PIC1, 0x0a);  // read IRR by default

    outb(IO_PIC2, 0x68);  // OCW3
    outb(IO_PIC2, 0x0a);  // OCW3

    if (irqmask != 0xFFFF) {
        pic_set_mask(irqmask);
    }
}
