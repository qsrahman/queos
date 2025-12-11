#include <x86.h>
#include <console.h>

void panic(const char* message, const char* file, int line) {
    // We encountered a massive problem and have to stop.
    cli();  // Disable interrupts.

    kprintf("PANIC(%s) at %s:%d\n", message, file, line);

    // Halt by going into an infinite loop.
    for (;;)
        ;
}

void panic_assert(const char* file, int line, const char* desc) {
    // An assertion failed, and we have to panic.
    cli();  // Disable interrupts.

    kprintf("ASSERTION-FAILED(%s) at %s:%d\n", desc, file, line);

    // Halt by going into an infinite loop.
    for (;;)
        ;
}
