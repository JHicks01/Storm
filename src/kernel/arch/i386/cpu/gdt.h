#ifndef _GDT_H_
#define _GDT_H_

#define GDT_NULL_SEGMENT_INDEX   0
#define GDT_CODE_SEGMENT_INDEX   1
#define GDT_DATA_SEGMENT_INDEX   2

/*
 * 16 bit segment selector values, to be used when setting ds, cs, etc,
 * registers.
 *
 * Bits 0 and 1 select the privilege level (0 is most privileged). Bit 2 is the
 * table indicator. A clear flag selects the GDT. Bits 0-15 are the index to
 * the relevant segment in the GDT. 
 */
#define GDT_CODE_SEGMENT_SELECTOR   (GDT_CODE_SEGMENT_INDEX << 3)
#define GDT_DATA_SEGMENT_SELECTOR   (GDT_DATA_SEGMENT_INDEX << 3)

#ifndef __ASSEMBLER__ 

void gdt_setup(void);

#endif /* __ASSEMBLER__ */

#endif /* _GDT_H_ */
