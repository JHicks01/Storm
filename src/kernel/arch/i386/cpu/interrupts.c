#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "assert.h"
#include "idt.h"
#include "interrupts.h"
#include "i8259.h"
#include "panic.h"

struct registers {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
};

struct interrupt_stack_frame {
    struct registers registers;
    uint32_t int_no;
    uint32_t error;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
};

struct irq_handler {
    irq_handler_fn_t fn;
    void *arg;
};

void isr_main(const struct interrupt_stack_frame *frame);

static struct irq_handler irq_handlers[I8259_NO_IRQS];

static void
irq_handler_init(struct irq_handler *handler, irq_handler_fn_t fn, void *arg)
{
    handler->fn = fn;
    handler->arg = arg;
}

static void
irq_handler_execute(struct irq_handler *handler)
{
    if (handler->fn) {
        handler->fn(handler->arg);
    }
}

static void
divide_error_exception_handler(void)
{
    panic("Divide error");
}

static void
gp_fault_exception_handler(void)
{
    panic("General protection fault");
}

static void
interrupt_handle_cpu_exception(const struct interrupt_stack_frame *frame)
{
    switch (frame->int_no) {
    case IDT_DIVIDE_ERROR_INT_NO:
        divide_error_exception_handler();
        break;

    case IDT_GP_FAULT_INT_NO:
        gp_fault_exception_handler();
        break;
        
    default:
        panic("Unhandled exception");
        break;
    }
}

static void
interrupt_handle_irq(uint8_t irq)
{
    struct irq_handler *handler;

    i8259_send_eoi(irq);

    assert(irq < sizeof(irq_handlers));
    handler = &irq_handlers[irq];

    irq_handler_execute(handler);
}

void
isr_main(const struct interrupt_stack_frame *frame)
{
    if (frame->int_no < IDT_IRQ_BASE) {
        interrupt_handle_cpu_exception(frame);
    } else {
        interrupt_handle_irq(frame->int_no - IDT_IRQ_BASE);
    }
}

void
interrupts_init(void)
{
    for (size_t i = 0; i < I8259_NO_IRQS; i++) {
        irq_handler_init(&irq_handlers[i], NULL, NULL);
    }
}

void
interrupts_register_irq(uint8_t irq, irq_handler_fn_t fn, void *arg)
{
    assert(irq < sizeof(irq_handlers));
    irq_handler_init(&irq_handlers[irq], fn, arg);
    i8259_enable_irq(irq);
}
