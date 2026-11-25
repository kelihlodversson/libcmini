/* The standard ELF stack is 16-byte aligned at entry. */
#include "libc-symbols.h"

	.text
	.globl C_SYMBOL_NAME(_start)
	.type C_SYMBOL_NAME(_start), @function
C_SYMBOL_NAME(_start):
	movl %esp, %edi
	movl %edx, %esi
	call C_SYMBOL_NAME(__libc_start_elf)
	ud2
	.size C_SYMBOL_NAME(_start), .-C_SYMBOL_NAME(_start)
	.section .note.GNU-stack,"",@progbits
