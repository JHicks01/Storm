/* void thread_load_first_asm(uint32_t stack_ptr); */
.global thread_load_first_asm
thread_load_first_asm:
        mov 4(%esp), %esp
        jmp thread_restore
        
thread_restore:
        pop %esi
        pop %edi
        pop %ebx
        pop %ebp
        ret
