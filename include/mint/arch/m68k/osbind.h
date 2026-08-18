/*
 * arch/m68k/osbind.h - m68k trap primitives for GEMDOS/BIOS/XBIOS bindings
 *
 * Moved out of mint/osbind.h verbatim as part of the ARM port (see
 * https://github.com/kelihlodversson/libcmini/issues/1): mint/osbind.h
 * now just includes the right arch/<cpu>/osbind.h and keeps the shared
 * GEMDOS/BIOS/XBIOS function definitions built on top of it.
 *
 * osbind.h	bindings for OS traps
 *
 *		++jrb bammi@cadence.com
 */

/*
 * majorly re-hacked for gcc-1.36 and probably beyond
 * all inlines changed to #defines, beacuse gcc is not
 * handling clobbered reggies correctly when -mshort.
 * We now use the Statement Exprs feature of GnuC
 *
 * 10/12/89
 *	changed all "g" constraints to "r" that will force
 *	all operands to be evaluated (or lea calculated)
 *	before the __asm__. This is necessary because
 *	if we had the (looser) "g" constraint, then sometimes
 *	we are in the situation where stuff is in the stack,
 *	and we are modifying the stack under Gcc (but eventually
 *	restoring it before the end of the __asm__), and it does
 *	not know about it (i believe there is no way to tell it
 *	this either, but you can hardly expect that). by forcing
 *	the stricter "r" constraint, we force the eval before using
 *	the val (or lea as the case may be) and we dont get into
 *	trouble.
 *	(thanks to ers for finding this problem!)
 *	[one side effect of this is that we may(depending on the
 *	  situation) actually end up with better code when the
 *	values are already in reggies, or that value is used
 *	later on (note that Gnu's reggie allocation notices the
 *	clobbered reggie, and does'nt put the value/or uses
 *	them from those reggies, nice huh!)
 *
 *  28/2/90
 *	another major re-hack:
 *	-- the basic reason: there was just no reliable
 *	way to get the definitions (inline or not does'nt matter) to
 *	fully evaluate the args before we changed the sp from under it.
 *	(if -fomit-frame-pointer is *not* used, then most of the time
 *	 we dont need to do this, as things will just reference off of
 *	 a6, but this is not true all of the time).
 *	my solution was to use local vars in the body of the statement
 *	exprs, and initialize them from the args of the statement expr block.
 *	to force the evaluation of the args before we change sp from
 *	under gcc's feet, we make the local vars volatile. we use a
 *	slight code optimization heuristic: if there are more than 4
 *	args, only then we make the local volatile, and relax
 *	the "r" constraint to "g". otherwise, we dont put the volatile
 *	and force the evaluation by putting the "r" constaint. this
 *	produces better code in most sitiations (when !__NO_INLINE__
 *	especially), as either the args are already in a register or
 *	there is good chance they will soon be reused, and in that
 *	case it will already be in a register.
 *      it may (the local vars, especially when no volatile)
 *	look like overhead, but in 99% of the situations gcc will just
 *	optimize that assignment right out. besides, this makes
 *	these defines totally safe (from re-evaluation of the macro args).
 *
 *	-- as suggested by andreas schwab (thanks!)
 *	 (schwab@ls5.informatik.uni-dortmund.de) all the retvalues are now
 *	 local register vals (see the extentions section in the info file)
 *	 this really worked out great as all the silly "movl d0,%0" at
 *	 the end of each def can now be removed, and the value of
 *	 retvalue ends up in the correct register. it avoids all the
 *	 silly "mov d0,[d|a]n" type sequences from being generated. a real win.
 *	 (note in the outputs "=r"(retvalue) still has to be specified,
 *	 otherwise in certain situations you end up loosing the return
 *	 value in d0, as gcc sees no output, and correctly assumes that the
 *	 asm returns no value).
 *
 *	-- all the n's (the function #'s for the traps) are now given
 *	the more relaxed "g". This again results in better code, as
 *	it is always a constant, and gcc turns the movw %1,sp@- into
 *	a movw #n,sp@-. we could have given them a "i" constraint too,
 *	but "g" gives gcc more breathing room, and it does the right
 *	thing. note: the n's still need to have "r" constraints in the
 *	non-inline form (function form), as they are no longer constants
 *	in the function, but a normal arg on the stack frame, and hence
 *	we need to force evaluation before we change sp. (see osbind.c)
 *
 *	-- straps.cpp and traps.c are history. we dont need no stinking
 *	non-reentrant bindings (straps) or incorrect ones (traps.c :-)
 *
 * 03/15/92 ++jrb
 *	-- another re-hack needed for gcc-2.0: the optimization that we
 *      used earlier for traps with more than 4 args, making them volatile
 *	and using "g" constraints no longer works, because gcc has become
 *	so smart! we now remove the volatile, and give "r" constraints
 *	(just like traps with <= 4 args). that way the args are evaled
 *	before we change the stack under gcc, and at appropriate times
 *	put into reggies and pushed (or as in most cases, they are evaled
 *	straight into reggies and pushed -- and in even more common cases
 *	they are already in reggies, and they are just pushed). not doing
 *	this with -fomit-frame-pointer was causing the temps (from evaluing
 *	the args) to be created on the stack, but when we changed sp
 *	from under gccs feet, the offsets  to the temps ended up being wrong.
 *
 * 10/28/93 ++jrb
 *	relax the constraints on the inputs of trap_14_wwwwwww (only
 *	Rsconf maps to this)  to "g" from "r", as these many "r" 's
 *	give gcc 2.>3.X heartaches (understandably). note this is ok
 *	since these args will never be expressions, and we never
 *	have to constrain hard enough to force eval before we change
 *	sp from underneath gcc.
 *
 */

#ifndef _MINT_ARCH_M68K_OSBIND_H
#define _MINT_ARCH_M68K_OSBIND_H 1

/*
 * GNU C (pseudo inline) Statement Exprs for traps
 *
 */

#define trap_1_w(n)							\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"addql	#2,%%sp\n\t"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n)				/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_1_ww(n, a)							\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"addql	#4,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a)			/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_1_wl(n, a)							\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long  _a = (long) (a);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"addql	#6,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a)			/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_1_wlw(n, a, b)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long  _a = (long) (a);						\
	short _b = (short)(b);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
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

#define trap_1_wwll(n, a, b, c)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	long  _b = (long) (b);						\
	long  _c = (long) (c);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
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

#define trap_1_wlww(n, a, b, c)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long  _a = (long) (a);						\
	short _b = (short)(b);						\
	short _c = (short)(c);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%4,%%sp@-\n\t"					\
		"movw	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
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

#define trap_1_wlwww(n, a, b, c, d)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	short _b = (long)(b);						\
	long  _c = (short) (c);						\
	long  _d = (short) (d);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%5,%%sp@-\n\t"					\
		"movw	%4,%%sp@-\n\t"					\
		"movw	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(12),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c), "r"(_d) /* inputs  */	\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc", "memory"			\
	);								\
	retvalue;							\
})

#define trap_1_www(n, a, b)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	short _b = (short)(b);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"addql	#6,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b)		/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_1_wll(n, a, b)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long  _a = (long) (a);						\
	long  _b = (long) (b);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(10),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b)		/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_1_wwlll(n, a, b, c, d)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);			\
	long  _b = (long) (b);			\
	long  _c = (long) (c);			\
	long  _d = (long) (d);			\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%5,%%sp@-\n\t"					\
		"movl	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(16),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c), "r"(_d) /* inputs  */	\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc", "memory"			\
	);								\
	retvalue;							\
})

#define trap_1_wwwll(n, a, b, c, d)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	short _b = (short)(b);						\
	long  _c = (long) (c);						\
	long  _d = (long) (d);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%5,%%sp@-\n\t"					\
		"movl	%4,%%sp@-\n\t"					\
		"movw	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"lea	%%sp@(14),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c), "r"(_d) /* inputs  */	\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc", "memory"			\
	);								\
	retvalue;							\
})

#define trap_13_wl(n, a)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long  _a = (long) (a);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#13\n\t"					\
		"addql	#6,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a)			/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_13_w(n)							\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#13\n\t"					\
		"addql	#2,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n)				/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_13_ww(n, a)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#13\n\t"					\
		"addql	#4,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a)			/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_13_www(n, a, b)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	short _b = (short)(b);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#13\n\t"					\
		"addql	#6,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b)		/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_13_wwlwww(n, a, b, c, d, e)				\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);			\
	long  _b = (long) (b);			\
	short _c = (short)(c);			\
	short _d = (short)(d);			\
	short _e = (short)(e);			\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%6,%%sp@-\n\t"					\
		"movw	%5,%%sp@-\n\t"					\
		"movw	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#13\n\t"					\
		"lea	%%sp@(14),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n),							\
	  "r"(_a), "r"(_b), "r"(_c), "r"(_d), "r"(_e) /* inputs  */	\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc", "memory"			\
	);								\
	retvalue;							\
})

#define trap_13_wwlwwwl(n, a, b, c, d, e, f)				\
__extension__								\
({									\
	register long __retvalue __asm__("d0");				\
	short _a = (short)(a);			\
	long  _b = (long) (b);			\
	short _c = (short)(c);			\
	short _d = (short)(d);			\
	short _e = (short)(e);			\
	long  _f = (long)(f);			\
									\
	__asm__ volatile						\
	(								\
		"movl	%7,%%sp@-\n\t"					\
		"movw	%6,%%sp@-\n\t"					\
		"movw	%5,%%sp@-\n\t"					\
		"movw	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#13\n\t"					\
		"lea	%%sp@(18),%%sp"					\
	: "=r"(__retvalue)			/* outputs */		\
	: "g"(n), "r"(_a),						\
	  "r"(_b), "r"(_c), "r"(_d), "r"(_e), "r"(_f) /* inputs  */	\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc", "memory"			\
	);								\
	__retvalue;							\
})

#define trap_13_wwl(n, a, b)						\
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
		"trap	#13\n\t"					\
		"addql	#8,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b)		/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_14_wwl(n, a, b)						\
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
		"trap	#14\n\t"					\
		"addql	#8,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b)              /* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_14_wwll(n, a, b, c)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	long  _b = (long) (b);						\
	long  _c = (long) (c);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"lea	%%sp@(12),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c)     /* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_14_ww(n, a)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"addql	#4,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a)			/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_14_w(n)							\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"addql	#2,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n)				/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_14_wllw(n, a, b, c)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long  _a = (long) (a);						\
	long  _b = (long) (b);						\
	short _c = (short)(c);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"lea	%%sp@(12),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c)       /* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_14_wl(n, a)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long  _a = (long) (a);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"addql	#6,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a)			/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_14_www(n, a, b)						\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	short _b = (short)(b);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"addql	#6,%%sp"						\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b)		/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_14_wllwwwww(n, a, b, c, d, e, f, g)			\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long  _a = (long) (a);						\
	long  _b = (long) (b);						\
	short _c = (short)(c);						\
	short _d = (short)(d);						\
	short _e = (short)(e);						\
	short _f = (short)(f);						\
	short _g = (short)(g);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%8,%%sp@-\n\t"					\
		"movw	%7,%%sp@-\n\t"					\
		"movw	%6,%%sp@-\n\t"					\
		"movw	%5,%%sp@-\n\t"					\
		"movw	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"lea	%%sp@(20),%%sp "					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b),					\
	  "r"(_c), "r"(_d), "r"(_e), "r"(_f), "r"(_g) /* inputs  */	\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc", "memory"			\
	);								\
	retvalue;							\
})

#define trap_14_wllwwwwlw(n, a, b, c, d, e, f, g, h)			\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long  _a = (long) (a);						\
	long  _b = (long) (b);						\
	short _c = (short)(c);						\
	short _d = (short)(d);						\
	short _e = (short)(e);						\
	short _f = (short)(f);						\
	long  _g = (long) (g);						\
	short _h = (short)(h);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%9,%%sp@-\n\t"					\
		"movl	%8,%%sp@-\n\t"					\
		"movw	%7,%%sp@-\n\t"					\
		"movw	%6,%%sp@-\n\t"					\
		"movw	%5,%%sp@-\n\t"					\
		"movw	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"lea	%%sp@(24),%%sp "					\
	: "=r"(retvalue)			   /* outputs */	\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c),				\
	  "r"(_d), "r"(_e), "r"(_f), "r"(_g), "r"(_h) /* inputs  */	\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc", "memory"			\
	);								\
	retvalue;							\
})

#define trap_14_wllwwwwwlw(n, a, b, c, d, e, f, g, h, i)		\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long  _a = (long) (a);						\
	long  _b = (long) (b);						\
	short _c = (short)(c);						\
	short _d = (short)(d);						\
	short _e = (short)(e);						\
	short _f = (short)(f);						\
	short _g = (short)(g);						\
	long  _h = (long) (h);						\
	short _i = (short)(i);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%9,%%sp@-\n\t"					\
		"movl	%8,%%sp@-\n\t"					\
		"movw	%7,%%sp@-\n\t"					\
		"movw	%6,%%sp@-\n\t"					\
		"movw	%5,%%sp@-\n\t"					\
		"movw	%4,%%sp@-\n\t"					\
		"movw	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movl	%1,%%sp@-\n\t"					\
                "movw	%0,%%sp@-"					\
	:					      /* outputs */	\
	: "g"(n), "g"(_a), "g"(_b), "g"(_c), "g"(_d),			\
	  "g"(_e), "g"(_f), "g"(_g), "g"(_h), "g"(_i) /* inputs  */	\
	);								\
	    								\
	__asm__ volatile						\
	(								\
		"trap	#14\n\t"					\
		"lea	%%sp@(26),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: 					/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc", "memory"			\
	);								\
	retvalue;							\
})


#define trap_14_wwwwwww(n, a, b, c, d, e, f)				\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	short _b = (short)(b);						\
	short _c = (short)(c);						\
	short _d = (short)(d);						\
	short _e = (short)(e);						\
	short _f = (short)(f);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%7,%%sp@-\n\t"					\
		"movw	%6,%%sp@-\n\t"					\
		"movw	%5,%%sp@-\n\t"					\
		"movw	%4,%%sp@-\n\t"					\
		"movw	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"lea	%%sp@(14),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "g"(_a),						\
	  "g"(_b), "g"(_c), "g"(_d), "g"(_e), "g"(_f)	/* inputs  */	\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc", "memory"			\
	);								\
	retvalue;							\
})

#define trap_14_wlll(n, a, b, c)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long  _a = (long) (a);						\
	long  _b = (long) (b);						\
	long  _c = (long) (c);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"lea	%%sp@(14),%%sp "					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c)     /* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_14_wllww(n, a, b, c, d)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long  _a = (long) (a);						\
	long  _b = (long) (b);						\
	short _c = (short)(c);						\
	short _d = (short)(d);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%5,%%sp@-\n\t"					\
		"movw	%4,%%sp@-\n\t"					\
		"movl	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"lea	%%sp@(14),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n),							\
	  "r"(_a), "r"(_b), "r"(_c), "r"(_d)    /* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc", "memory"			\
	);								\
	retvalue;							\
})

#define trap_14_wwwwl(n, a, b, c, d)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	short _b = (short)(b);						\
	short _c = (short)(c);						\
	long  _d = (long) (d);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%5,%%sp@-\n\t"					\
		"movw	%4,%%sp@-\n\t"					\
		"movw	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"lea	%%sp@(12),%%sp "					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n),							\
	  "r"(_a), "r"(_b), "r"(_c), "r"(_d)        /* inputs  */	\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc", "memory"			\
	);								\
	retvalue;							\
})

#define trap_14_wwwl(n, a, b, c)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	short _a = (short)(a);						\
	short _b = (short)(b);						\
	long  _c = (long)(c);						\
	    								\
	__asm__ volatile						\
	(								\
		"movl	%4,%%sp@-\n\t"					\
		"movw	%3,%%sp@-\n\t"					\
		"movw	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"lea	%%sp@(10),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n), "r"(_a), "r"(_b), "r"(_c)	/* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"    /* clobbered regs */	\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

#define trap_14_wlwlw(n, a, b, c, d)					\
__extension__								\
({									\
	register long retvalue __asm__("d0");				\
	long  _a = (long) (a);						\
	short _b = (short)(b);						\
	long  _c = (long) (c);						\
	short _d = (short)(d);						\
	    								\
	__asm__ volatile						\
	(								\
		"movw	%5,%%sp@-\n\t"					\
		"movl	%4,%%sp@-\n\t"					\
		"movw	%3,%%sp@-\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	%1,%%sp@-\n\t"					\
		"trap	#14\n\t"					\
		"lea	%%sp@(14),%%sp"					\
	: "=r"(retvalue)			/* outputs */		\
	: "g"(n),							\
	  "r"(_a), "r"(_b), "r"(_c), "r"(_d)    /* inputs  */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc", "memory"			\
	  AND_MEMORY							\
	);								\
	retvalue;							\
})

/*
 * Safe binding to switch back from supervisor to user mode.
 * On TOS or EmuTOS, if the stack pointer has changed between Super(0)
 * and Super(oldssp), the resulting user stack pointer is wrong.
 * This bug does not occur with FreeMiNT.
 * So the safe way to return from supervisor to user mode is to backup
 * the stack pointer then restore it after the trap.
 * Sometimes, GCC optimizes the stack usage, so this matters.
 */
#define SuperToUser(ptr)						\
(void)__extension__							\
({									\
	register long retvalue __asm__("d0");				\
	register long sp_backup;					\
									\
	__asm__ volatile						\
	(								\
		"movl	%%sp,%1\n\t"					\
		"movl	%2,%%sp@-\n\t"					\
		"movw	#0x20,%%sp@-\n\t"					\
		"trap	#1\n\t"						\
		"movl	%1,%%sp\n\t"					\
	: "=r"(retvalue), "=&r"(sp_backup)	/* outputs */		\
	: "g"((long)(ptr)) 			/* inputs */		\
	: __CLOBBER_RETURN("d0") "d1", "d2", "a0", "a1", "a2", "cc"		\
	  AND_MEMORY							\
	);								\
})

#endif /* _MINT_ARCH_M68K_OSBIND_H */
