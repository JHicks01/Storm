#define ISR_WITH_ERROR(n)    \
.global isr_##n;             \
isr_##n:                     \
        pushl $(n);          \
        jmp isr_common

/*
 * Some interrupts do not push an error code. Push a dummy error code so we can
 * use the same interrupt frame for both cases.
 */
#define ISR_NO_ERROR(n)    \
.global isr_##n;           \
isr_##n:                   \
        pushl $0;          \
        pushl $(n);        \
        jmp isr_common

isr_common:
        pushal
        push %esp        /* isr_main takes a pointer to the frame. */
        call isr_main    
        add $4, %esp
        popal
        add $8, %esp     /* Cleanup dummy error code and interrupt number. */
        iret

/* Exceptions */
ISR_NO_ERROR(0)
ISR_NO_ERROR(1)
ISR_NO_ERROR(2)
ISR_NO_ERROR(3)
ISR_NO_ERROR(4)
ISR_NO_ERROR(5)
ISR_NO_ERROR(6)
ISR_NO_ERROR(7)
ISR_WITH_ERROR(8)
ISR_NO_ERROR(9)
ISR_WITH_ERROR(10)
ISR_WITH_ERROR(11)
ISR_WITH_ERROR(12)
ISR_WITH_ERROR(13)
ISR_WITH_ERROR(14)
ISR_NO_ERROR(15)
ISR_NO_ERROR(16)
ISR_WITH_ERROR(17)
ISR_NO_ERROR(18)
ISR_NO_ERROR(19)
ISR_NO_ERROR(20)
ISR_WITH_ERROR(21)

/* Interrupts 22-31 reserved by Intel */

/* User defined interrupts */
ISR_NO_ERROR(32)
ISR_NO_ERROR(33)
ISR_NO_ERROR(34)
ISR_NO_ERROR(35)
ISR_NO_ERROR(36)
ISR_NO_ERROR(37)
ISR_NO_ERROR(38)
ISR_NO_ERROR(39)
ISR_NO_ERROR(40)
ISR_NO_ERROR(41)
ISR_NO_ERROR(42)
ISR_NO_ERROR(43)
ISR_NO_ERROR(44)
ISR_NO_ERROR(45)
ISR_NO_ERROR(46)
ISR_NO_ERROR(47)
