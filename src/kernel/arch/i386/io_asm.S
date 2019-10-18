.section .text

.global io_read_byte
io_read_byte:
        push %ebp
        mov %esp, %ebp
        mov 8(%ebp), %edx       /* Store port in %edx. */
        in %dx, %al
        leave
        ret

.global io_write_byte
io_write_byte:
        push %ebp
        mov %esp, %ebp
        mov 12(%ebp), %eax      /* Store byte in %eax. */
        mov 8(%ebp), %edx       /* Store port in %edx. */
        out %al, %dx
        leave
        ret
