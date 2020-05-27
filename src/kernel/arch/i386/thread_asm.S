/* void thread_load_first_asm(uint32_t stack_ptr); */
.global thread_load_first_asm
thread_load_first_asm:
        mov 4(%esp), %esp
        jmp thread_restore

/*
 * void thread_context_switch_asm(uint32_t *current_stack_ptr,
 *                                uint32_t next_stack_ptr);
 */
.global thread_context_switch_asm
thread_context_switch_asm:
        mov 8(%esp), %ecx       /* ECX = next_stack_ptr */
        mov 4(%esp), %eax       /* EAX = current_stack_ptr */

        /* Save registers belonging to current thread. */
        push %ebp
        push %ebx
        push %edi
        push %esi

        /* Save current thread's stack. */
        mov %esp, (%eax)

        /* Switch to new stack. */
        mov %ecx, %esp
        
thread_restore:
        pop %esi
        pop %edi
        pop %ebx
        pop %ebp
        ret
