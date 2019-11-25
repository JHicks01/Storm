#include <stdbool.h>
#include <stdint.h>

#include "cpu.h"
#include "gdt.h"
#include "idt.h"

#define CPU_INTERRUPTS_ENABLED_FLAG (0x1 << 9)

void
cpu_setup(void)
{
    gdt_setup();
    idt_setup();
}

bool
cpu_interrupts_enabled(void)
{
    uint32_t flags;

    flags = cpu_get_eflags();

    return (flags & CPU_INTERRUPTS_ENABLED_FLAG);
}
