#include <setjmp.h>

/*
 * must be compiled with -fomit-frame-pointer,
 * otherwise we save the fp of the setjmp function
 * instead of the callers
 */

#ifdef __GNUC__
#pragma GCC optimize "-fomit-frame-pointer"

#if defined(__arm__)

/*
 * naked, not just -fomit-frame-pointer: setjmp/longjmp need r4-r11, sp
 * and lr exactly as the *caller* of setjmp left them, and a normal
 * (non-naked) function's own prologue is free to adjust sp and spill
 * registers -- including lr -- before any of our inline asm runs. naked
 * means no compiler-generated prologue/epilogue at all, so buf (r0) and
 * val (r1) can only be read via their AAPCS argument registers directly,
 * never as named C parameters.
 */

__attribute__((naked)) int setjmp(jmp_buf buf)
{
	__asm__ __volatile__(
		"\tstm	r0!, {r4-r11}\n"
		"\tstr	sp, [r0], #4\n"
		"\tstr	lr, [r0], #4\n"
		"\tmov	r0, #0\n"
		"\tbx	lr\n"
	);
}

__attribute__((naked)) void longjmp(jmp_buf buf, int val)
{
	__asm__ __volatile__(
		"\tldm	r0!, {r4-r11}\n"
		"\tldr	sp, [r0], #4\n"
		"\tldr	lr, [r0], #4\n"
		/*
		 * r0 = val ? val : 1, without a standalone predicated
		 * instruction (moveq et al need an IT block in Thumb-2,
		 * which isn't valid in ARM mode -- a branch works in both).
		 */
		"\tcmp	r1, #0\n"
		"\tbne	1f\n"
		"\tmov	r1, #1\n"
		"1:\tmov	r0, r1\n"
		"\tbx	lr\n"
	);
}

#elif defined(__x86_64__)

/*
 * x32: callee-saved %rbx, %rbp, %r12-%r15, then the caller's %rsp and
 * return address, 8 bytes each (jmp_buf is 16 longs, see setjmp.h). The
 * pointer arrives zero-extended in %rdi, so it can be used as is.
 */
__asm__(
	"\t.text\n"
	"\t.globl setjmp\n"
	"\t.type setjmp, @function\n"
	"setjmp:\n"
	"\tmovq %rbx, 0(%rdi)\n"
	"\tmovq %rbp, 8(%rdi)\n"
	"\tmovq %r12, 16(%rdi)\n"
	"\tmovq %r13, 24(%rdi)\n"
	"\tmovq %r14, 32(%rdi)\n"
	"\tmovq %r15, 40(%rdi)\n"
	"\tleaq 8(%rsp), %rdx\n"       /* sp as it is after setjmp returns */
	"\tmovq %rdx, 48(%rdi)\n"
	"\tmovq (%rsp), %rdx\n"        /* return address */
	"\tmovq %rdx, 56(%rdi)\n"
	"\txorl %eax, %eax\n"
	"\tret\n"
	"\t.size setjmp, .-setjmp\n"
	"\t.globl longjmp\n"
	"\t.type longjmp, @function\n"
	"longjmp:\n"
	"\tmovl %esi, %eax\n"
	"\ttestl %eax, %eax\n"
	"\tjnz 1f\n"
	"\tmovl $1, %eax\n"          /* val == 0 returns 1 */
	"1:\n"
	"\tmovq 0(%rdi), %rbx\n"
	"\tmovq 8(%rdi), %rbp\n"
	"\tmovq 16(%rdi), %r12\n"
	"\tmovq 24(%rdi), %r13\n"
	"\tmovq 32(%rdi), %r14\n"
	"\tmovq 40(%rdi), %r15\n"
	"\tmovq 56(%rdi), %rdx\n"
	"\tmovq 48(%rdi), %rsp\n"
	"\tjmp *%rdx\n"
	"\t.size longjmp, .-longjmp\n"
);

#else

int setjmp(jmp_buf buf)
{
	register long *a0 __asm__("%a0") = buf;
	register void *a1 __asm__("%a1") = __builtin_return_address(0);
	__asm__ __volatile__(
		"\tmovem.l	%%d2-%%d7/%%a1-%%a7,(%[regs])\n"
#ifdef __mcffpu__
		"\tfmovem%.d %%fp0-%%fp7,52(%[regs])\n"
#endif
#ifdef __HAVE_68881__
		"\tfmovem%.x %%fp0-%%fp7,52(%[regs])\n"
#endif
		:							/* output */
		: [regs] "a" (a0), "a"(a1)	/* input */
		: "memory"
	);
	return 0;
}

void longjmp(jmp_buf buf, int val)
{
	register int d0 __asm__("%d0") = val ? val : 1;
	register long *a0 __asm__("%a0") = buf;

	__asm__ __volatile__(
		"\tmovem.l	(%[a0]),%%d2-%%d7/%%a1-%%a7\n"
#ifdef __mcffpu__
		"\tfmovem%.d 52(%[a0]),%%fp0-%%fp7\n"
#endif
#ifdef __HAVE_68881__
		"\tfmovem%.x 52(%[a0]),%%fp0-%%fp7\n"
#endif
		"\taddq.l #4,%%a7\n"		/* pop return pc of setjmp() call */
		"\tjmp (%%a1)\n"
		:							/* output */
		: [a0]"a"(a0), "d" (d0)
		: /* not reached; so no need to declare any clobbered regs */
	);
	__builtin_unreachable();
}

#endif /* __arm__ || __x86_64__ */

int sigsetjmp(jmp_buf buf, int mask)
{
    return setjmp(buf);
}

#endif
