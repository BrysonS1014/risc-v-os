#ifndef UART_H
#define UART_H

#include "common.h"

/* Very small polled UART driver for the ns16550a-compatible UART
 * provided by QEMU's RISC-V 'virt' machine.
 */

void uart_init(void);
void uart_putc(char c);
int  uart_getc(void);          /* non-blocking, returns -1 if nothing */
int  uart_getc_blocking(void); /* blocking */

#endif /* UART_H */
