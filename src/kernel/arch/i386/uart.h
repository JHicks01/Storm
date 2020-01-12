#ifndef UART_H
#define UART_H

#include "stdint.h"

/* Initialize the UART driver. */
void uart_init(void);

/* Write a byte to UART. */
void uart_write_byte(uint8_t byte);

#endif /* UART_H */
