#ifndef _CPU_H_
#define _CPU_H_

#include <stdbool.h>
#include <stdint.h>

#include "interrupts.h"

#define CPU_SEG_S_FLAG (0x1 << 12)
#define CPU_SEG_P_FLAG (0x1 << 15)
#define CPU_SEG_DB_FLAG (0x1 << 22)
#define CPU_SEG_G_FLAG (0x1 << 23)

void cpu_init(void);

uint32_t cpu_get_eflags(void);

bool cpu_interrupts_enabled(void);
void cpu_enable_interrupts(void);
void cpu_disable_interrupts(void);

void cpu_wait_for_interrupt(void);
void cpu_halt(void);

void cpu_register_irq(uint8_t irq, irq_handler_fn_t fn, void *arg);

#endif /* _CPU_H_ */
