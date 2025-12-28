#pragma once

#include <stdint.h>

// Intrrupts 0-31 are reserved by intel for use as exceptions
#define IRQ0 32   // Timer
#define IRQ1 33   // Keyboard
#define IRQ2 34   // CSD
#define IRQ3 35   // COM2
#define IRQ4 36   // COM1
#define IRQ5 37   // LPT2
#define IRQ6 38   // Floppy
#define IRQ7 39   // LPT1
#define IRQ8 40   // RCLK
#define IRQ9 41   // Free
#define IRQ10 42  // Free
#define IRQ11 43  // Free
#define IRQ12 44  // PS2
#define IRQ13 45  // FPU
#define IRQ14 46  // IDE1
#define IRQ15 47  // IDE2

#define ISR_SYSCALL 64

// A struct describing an interrupt gate.
typedef struct idt_entry {
    uint16_t base_lo;  // The lower 16 bits of the address to jump to when this
                       // interrupt fires.
    uint16_t sel;      // Kernel segment selector.
    uint8_t always0;   // This must always be zero.
    uint8_t flags;     // More flags. See documentation.
    uint16_t base_hi;  // The upper 16 bits of the address to jump to.
} __attribute__((packed)) idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
typedef struct idt_ptr {
    uint32_t base;  // The address of the first element in our idt_entry_t array.
    uint16_t limit;
} __attribute__((packed)) idt_ptr_t;

typedef struct registers {
    uint32_t ds;                                      // Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // Pushed by pusha.
    uint32_t int_no, err_code;                        // Interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, useresp, ss;            // Pushed by the processor automatically.
} __attribute__((packed)) registers_t;

static inline void lidt(idt_entry_t* p, int size) {
    volatile uint16_t idt_desc[3];

    idt_desc[0] = size - 1;
    idt_desc[1] = (uint32_t)p;
    idt_desc[2] = (uint32_t)p >> 16;

    __asm__ __volatile__("lidt (%0)"
                         :
                         : "r"(idt_desc));
}

void init_idt(void);
void set_idt_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// Enables registration of callbacks for interrupts or IRQs.
// For IRQs, to ease confusion, use the #defines above as the
// first parameter.
typedef void (*isr_t)(registers_t*);
void register_isr_handler(uint8_t n, isr_t handler);
void unregister_isr_handler(uint8_t n);

// These directives let us access the addresses of our ASM ISR handlers.
void isr0(void);
void isr1(void);
void isr2(void);
void isr3(void);
void isr4(void);
void isr5(void);
void isr6(void);
void isr7(void);
void isr8(void);
void isr9(void);
void isr10(void);
void isr11(void);
void isr12(void);
void isr13(void);
void isr14(void);
void isr15(void);
void isr16(void);
void isr17(void);
void isr18(void);
void isr19(void);
void isr20(void);
void isr21(void);
void isr22(void);
void isr23(void);
void isr24(void);
void isr25(void);
void isr26(void);
void isr27(void);
void isr28(void);
void isr29(void);
void isr30(void);
void isr31(void);

void irq0(void);
void irq1(void);
void irq2(void);
void irq3(void);
void irq4(void);
void irq5(void);
void irq6(void);
void irq7(void);
void irq8(void);
void irq9(void);
void irq10(void);
void irq11(void);
void irq12(void);
void irq13(void);
void irq14(void);
void irq15(void);

// system call interrupt
void isr64(void);
