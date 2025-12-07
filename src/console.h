#ifndef CONSOLE_H
#define CONSOLE_H

#include "common.h"

void console_init(void);
void console_putc(char c);
void console_write(const char *s);
void console_readline(char *buf, size_t max);

#endif /* CONSOLE_H */
