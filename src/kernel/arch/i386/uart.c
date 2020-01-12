#include "stdint.h"

#include "io.h"
#include "uart.h"

#define UART_CLOCK_FREQ   115200
#define UART_BAUD_RATE    115200
#define UART_DIVISOR      (UART_CLOCK_FREQ / UART_BAUD_RATE)

#define UART_COM1_PORT 0x3F8

#define UART_DATA_REG_OFFSET       0
#define UART_DIV_LOW_REG_OFFSET    0
#define UART_DIV_HIGH_REG_OFFSET   1
#define UART_LCR_REG_OFFSET        3

#define UART_DLAB_BIT (0x1 << 7)

#define UART_LCR_8_DATA_BITS     0x3
#define UART_LCR_1_STOP_BIT      0x0
#define UART_LCR_NO_PARITY_BIT   0x0

void
uart_init(void)
{
    /* Set DLAB (Divisor Latch Access Bit) to set Baud divisor. */
    io_write_byte(UART_COM1_PORT + UART_LCR_REG_OFFSET, UART_DLAB_BIT);

    /* Low byte of divisor. */
    io_write_byte(UART_COM1_PORT + UART_DIV_LOW_REG_OFFSET, UART_DIVISOR);

    /* High byte of divisor. */
    io_write_byte(UART_COM1_PORT + UART_DIV_HIGH_REG_OFFSET, UART_DIVISOR >> 8);

    /* Clear DLAB. */
    io_write_byte(UART_COM1_PORT + UART_LCR_REG_OFFSET, 0);

    /* Use 8 data bits, 1 stop bit and no parity bit. */
    io_write_byte(UART_COM1_PORT + UART_LCR_REG_OFFSET,
                  UART_LCR_8_DATA_BITS |
                  UART_LCR_1_STOP_BIT |
                  UART_LCR_NO_PARITY_BIT);

    /* TODO: Enable interrupts when we have them. */
}

void
uart_write_byte(uint8_t byte)
{
    io_write_byte(UART_COM1_PORT + UART_DATA_REG_OFFSET, byte);
}
