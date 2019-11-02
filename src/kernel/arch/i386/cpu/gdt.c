#include <stdint.h>

#include "cpu.h"
#include "cpu_table_descriptors.h"
#include "gdt.h"

#define GDT_SIZE 3

#define GDT_SEG_DATA_RW (0x1 << 9)
#define GDT_SEG_CODE_RX ((0x1 << 9) | (0x1 << 11))

extern void load_gdt(struct cpu_pseudo_descriptor *pseudo_descriptor);

static __attribute__((aligned(8))) struct cpu_segment_descriptor gdt[GDT_SIZE];

static void
gdt_null_descriptor_init(struct cpu_segment_descriptor *descriptor)
{
    descriptor->low = 0;
    descriptor->high = 0;
}

static void
gdt_code_descriptor_init(struct cpu_segment_descriptor *descriptor)
{
    descriptor->low = 0xffff;   /* Limit - bits 0-15 */
    descriptor->high = 
        (0xf << 16)             /* Limit - bits 16-19 */
        | CPU_SEG_S_FLAG
        | CPU_SEG_P_FLAG
        | CPU_SEG_DB_FLAG
        | CPU_SEG_G_FLAG
        | GDT_SEG_CODE_RX;
}

static void
gdt_data_descriptor_init(struct cpu_segment_descriptor *descriptor)
{
    descriptor->low = 0xffff;   /* Limit - bits 0-15 */
    descriptor->high = 
        (0xf << 16)             /* Limit - bits 16-19 */
        | CPU_SEG_S_FLAG
        | CPU_SEG_P_FLAG
        | CPU_SEG_DB_FLAG
        | CPU_SEG_G_FLAG
        | GDT_SEG_DATA_RW;
}

void
gdt_setup(void)
{
    struct cpu_pseudo_descriptor pseudo_desc;

    gdt_null_descriptor_init(&gdt[GDT_NULL_SEGMENT_INDEX]);
    gdt_code_descriptor_init(&gdt[GDT_CODE_SEGMENT_INDEX]);
    gdt_data_descriptor_init(&gdt[GDT_DATA_SEGMENT_INDEX]);

    cpu_pseudo_descriptor_init(&pseudo_desc,
                               (uint32_t)gdt,
                               (uint16_t)sizeof(gdt));

    load_gdt(&pseudo_desc);
}
