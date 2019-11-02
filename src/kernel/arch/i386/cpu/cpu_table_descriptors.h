#ifndef _CPU_TABLE_DESCRIPTORS_H_
#define _CPU_TABLE_DESCRIPTORS_H_

#include <stdint.h>

struct cpu_segment_descriptor {
    uint32_t low;
    uint32_t high;
} __attribute__((packed));

struct cpu_pseudo_descriptor {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void cpu_pseudo_descriptor_init(struct cpu_pseudo_descriptor *descriptor,
                                uint32_t addr,
                                uint16_t size);

#endif /* _CPU_TABLE_DESCRIPTORS_H_ */
