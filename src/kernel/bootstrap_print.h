/* Basic print functions, to be used until we have printf family. */

#ifndef BOOTSTRAP_PRINT_H
#define BOOTSTRAP_PRINT_H

#include <stddef.h>

/*
 * Print a string.
 *
 * Returns the number of characters printed.
 */
size_t print_str(const char *str);

/* Print an integer. */
void print_int(int x);

/* Print an unsigned integer in hex form. */
void print_hex(unsigned int x);

/* Print a pointer. */
void print_ptr(const void *ptr); 

#endif /* BOOTSTRAP_PRINT_H */
