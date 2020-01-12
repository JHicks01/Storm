#include "bootstrap_print.h"
#include "i8259.h"
#include "cpu.h"
#include "uart.h"

void start_kernel(void);

void
start_kernel(void)
{
    cpu_init();
    i8259_init();
    uart_init();
    
    print_str("Bootup complete!\n");

    for (;;);
}
