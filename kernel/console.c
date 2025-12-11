#include <x86.h>
#include <console.h>
#include <uart.h>
#include <string.h>

#define INPUT_BUF 128
#define BACKSPACE 0x100
#define CRTPORT 0x3D4

#define C(x) ((x) - '@')  // Control-x
static ushort* crt = 0;
static ushort attrib = 0x0F;  // black on white

struct {
    char buf[INPUT_BUF];
    uint r;  // Read index
    uint w;  // Write index
    uint e;  // Edit index
} input;

static void cga_putc(int c) {
    int pos;

    // Cursor position: col + 80 * row.
    outb(CRTPORT, 14);
    pos = inb(CRTPORT + 1) << 8;
    outb(CRTPORT, 15);
    pos |= inb(CRTPORT + 1);

    if (c == '\n')
        pos += 80 - pos % 80;
    else if (c == BACKSPACE) {
        if (pos > 0)
            --pos;
    } else
        crt[pos++] = (c & 0xFF) | (attrib << 8);  // black on white

    if ((pos / 80) >= 24) {  // Scroll up.
        memmove(crt, crt + 80, sizeof(crt[0]) * 23 * 80);
        pos -= 80;
        memset(crt + pos, 0, sizeof(crt[0]) * (24 * 80 - pos));
    }

    outb(CRTPORT, 14);
    outb(CRTPORT + 1, pos >> 8);
    outb(CRTPORT, 15);
    outb(CRTPORT + 1, pos);
    crt[pos] = ' ' | (attrib << 8);
}

// Clears the screen, by putting lots of spaces into the framebuffer.
void cga_cls(void) {
    for (int i = 0; i < 80 * 25; i++) {
        crt[i] = ' ' | (attrib << 8);
    }
    // Move the hardware cursor [0, 0].
    outb(CRTPORT, 14);
    outb(CRTPORT + 1, 0);
    outb(CRTPORT, 15);
    outb(CRTPORT + 1, 0);
}

// Sets the forecolor and backcolor that we will use
void cga_set_color(uchar fg, uchar bg) {
    // top 4 bits are the background color,
    // bottom 4 bits are the foreground color
    attrib = (bg << 4) | (fg & 0x0F);
}

void cons_putc(int c) {
    if (c == BACKSPACE) {
        uart_putc('\b');
        uart_putc(' ');
        uart_putc('\b');
    } else
        uart_putc(c);
    cga_putc(c);
}

// Outputs a null-terminated ASCII string to the monitor.
void cons_puts(const char* str) {
    while (*str) {
        cons_putc(*str++);
    }
}

// return the next input character from the console, or 0 if none waiting
int cons_getc(void) {
    // grab the next character from the input buffer.
    if (input.r != input.w) {
        int c = input.buf[input.r++];
        if (input.r == INPUT_BUF)
            input.r = 0;
        return c;
    }
    return 0;
}

void console_isr(int (*getc)(void)) {
    int c;

    while ((c = getc()) >= 0) {
        switch (c) {
            case C('U'):  // Kill line.
                while (input.e != input.w &&
                       input.buf[(input.e - 1) % INPUT_BUF] != '\n') {
                    input.e--;
                    cons_putc(BACKSPACE);
                }
                break;
            case C('H'):
            case '\x7f':  // Backspace
                if (input.e != input.w) {
                    input.e--;
                    cons_putc(BACKSPACE);
                }
                break;
            default:
                if (c != 0 && input.e - input.r < INPUT_BUF) {
                    c = (c == '\r') ? '\n' : c;
                    input.buf[input.e++ % INPUT_BUF] = c;
                    cons_putc(c);
                    if (c == '\n' || c == C('D') ||
                        input.e == input.r + INPUT_BUF) {
                        input.w = input.e;
                    }
                }
                break;
        }
    }
}

void init_console(void) {
    crt = (ushort*)0xB8000;  // CGA memory
    cga_cls();
}
