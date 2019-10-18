/* Basic print functions, to be used until we have printf family. */

#include <stddef.h>
#include <stdint.h>

#include "kernel/arch/i386/uart.h"
#include "bootstrap_print.h"

static char
int_to_hex_char(uint8_t x)
{
    if (x <= 9) {
        return x + 48;
    } else {
        return x + 87;
    }
}

size_t
print_str(const char *str)
{
    size_t count;

    if (!str) {
        return 0;
    }

    count = 0;

    while (*str) {
        uart_write_byte(*str);
        str++;
        count++;
    }

    return count;
}

void
print_int(int x)
{
    size_t index;
    int digits[10];

    if (x == 0) {
        uart_write_byte('0');
        return;
    }

    if (x < 0) {
        uart_write_byte('-');
        x = -1*x;
    }

    index = 0;
    
    while (x != 0) {
        digits[index] = x % 10;
        x = x / 10;
        index++;
    }

    for (size_t i = index - 1; i < index - 1; i--) {
        uart_write_byte(digits[i] + 48);
    }
}

void
print_hex(unsigned int x)
{
    size_t index;
    char chars[9];

    print_str("0x");

    if (x == 0) {
        print_str("0");
        return;
    }

    index = 0;
    while (x != 0) {
        chars[index] = int_to_hex_char(x % 16);

        x = x / 16;
        index++;
    }

    for (size_t i = index - 1; i < index - 1; i--) {
        uart_write_byte(chars[i]);
    }
}

void
print_ptr(const void *ptr)
{
    print_hex((unsigned int)ptr);
}
