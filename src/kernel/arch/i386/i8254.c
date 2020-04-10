#include <stddef.h>
#include <stdint.h>

#include "cpu.h"
#include "io.h"
#include "i8254.h"

#define I8254_OSCIL_FREQ             1193182

#define I8254_CHANNEL0_DATA_PORT     0x40
#define I8254_COMMAND_PORT           0x43

#define I8254_MODE_BINARY            0x0
#define I8254_MODE_RATE_GENERATOR    (0x1 << 2)
#define I8254_MODE_ACCESS_LOW        (0x1 << 4)
#define I8254_MODE_ACCESS_HIGH       (0x1 << 5)
#define I8254_SELECT_CHANNEL_0       0x0

#define I8254_IRQ_NO                 0

static uint32_t i8254_tick_count = 0;
static uint32_t i8254_freq;

static void
i8254_irq_handler(void *arg)
{
    (void)arg;

    i8254_tick_count++;
}

void
i8254_init(uint32_t freq)
{
    uint8_t cmd;
    uint16_t divisor;

    i8254_freq = freq;

    cmd = I8254_MODE_BINARY |
        I8254_MODE_RATE_GENERATOR |
        I8254_MODE_ACCESS_LOW |
        I8254_MODE_ACCESS_HIGH |
        I8254_SELECT_CHANNEL_0;

    io_write_byte(I8254_COMMAND_PORT, cmd);

    divisor = I8254_OSCIL_FREQ / i8254_freq;

    io_write_byte(I8254_CHANNEL0_DATA_PORT, divisor & 0xff);
    io_write_byte(I8254_CHANNEL0_DATA_PORT, divisor >> 8);

    cpu_register_irq(I8254_IRQ_NO, i8254_irq_handler, NULL);
}

void
i8254_sleep(uint32_t time_s)
{
    uint32_t required_ticks, finished_tick;

    required_ticks = i8254_freq * time_s;

    finished_tick = i8254_tick_count + required_ticks;

    while (i8254_tick_count < finished_tick) {
        cpu_wait_for_interrupt();
    }
}
