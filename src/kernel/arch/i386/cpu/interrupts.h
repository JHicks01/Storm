#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <stdint.h>

/*
 * IRQ handler function type.
 *
 * IRQ handlers take a single void * argument and return void.
 */
typedef void (*irq_handler_fn_t)(void *arg);

/* Initialize interrupts.c module. */
void interrupts_init(void);

/*
 * Enable IRQ number 'irq' and register an IRQ handler to service that
 * interrupt.
 */
void interrupts_register_irq(uint8_t irq, irq_handler_fn_t fn, void *arg);

#endif /* _INTERRUPTS_H_ */
