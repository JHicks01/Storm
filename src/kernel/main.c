#include "kernel/bootstrap_print.h"
#include "kernel/arch/i386/uart.h"

void start_kernel(void);

void
start_kernel(void)
{
    uart_setup();

    print_str("Bootup complete!\n");

    for (;;);
}
