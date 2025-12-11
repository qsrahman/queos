#include <x86.h>
#include <idt.h>
#include <timer.h>

static volatile uint ticks = 0;

static void timer_callback(registers_t* regs) {
    (void)regs;

    // Increment our 'tick count'
    ticks++;
}

void init_timer(uint frequency) {
    // Firstly, register our timer callback.
    register_interrupt_handler(IRQ0, timer_callback);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    uint divisor = 1193180 / frequency;

    // Send the command byte.
    outb(0x43, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    uchar l = (uchar)(divisor & 0xFF);
    uchar h = (uchar)((divisor >> 8) & 0xFF);

    // Send the frequency divisor.
    outb(0x40, l);
    outb(0x40, h);
}
