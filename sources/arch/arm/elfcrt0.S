#include "libc-symbols.h"
#include "../elf-entry-note.h"

	.syntax unified
	.arm
	.text
	.align 2
	.globl C_SYMBOL_NAME(_start)
	.type C_SYMBOL_NAME(_start), %function
C_SYMBOL_NAME(_start):
	mov r1, r0
	mov r0, sp
	b C_SYMBOL_NAME(__libc_start_elf)
	.size C_SYMBOL_NAME(_start), .-C_SYMBOL_NAME(_start)
	.section .preinit_array,"aw",%preinit_array
	.balign 4
	.word C_SYMBOL_NAME(__libc_elf_publish_vectors)
	.section .note.GNU-stack,"",%progbits
