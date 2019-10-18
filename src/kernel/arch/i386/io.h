#ifndef IO_H
#define IO_H

#include <stdint.h>

uint8_t io_read_byte(uint16_t port);

void io_write_byte(uint16_t port, uint8_t byte);

#endif /* IO_H */
