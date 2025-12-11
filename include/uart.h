#pragma once

#include <idt.h>

void init_uart(void);
void uart_putc(int c);
void uart_isr(registers_t* regs);
