#include "cpu.h"
#include "heap.h"
#include "i8259.h"
#include "printf.h"
#include "uart.h"

void start_kernel(void);

void
start_kernel(void)
{
    cpu_init();
    i8259_init();
    uart_init();
    heap_init();
    
    printf("Bootup complete!\n");

    for (;;);
}
