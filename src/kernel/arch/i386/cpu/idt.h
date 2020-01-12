#ifndef _IDT_H_
#define _IDT_H_

#define IDT_IRQ_BASE 32

#define IDT_DIVIDE_ERROR_INT_NO    0
#define IDT_GP_FAULT_INT_NO        13

void idt_init(void);

#endif /* _IDT_H_ */
