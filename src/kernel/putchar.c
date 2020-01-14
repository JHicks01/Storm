#include "printf.h"
#include "uart.h"

/*
 * Write char to UART.
 *
 * This function is used by the printf lib.
 */
void
_putchar(char c)
{
    uart_write_byte(c);
}
