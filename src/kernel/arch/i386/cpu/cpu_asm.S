.global cpu_get_eflags
cpu_get_eflags:
        pushf
        pop %eax
        ret
        
.global cpu_enable_interrupts
cpu_enable_interrupts:
        sti
        ret

.global cpu_disable_interrupts
cpu_disable_interrupts:
        cli
        ret

.global cpu_wait_for_interrupt
cpu_wait_for_interrupt:
        hlt
        ret
