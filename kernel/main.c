#include <x86.h>
#include <gdt.h>
#include <idt.h>
#include <kbd.h>
#include <uart.h>
#include <timer.h>
#include <stddef.h>
#include <monitor.h>
#include <console.h>

void kmain(void) {
    init_gdt();
    init_idt();
    init_console();
    init_timer(100);
    init_kbd();
    init_uart();

    sti();

    while (1) {
        monitor(NULL);
    }
}
