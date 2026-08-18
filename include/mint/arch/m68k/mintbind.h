/*
 * arch/m68k/mintbind.h - m68k trap primitives for the MiNT-extension
 * GEMDOS bindings in mint/mintbind.h
 *
 * Moved out of mint/mintbind.h verbatim, the same way mint/osbind.h's
 * own primitives were split into arch/m68k/osbind.h and arch/arm/osbind.h
 * -- see https://github.com/kelihlodversson/libcmini/issues/1.
 */

#ifndef _MINT_ARCH_M68K_MINTBIND_H
#define _MINT_ARCH_M68K_MINTBIND_H 1

/* see compiler.h for __extension__ and AND_MEMORY */

#define trap_1_wllw(n, a, b, c)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long _a = (long)(a);						\
	long  _b = (long) (b);						\
	short  _c = (short) (c);					\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(12),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c)     /* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_1_wwlw(n, a, b, c)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	long  _b = (long) (b);						\
	short  _c = (short) (c);					\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(10),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c)     /* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_1_wwww(n, a, b, c)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	short  _b = (short)(b);						\
	short  _c = (short)(c);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%4,%%sp@-\n\t"					\
		"movw	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"addql	#8,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c)     /* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_1_wwwl(n, a, b, c)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	short  _b = (short)(b);						\
	long  _c = (long)(c);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%4,%%sp@-\n\t"					\
		"movw	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(10),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c)     /* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_1_wwl(n, a, b)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	long  _b = (long) (b);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"addql	#8,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b)		/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_1_wlllw(n, a, b, c, d)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long _a = (long) (a);						\
	long _b = (long) (b);						\
	long _c = (long) (c);						\
	short _d = (short) (d);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%5,%%sp@-\n\t"					\
		"movl	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(16),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c), "r"(_d) /* inputs  */	\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_1_wlll(n, a, b, c)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long _a = (long)(a);						\
	long _b = (long)(b);						\
	long _c = (long)(c);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(14),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c)	/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
									\
	retvalue;							\
})

#define trap_1_wwllll(n, a, b, c, d, e)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	long  _b = (long) (b);						\
	long  _c = (long) (c);						\
	long  _d = (long) (d);						\
	long  _e = (long) (e);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%6,%%sp@-\n\t"					\
		"movl	%5,%%sp@-\n\t"					\
		"movl	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(20),%%sp "					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c), "r"(_d), "r"(_e) /* inputs  */ \
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_1_wllll(n, a, b, c, d)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long _a = (long)(a);						\
	long _b = (long)(b);						\
	long _c = (long)(c);						\
	long _d = (long)(d);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%5,%%sp@-\n\t"					\
		"movl	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(18),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c),				\
	  "r"(_d)				/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
									\
	retvalue;							\
})

#define trap_1_wwlllll(n, a, b, c, d, e, f)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	long _b = (long)(b);						\
	long _c = (long)(c);						\
	long _d = (long)(d);						\
	long _e = (long)(e);						\
	long _f = (long)(f);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%7,%%sp@-\n\t"					\
		"movl	%6,%%sp@-\n\t"					\
		"movl	%5,%%sp@-\n\t"					\
		"movl	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(24),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c),				\
	  "r"(_d), "r"(_e), "r"(_f)		/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
									\
	retvalue;							\
})

#define trap_1_wlllll(n, a, b, c, d, e)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long _a = (long)(a);						\
	long _b = (long)(b);						\
	long _c = (long)(c);						\
	long _d = (long)(d);						\
	long _e = (long)(e);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%6,%%sp@-\n\t"					\
		"movl	%5,%%sp@-\n\t"					\
		"movl	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(22),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c),				\
	  "r"(_d), "r"(_e)			/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
									\
	retvalue;							\
})

#define trap_1_wllllll(n, a, b, c, d, e, f)				\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long _a = (long)(a);						\
	long _b = (long)(b);						\
	long _c = (long)(c);						\
	long _d = (long)(d);						\
	long _e = (long)(e);						\
	long _f = (long)(f);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%7,%%sp@-\n\t"					\
		"movl	%6,%%sp@-\n\t"					\
		"movl	%5,%%sp@-\n\t"					\
		"movl	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(26),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c),				\
	  "r"(_d), "r"(_e), "r"(_f)		/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
									\
	retvalue;							\
})

#endif /* _MINT_ARCH_M68K_MINTBIND_H */
