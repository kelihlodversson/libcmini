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
 *  - %rsp is already set to _base->p_hitpa by the kernel's gouser() before
 *    a launched program gets control -- matching ARM's own convention
 *    (bdos/arch/arm/rwa.S's comment on p_hitpa), guaranteed by the same
 *    bdos/proc.c (init_pd_fields()) comment that documents it
 *  - there is no equivalent of the 64-byte "buggy OS reads past the top
 *    of the stack on every trap" workaround arch/m68k/setstack.S talks
 *    about: pTOS's GEMDOS syscall entry (bios/arch/x86_64/trapasm.S) takes
 *    its arguments from registers, never from the stack
 *
 * See pTOS issue #329 (https://github.com/kelihlodversson/pTOS/issues/329)
 * and doc/ptos.txt for the ILP32 userspace ABI design.
 */

#include "libc-symbols.h"

	.globl	C_SYMBOL_NAME(_base)		@ BASEPAGE *, defined in globals.c
	.globl	C_SYMBOL_NAME(_stksize)		@ long, defined by user or in stksiz.c

	.extern	C_SYMBOL_NAME(_crtinit)		@ void _crtinit(void);

	.text
	.align	8
	.globl	C_SYMBOL_NAME(_start)
	.type	C_SYMBOL_NAME(_start), @function
C_SYMBOL_NAME(_start):
	/* %rdi = basepage pointer (from kernel's gouser() via syscall dispatch) */
	movq	%rdi, C_SYMBOL_NAME(_base)(%rip)

	/* Tail-call to C-level initialization */
	jmp	C_SYMBOL_NAME(_crtinit)
