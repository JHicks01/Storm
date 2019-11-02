#include <stddef.h>
#include <stdint.h>

#include "kernel/bootstrap_print.h"
#include "cpu.h"
#include "cpu_table_descriptors.h"
#include "gdt.h"
#include "idt.h"

#define IDT_SIZE 256

#define IDT_SEG_INT_GATE ((0x1 << 9) | (0x1 << 10) | (0x1 << 11))

extern void load_idt(struct cpu_pseudo_descriptor *pseudo_descriptor);

static __attribute__((aligned(8))) struct cpu_segment_descriptor idt[IDT_SIZE];

typedef void (*interrupt_entry_point_t)(void);

static void
idt_default_interrupt_handler(void)
{
    print_str("Interrupt!\n");
}

static void
idt_interrupt_gate_descriptor_init(struct cpu_segment_descriptor *descriptor,
                                   interrupt_entry_point_t entry_fn)
{
    descriptor->low =
        ((uint32_t)entry_fn & (0xffff))
        | (GDT_CODE_SEGMENT_SELECTOR << 16);

    descriptor->high =
        ((uint32_t)entry_fn & (0xffff0000))
        | CPU_SEG_P_FLAG
        | IDT_SEG_INT_GATE;
}

void
idt_setup(void)
{
    struct cpu_pseudo_descriptor pseudo_desc;

    for (size_t i = 0; i < IDT_SIZE; i++) {
        idt_interrupt_gate_descriptor_init(&idt[i],
                                           idt_default_interrupt_handler);
    }

    cpu_pseudo_descriptor_init(&pseudo_desc,
                               (uint32_t)idt,
                               (uint16_t)sizeof(idt));


    load_idt(&pseudo_desc);
}
