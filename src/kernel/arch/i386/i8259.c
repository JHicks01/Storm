#include <stdint.h>

#include "assert.h"
#include "idt.h"
#include "io.h"
#include "i8259.h"

#define I8259_MASTER_PIC_CMD_PORT     0x20
#define I8259_MASTER_PIC_DATA_PORT    0x21
#define I8259_SLAVE_PIC_CMD_PORT      0xA0
#define I8259_SLAVE_PIC_DATA_PORT     0xA1

#define I8259_SLAVE_IRQ 2

#define I8259_INIT_CMD    0x11
#define I8259_EOI_CMD     0x20

#define I8259_8086_MODE 0x1

struct i8259_pic {
    uint16_t cmd_port;
    uint16_t data_port;
    uint8_t imr;
};

static struct i8259_pic master_pic;
static struct i8259_pic slave_pic;

static void
i8259_pic_init(struct i8259_pic *pic, uint16_t cmd_port, uint16_t data_port)
{
    pic->cmd_port = cmd_port;
    pic->data_port = data_port;
    pic->imr = 0xff; /* All irqs masked. */
}

static void
i8259_pic_write_cmd(const struct i8259_pic *pic, uint8_t byte)
{
    io_write_byte(pic->cmd_port, byte);
}

static void
i8259_pic_write_data(const struct i8259_pic *pic, uint8_t byte)
{
    io_write_byte(pic->data_port, byte);
}

static void
i8259_pic_update_imr(const struct i8259_pic *pic)
{
    i8259_pic_write_data(pic, pic->imr);
}

static void
i8259_pic_enable_irq(struct i8259_pic *pic, uint8_t irq)
{
    pic->imr &= ~(1 << irq);
    i8259_pic_update_imr(pic);
}

static void
i8259_pic_disable_irq(struct i8259_pic *pic, uint8_t irq)
{
    pic->imr |= (1 << irq);
    i8259_pic_update_imr(pic);
}

static void
i8259_pic_send_eoi(struct i8259_pic *pic)
{
    i8259_pic_write_cmd(pic, I8259_EOI_CMD);
}

static void
i8259_master_pic_init(struct i8259_pic *pic)
{
    i8259_pic_init(pic, I8259_MASTER_PIC_CMD_PORT, I8259_MASTER_PIC_DATA_PORT);
}

static void
i8259_slave_pic_init(struct i8259_pic *pic)
{
    i8259_pic_init(pic, I8259_SLAVE_PIC_CMD_PORT, I8259_SLAVE_PIC_DATA_PORT);
}

void
i8259_init(void)
{
    i8259_master_pic_init(&master_pic);
    i8259_slave_pic_init(&slave_pic);

    /*
     * After receiving the first initialization word on it's cmd port, the PIC
     * will wait to receive a further 3 initialization words on it's data port.
     *
     * Word 2: Interrupt vector offset.
     *
     * Word 3: In master mode this word informs the pic which IRQs have slave
     *         pics.
     *
     *         In slave mode this word informs the pic which master pic's IRQ
     *         it is cascaded on.
     *
     * Word 4: Additional information about the environment.
     */

    /* ICW1: Begin initialization sequence. */
    i8259_pic_write_cmd(&master_pic, I8259_INIT_CMD);
    i8259_pic_write_cmd(&slave_pic, I8259_INIT_CMD);

    /*
     * ICW2: Vectors 0-31 are in use, so the desired offset for the master pic
     * is 32 (0x20). The slave comes immediately after at offset 0x28.
     */
    i8259_pic_write_data(&master_pic, IDT_IRQ_BASE);
    i8259_pic_write_data(&slave_pic, IDT_IRQ_BASE + 8);

    /* ICW3: Inform the master which IRQs are being used to drive slave pics. */
    i8259_pic_write_data(&master_pic, 0x1 << I8259_SLAVE_IRQ);

    /* ICW3: Inform the slave which IRQ the master is using to drive it. */
    i8259_pic_write_data(&slave_pic, I8259_SLAVE_IRQ);

    /* ICW4: 8086 mode. */
    i8259_pic_write_data(&master_pic, I8259_8086_MODE);
    i8259_pic_write_data(&slave_pic, I8259_8086_MODE);

    /* Mask all interrupts (i8259_pic_init() initialized imr value to 0xff). */
    i8259_pic_update_imr(&master_pic);
    i8259_pic_update_imr(&slave_pic);

    /* Enable IRQ that drives the slave pic. */
    i8259_enable_irq(I8259_SLAVE_IRQ);
}

void
i8259_enable_irq(uint8_t irq)
{
    assert(irq < I8259_NO_IRQS);

    if (irq >= 8) {
        i8259_pic_enable_irq(&slave_pic, irq - 8);
    } else {
        i8259_pic_enable_irq(&master_pic, irq);
    }
}

void
i8259_disable_irq(uint8_t irq)
{
    assert(irq < I8259_NO_IRQS);

    if (irq >= 8) {
        i8259_pic_disable_irq(&slave_pic, irq - 8);
    } else {
        i8259_pic_disable_irq(&master_pic, irq);
    }
}

void
i8259_send_eoi(uint8_t irq)
{
    /*
     * If the IRQ was raised by the slave, the EOI command must be sent to both
     * master and slave PICs.
     */

    if (irq >= 8) {
        i8259_pic_send_eoi(&slave_pic);
    }

    i8259_pic_send_eoi(&master_pic);
}
