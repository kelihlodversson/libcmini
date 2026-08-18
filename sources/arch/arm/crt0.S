@
@ crt0.S - ARM startup code for programs running under pTOS
@
@ Mirrors arch/m68k/crt0.S's job (stash the basepage, get onto a valid
@ stack, hand off to the shared C-level _crtinit() in main.c), but the
@ actual work needed is much smaller on ARM:
@
@  - the basepage pointer arrives in r0, not on the stack -- see pTOS's
@    aes/arch/arm/gemstart.S ("our basepage address is in r0")
@  - sp is already set to _base->p_hitpa by the kernel's gouser() before
@    a launched program gets control -- see the comment on p_hitpa's
@    computation in pTOS's bdos/proc.c (init_pd_fields()), which exists
@    specifically to document and guarantee this
@  - there is no equivalent of the 64-byte "buggy OS reads past the top
@    of the stack on every trap" workaround arch/m68k/setstack.S talks
@    about: pTOS's GEMDOS trap entry (bdos/arch/arm/rwa.S) takes its
@    arguments from r0-r7, never from the stack, so nothing ever reads
@    past sp on a trap
@
@ See https://github.com/kelihlodversson/libcmini/issues/1.
@

#include "libc-symbols.h"

	.globl	C_SYMBOL_NAME(_base)		@ BASEPAGE *, defined in globals.c
	.globl	C_SYMBOL_NAME(_stksize)		@ long, defined by user or in stksiz.c

	.extern	C_SYMBOL_NAME(_crtinit)		@ void _crtinit(void);

	.syntax unified
	.text
	.align	2
	.globl	C_SYMBOL_NAME(_start)
	.type	C_SYMBOL_NAME(_start), %function
C_SYMBOL_NAME(_start):
	ldr	r1, =C_SYMBOL_NAME(_base)
	str	r0, [r1]		@ _base = basepage (r0)

	b	C_SYMBOL_NAME(_crtinit)
