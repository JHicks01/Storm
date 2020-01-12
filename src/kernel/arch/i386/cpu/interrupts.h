#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <stdint.h>

typedef void (*irq_handler_fn_t)(void *arg);

void interrupts_init(void);

void interrupts_register_irq(uint8_t irq, irq_handler_fn_t fn, void *arg);

#endif /* _INTERRUPTS_H_ */
