#include "assert.h"
#include "cpu.h"
#include "heap.h"
#include "i8254.h"
#include "i8259.h"
#include "printf.h"
#include "thread.h"
#include "uart.h"

#define STORM_TIMER_FREQ 100

void start_kernel(void);

void
start_kernel(void)
{
    cpu_init();
    i8259_init();
    i8254_init(STORM_TIMER_FREQ);
    uart_init();
    heap_init();

    assert(!thread_init());

    cpu_enable_interrupts();

    thread_start_scheduler();

    /* Never reached. */
}
