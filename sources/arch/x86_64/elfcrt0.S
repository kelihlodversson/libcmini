/* The standard ELF stack is 16-byte aligned at entry. */
#include "libc-symbols.h"
#include "../elf-entry-note.h"

	.text
	.globl C_SYMBOL_NAME(_start)
	.type C_SYMBOL_NAME(_start), @function
C_SYMBOL_NAME(_start):
	movl %esp, %edi
	movl %edx, %esi
	call C_SYMBOL_NAME(__libc_start_elf)
	ud2
	.size C_SYMBOL_NAME(_start), .-C_SYMBOL_NAME(_start)
	/* Link this object first so libc state precedes user preinitializers. */
	.section .preinit_array,"aw",@preinit_array
	.balign 4
	.long C_SYMBOL_NAME(__libc_elf_publish_vectors)
	.section .note.GNU-stack,"",@progbits
