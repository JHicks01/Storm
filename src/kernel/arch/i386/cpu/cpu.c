#include <stdbool.h>
#include <stdint.h>

#include "cpu.h"
#include "gdt.h"
#include "idt.h"
#include "interrupts.h"

#define CPU_INTERRUPTS_ENABLED_FLAG (0x1 << 9)

void
cpu_init(void)
{
    gdt_init();
    idt_init();
    interrupts_init();
}

bool
cpu_interrupts_enabled(void)
{
    uint32_t flags;

    flags = cpu_get_eflags();

    return (flags & CPU_INTERRUPTS_ENABLED_FLAG);
}

void
cpu_halt(void)
{
    cpu_disable_interrupts();
    
    for (;;) {
        cpu_wait_for_interrupt();
    }
}

void cpu_register_irq(uint8_t irq, irq_handler_fn_t fn, void *arg)
{
    interrupts_register_irq(irq, fn, arg);
}
