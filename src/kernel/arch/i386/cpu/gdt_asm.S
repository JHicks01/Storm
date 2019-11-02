#include "gdt.h"

.global load_gdt
load_gdt:
        mov 4(%esp), %ebx   /* Pointer to pseudo descriptor. */
        lgdt (%eax)
        mov $GDT_DATA_SEGMENT_SELECTOR, %ax
        mov %ax, %ds
        mov %ax, %es
        mov %ax, %fs
        mov %ax, %gs
        mov %ax, %ss
        jmp $GDT_CODE_SEGMENT_SELECTOR, $out

out:
        ret
