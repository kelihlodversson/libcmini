/*
 * crt0.S - x86-64 startup code for programs running under pTOS
 *
 * x86-64 ILP32 userspace ABI: 32-bit int/long/pointer, 64-bit long long.
 * The kernel runs in 64-bit mode (LP64), but userspace stays ILP32.
 *
 * This mirrors arch/m68k/crt0.S's job (stash the basepage, get onto a valid
 * stack, hand off to the shared C-level _crtinit() in main.c), adapted
 * for the x86-64 pTOS port's own conventions:
 *
 *  - the basepage pointer arrives in %rdi (first syscall argument register),
 *    not on the stack -- see pTOS's bios/arch/x86_64/trap.c's dispatcher
 *    (x86_64_trap_dispatch) which extracts it from the syscall frame
 *  - the kernel starts the process on a private stack of its own (16 MiB of
 *    address space, its pages backed as they are touched), so there is no
 *    stack to set up here, no TPA and no Mshrink(); _stksize, if the program
 *    sets it, is passed to Psetlimit() (see _crtinit() in main.c)
 *  - there is no equivalent of the 64-byte "buggy OS reads past the top
 *    of the stack on every trap" workaround arch/m68k/setstack.S talks
 *    about: pTOS's GEMDOS syscall entry (bios/arch/x86_64/trapasm.S) takes
 *    its arguments from registers, never from the stack
 *
 * See pTOS issue #329 (https://github.com/kelihlodversson/pTOS/issues/329)
 * and doc/ptos.txt for the ILP32 userspace ABI design.
 */

#include "libc-symbols.h"

	.globl	C_SYMBOL_NAME(_base) /* BASEPAGE *, defined in globals.c */
	.globl	C_SYMBOL_NAME(_stksize) /* long, defined by user or in stksiz.c */

	.extern	C_SYMBOL_NAME(_crtinit) /* void _crtinit(void); */

	.text
	.align	8
	.globl	C_SYMBOL_NAME(_start)
	.type	C_SYMBOL_NAME(_start), @function
C_SYMBOL_NAME(_start):
	/* %rdi = basepage pointer (from kernel's gouser() via syscall dispatch) */
	/* ILP32: _base is 32-bit, use movl with %edi to avoid writing 8 bytes */
	movl	%edi, C_SYMBOL_NAME(_base)(%rip)
	/* RSI is the explicit pTOS entry type, independent of the basepage. */
	cmpl	$1, %esi
	je	C_SYMBOL_NAME(_acc_main)

	/* Tail-call to C-level initialization */
	jmp	C_SYMBOL_NAME(_crtinit)
