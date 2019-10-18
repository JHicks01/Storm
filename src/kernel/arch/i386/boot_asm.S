/* Initial bootup code. Based on template from OSDev wiki. */

/* Declare constants for the multiboot header. */
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

/* Declare a header as in the Multiboot Standard. */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Reserve a stack for the bootstrap thread. */
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

/* The kernel entry point. */
.section .text
.global boot_start
boot_start:
	movl $stack_top, %esp

	call start_kernel

	/* Hang if start_kernel returns. */
infinite_loop:
        jmp infinite_loop
