#include <x86.h>
#include <uart.h>
#include <console.h>

#define COM1 0x3f8

static int uart = 0;  // is there a uart?

void init_uart(void) {
    // Turn off the FIFO
    outb(COM1 + 2, 0);

    // 9600 baud, 8 data bits, 1 stop bit, parity off.
    outb(COM1 + 3, 0x80);  // Unlock divisor
    outb(COM1 + 0, 115200 / 9600);
    outb(COM1 + 1, 0);
    outb(COM1 + 3, 0x03);  // Lock divisor, 8 data bits.
    outb(COM1 + 4, 0);
    outb(COM1 + 1, 0x01);  // Enable receive interrupts.

    // If status is 0xFF, no serial port.
    if (inb(COM1 + 5) == 0xFF) return;

    uart = 1;

    // Acknowledge pre-existing interrupt conditions;
    inb(COM1 + 2);
    inb(COM1 + 0);

    register_isr_handler(IRQ4, uart_isr);

    // Announce that we're here.
    for (char* p = "QUEOS...\n"; *p; p++) {
        uart_putc(*p);
    }
}

// Spin for a given number of microseconds.
// On real hardware would want to tune this dynamically.
void micro_delay(int us) {
    (void)us;
}

void uart_putc(int c) {
    if (!uart) return;

    for (int i = 0; i < 128 && !(inb(COM1 + 5) & 0x20); i++) {
        micro_delay(10);
    }
    outb(COM1 + 0, c);
}

static int uart_getc(void) {
    if (!uart) return -1;
    if (!(inb(COM1 + 5) & 0x01)) return -1;
    return inb(COM1 + 0);
}

void uart_isr(registers_t* regs) {
    (void)regs;
    console_isr(uart_getc);
}
