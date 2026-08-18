@
@ minicrt0.S - ARM startup code for programs running under pTOS
@
@ Identical to crt0.S except for the C entry point it hands off to:
@ this is the minimal variant that skips argv/environ parsing (suitable
@ for ACCs and auto-folder programs on m68k; see arch/m68k/minicrt0.S),
@ jumping to _crtinit_noargs() (minimain.c) instead of _crtinit()
@ (main.c). See crt0.S's comment for why ARM's startup is this short.
@
@ See https://github.com/kelihlodversson/libcmini/issues/1.
@

#include "libc-symbols.h"

	.globl	C_SYMBOL_NAME(_base)		@ BASEPAGE *, defined in globals.c
	.globl	C_SYMBOL_NAME(_stksize)		@ long, defined by user or in stksiz.c

	.extern	C_SYMBOL_NAME(_crtinit_noargs)	@ void _crtinit_noargs(void);

	.syntax unified
	.text
	.align	2
	.globl	C_SYMBOL_NAME(_start)
	.type	C_SYMBOL_NAME(_start), %function
C_SYMBOL_NAME(_start):
	ldr	r1, =C_SYMBOL_NAME(_base)
	str	r0, [r1]		@ _base = basepage (r0)

	b	C_SYMBOL_NAME(_crtinit_noargs)
