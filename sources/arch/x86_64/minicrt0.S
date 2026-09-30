/*
 * minicrt0.S - x86-64 minimal startup code for programs running under pTOS
 *
 * x86-64 ILP32 userspace ABI: 32-bit int/long/pointer, 64-bit long long.
 *
 * Identical to crt0.S except for the C entry point it hands off to:
 * this is the minimal variant that skips argv/environ parsing (suitable
 * for ACCs and auto-folder programs on m68k; see arch/m68k/minicrt0.S),
 * jumping to _crtinit_noargs() (minimain.c) instead of _crtinit() (main.c).
 *
 * See crt0.S's comment for the x86-64 pTOS port's calling convention.
 * See pTOS issue #329 for the ILP32 userspace ABI design.
 */

#include "libc-symbols.h"

	.globl	C_SYMBOL_NAME(_base)		@ BASEPAGE *, defined in globals.c
	.globl	C_SYMBOL_NAME(_stksize)		@ long, defined by user or in stksiz.c

	.extern	C_SYMBOL_NAME(_crtinit_noargs)	@ void _crtinit_noargs(void);

	.text
	.align	8
	.globl	C_SYMBOL_NAME(_start)
	.type	C_SYMBOL_NAME(_start), @function
C_SYMBOL_NAME(_start):
	/* %rdi = basepage pointer (from kernel's gouser() via syscall dispatch) */
	movq	%rdi, C_SYMBOL_NAME(_base)(%rip)

	/* Tail-call to C-level initialization (no args) */
	jmp	C_SYMBOL_NAME(_crtinit_noargs)
