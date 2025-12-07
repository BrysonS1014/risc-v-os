#include "uart.h"

/* UART0 is an ns16550a at 0x10000000 on QEMU 'virt'. */
#define UART0_BASE   0x10000000UL

#define UART_RBR     0x00    /* Receive buffer (read only) */
#define UART_THR     0x00    /* Transmit holding (write only) */
#define UART_LSR     0x05    /* Line status register */

#define UART_LSR_DR   0x01   /* Data ready */
#define UART_LSR_THRE 0x20   /* Transmitter holding register empty */

static inline volatile uint8_t *uart_reg_ptr(uintptr_t offset)
{
    return (volatile uint8_t *)(UART0_BASE + offset);
}

void uart_init(void)
{
    /* QEMU sets up the UART for us; nothing to do for a simple console. */
}

void uart_putc(char c)
{
    /* Wait until transmitter is ready. */
    while ( (*uart_reg_ptr(UART_LSR) & UART_LSR_THRE) == 0 ) {
        /* spin */
    }
    *uart_reg_ptr(UART_THR) = (uint8_t)c;
}

int uart_getc(void)
{
    if ( (*uart_reg_ptr(UART_LSR) & UART_LSR_DR) == 0 ) {
        return -1;
    }
    return (int)(*uart_reg_ptr(UART_RBR));
}

int uart_getc_blocking(void)
{
    int ch;
    while ( (ch = uart_getc()) < 0 ) {
        /* spin */
    }
    return ch;
}
