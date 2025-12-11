#pragma once

int kprintf(const char* format, ...);

void cons_putc(int c);
void cons_puts(const char* s);
int cons_getc(void);
void console_isr(int (*getc)(void));
void init_console(void);
void cga_cls(void);
