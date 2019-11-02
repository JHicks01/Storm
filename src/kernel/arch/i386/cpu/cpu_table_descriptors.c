#include <stdint.h>

#include "cpu_table_descriptors.h"

void cpu_pseudo_descriptor_init(struct cpu_pseudo_descriptor *descriptor,
                                uint32_t addr,
                                uint16_t size)
{
    descriptor->limit = size - 1;
    descriptor->base = addr;
}
