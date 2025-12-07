#include "console.h"
#include "uart.h"
#include "kstring.h"

void console_init(void)
{
    uart_init();
}

void console_putc(char c)
{
    if (c == '\n') {
        uart_putc('\r');
    }
    uart_putc(c);
}

void console_write(const char *s)
{
    if (!s) return;
    while (*s) {
        console_putc(*s++);
    }
}

void console_readline(char *buf, size_t max)
{
    size_t len = 0;
    if (!buf || max == 0) return;

    for (;;) {
        int ch = uart_getc_blocking();
        char c = (char)ch;

        if (c == '\r') c = '\n';

        if (c == '\n') {
            console_putc('\n');
            break;
        } else if (c == '\b' || c == 127) {
            /* Backspace */
            if (len > 0) {
                len--;
                console_write("\b \b");
            }
        } else if (len + 1 < max) {
            buf[len++] = c;
            console_putc(c);
        }
        /* If buffer is full, keep reading until newline to discard. */
    }

    buf[len] = '\0';
}
