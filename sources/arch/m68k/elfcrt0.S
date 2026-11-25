#include "libc-symbols.h"

	.text
	.even
	.globl C_SYMBOL_NAME(_start)
	.type C_SYMBOL_NAME(_start), @function
C_SYMBOL_NAME(_start):
	move.l %sp,%a0
	move.l %d1,-(%sp)
	move.l %a0,-(%sp)
	jsr C_SYMBOL_NAME(__libc_start_elf)
	illegal
	.size C_SYMBOL_NAME(_start), .-C_SYMBOL_NAME(_start)
	.section .note.GNU-stack,"",@progbits
