#ifndef _I8259_H_
#define _I8259_H_

#include <stdint.h>

#define I8259_NO_IRQS 16

void i8259_init(void);

void i8259_enable_irq(uint8_t irq);
void i8259_disable_irq(uint8_t irq);

void i8259_send_eoi(uint8_t irq);

#endif /* _I8259_H_ */
