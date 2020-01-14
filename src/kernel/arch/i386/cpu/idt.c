#include <stddef.h>
#include <stdint.h>

#include "cpu.h"
#include "cpu_table_descriptors.h"
#include "gdt.h"
#include "idt.h"
#include "panic.h"

#define IDT_SIZE 256

#define IDT_SEG_INT_GATE ((0x1 << 9) | (0x1 << 10) | (0x1 << 11))

extern void load_idt(struct cpu_pseudo_descriptor *pseudo_descriptor);

#define DECLARE_ISR_FN(n) extern void isr_##n(void)

DECLARE_ISR_FN(0);
DECLARE_ISR_FN(1);
DECLARE_ISR_FN(2);
DECLARE_ISR_FN(3);
DECLARE_ISR_FN(4);
DECLARE_ISR_FN(5);
DECLARE_ISR_FN(6);
DECLARE_ISR_FN(7);
DECLARE_ISR_FN(8);
DECLARE_ISR_FN(9);
DECLARE_ISR_FN(10);
DECLARE_ISR_FN(11);
DECLARE_ISR_FN(12);
DECLARE_ISR_FN(13);
DECLARE_ISR_FN(14);
DECLARE_ISR_FN(15);
DECLARE_ISR_FN(16);
DECLARE_ISR_FN(17);
DECLARE_ISR_FN(18);
DECLARE_ISR_FN(19);
DECLARE_ISR_FN(20);
DECLARE_ISR_FN(21);
DECLARE_ISR_FN(32);
DECLARE_ISR_FN(33);
DECLARE_ISR_FN(34);
DECLARE_ISR_FN(35);
DECLARE_ISR_FN(36);
DECLARE_ISR_FN(37);
DECLARE_ISR_FN(38);
DECLARE_ISR_FN(39);
DECLARE_ISR_FN(40);
DECLARE_ISR_FN(41);
DECLARE_ISR_FN(42);
DECLARE_ISR_FN(43);
DECLARE_ISR_FN(44);
DECLARE_ISR_FN(45);
DECLARE_ISR_FN(46);
DECLARE_ISR_FN(47);

static __attribute__((aligned(8))) struct cpu_segment_descriptor idt[IDT_SIZE];

typedef void (*interrupt_entry_point_t)(void);

static void
idt_default_interrupt_handler(void)
{
    panic("Unhandled interrupt!");
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

#define INIT_IDT_ENTRY(n) idt_interrupt_gate_descriptor_init(&idt[n], isr_##n)

void
idt_init(void)
{
    struct cpu_pseudo_descriptor pseudo_desc;

    /*
     * Initialize every entry to the default handler so nothing is left
     * uninitialized.
     */
    for (size_t i = 0; i < IDT_SIZE; i++) {
        idt_interrupt_gate_descriptor_init(&idt[i],
                                           idt_default_interrupt_handler);
    }

    /* Exceptions */
    INIT_IDT_ENTRY(0);
    INIT_IDT_ENTRY(1);
    INIT_IDT_ENTRY(2);
    INIT_IDT_ENTRY(3);
    INIT_IDT_ENTRY(4);
    INIT_IDT_ENTRY(5);
    INIT_IDT_ENTRY(6);
    INIT_IDT_ENTRY(7);
    INIT_IDT_ENTRY(8);
    INIT_IDT_ENTRY(9);
    INIT_IDT_ENTRY(10);
    INIT_IDT_ENTRY(11);
    INIT_IDT_ENTRY(12);
    INIT_IDT_ENTRY(13);
    INIT_IDT_ENTRY(14);
    INIT_IDT_ENTRY(15);
    INIT_IDT_ENTRY(16);
    INIT_IDT_ENTRY(17);
    INIT_IDT_ENTRY(18);
    INIT_IDT_ENTRY(19);
    INIT_IDT_ENTRY(20);
    INIT_IDT_ENTRY(21);

    /* Interrupts 22-31 reserved by Intel. */

    /* User defined interrupts */
    INIT_IDT_ENTRY(32);
    INIT_IDT_ENTRY(33);
    INIT_IDT_ENTRY(34);
    INIT_IDT_ENTRY(35);
    INIT_IDT_ENTRY(36);
    INIT_IDT_ENTRY(37);
    INIT_IDT_ENTRY(38);
    INIT_IDT_ENTRY(39);
    INIT_IDT_ENTRY(40);
    INIT_IDT_ENTRY(41);
    INIT_IDT_ENTRY(42);
    INIT_IDT_ENTRY(43);
    INIT_IDT_ENTRY(44);
    INIT_IDT_ENTRY(45);
    INIT_IDT_ENTRY(46);
    INIT_IDT_ENTRY(47);

    cpu_pseudo_descriptor_init(&pseudo_desc,
                               (uint32_t)idt,
                               (uint16_t)sizeof(idt));


    load_idt(&pseudo_desc);
}
