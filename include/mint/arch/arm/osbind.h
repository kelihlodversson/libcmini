/*
 * arch/arm/osbind.h - ARM trap primitives for GEMDOS/BIOS/XBIOS bindings
 *
 * mint/osbind.h includes this (instead of arch/m68k/osbind.h) on ARM and
 * uses these to build the same GEMDOS/BIOS/XBIOS function definitions as
 * on m68k. See https://github.com/kelihlodversson/libcmini/issues/1 for
 * how this ABI was derived from pTOS's bios/arch/arm/vectorsasm.S and
 * bdos/arch/arm/rwa.S.
 *
 * Unlike m68k, where arguments are pushed onto the stack with a width of
 * their own (hence the trap_<N>_<letters> names below, one letter per
 * argument: w = word, l = long), pTOS passes the function number in r0
 * and every argument in its own 32-bit register (r1-r4), with
 * svc #1/#13/#14 for GEMDOS/BIOS/XBIOS respectively. That makes the w/l
 * distinction moot here: it only ever affected how a value was pushed,
 * and there is no push left to do. It also means the old m68k comment
 * this file used to carry about gcc mishandling clobbered registers in
 * real inline functions under -mshort doesn't apply on ARM: there is no
 * -mshort here, int is a plain 32-bit type, and the ARM trap ABI needs
 * no stack manipulation at all, so these are ordinary static inline
 * functions rather than statement-expression macros.
 *
 * Only 0-4 arguments are covered. pTOS's BIOS/XBIOS trap entry
 * (_biostrap/_xbiostrap) only forwards r1-r4 to the target handler and
 * does not yet marshal a 5th+ stack-spilled argument, so the handful of
 * calls that need more (Rwabs/Lrwabs, Floprd/Flopwr/Flopfmt/Flopver,
 * Rsconf) are left undefined here until that's addressed on the pTOS
 * side; using one is a compile error rather than a silently wrong call.
 */

#ifndef _MINT_ARCH_ARM_OSBIND_H
#define _MINT_ARCH_ARM_OSBIND_H 1

/* ---- GEMDOS (svc #1) ------------------------------------------------- */

static __inline__ long trap_1_w(short n)
{
	register long r0 __asm__("r0") = n;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	:
	: "r1", "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_ww(short n, short a)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1)
	: "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wl(short n, long a)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1)
	: "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wlw(short n, long a, short b)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2)
	: "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_www(short n, short a, short b)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2)
	: "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wll(short n, long a, long b)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2)
	: "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wwll(short n, short a, long b, long c)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3)
	: "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wlww(short n, long a, short b, short c)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3)
	: "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wlwww(short n, long a, short b, short c, short d)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	register long r4 __asm__("r4") = d;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3), "r"(r4)
	: "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wwlll(short n, short a, long b, long c, long d)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	register long r4 __asm__("r4") = d;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3), "r"(r4)
	: "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wwwll(short n, short a, short b, long c, long d)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	register long r4 __asm__("r4") = d;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3), "r"(r4)
	: "ip", "lr", "cc", "memory");
	return r0;
}

/* ---- BIOS (svc #13) --------------------------------------------------- */

static __inline__ long trap_13_w(short n)
{
	register long r0 __asm__("r0") = n;
	__asm__ volatile ("svc #13"
	: "+r"(r0)
	:
	: "r1", "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_13_ww(short n, short a)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	__asm__ volatile ("svc #13"
	: "+r"(r0)
	: "r"(r1)
	: "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_13_wl(short n, long a)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	__asm__ volatile ("svc #13"
	: "+r"(r0)
	: "r"(r1)
	: "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_13_www(short n, short a, short b)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	__asm__ volatile ("svc #13"
	: "+r"(r0)
	: "r"(r1), "r"(r2)
	: "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_13_wwl(short n, short a, long b)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	__asm__ volatile ("svc #13"
	: "+r"(r0)
	: "r"(r1), "r"(r2)
	: "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

/* trap_13_wwlwww / trap_13_wwlwwwl (Rwabs/Lrwabs's 5/6-argument forms) are
 * not available on ARM yet -- see the file comment above. */

/* ---- XBIOS (svc #14) --------------------------------------------------- */

static __inline__ long trap_14_w(short n)
{
	register long r0 __asm__("r0") = n;
	__asm__ volatile ("svc #14"
	: "+r"(r0)
	:
	: "r1", "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_14_ww(short n, short a)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1)
	: "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_14_wl(short n, long a)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1)
	: "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_14_www(short n, short a, short b)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1), "r"(r2)
	: "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_14_wwl(short n, short a, long b)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1), "r"(r2)
	: "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_14_wllw(short n, long a, long b, short c)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3)
	: "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_14_wwll(short n, short a, long b, long c)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3)
	: "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_14_wwwl(short n, short a, short b, long c)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3)
	: "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_14_wlll(short n, long a, long b, long c)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3)
	: "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_14_wllww(short n, long a, long b, short c, short d)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	register long r4 __asm__("r4") = d;
	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3), "r"(r4)
	: "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_14_wwwwl(short n, short a, short b, short c, long d)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	register long r4 __asm__("r4") = d;
	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3), "r"(r4)
	: "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_14_wlwlw(short n, long a, short b, long c, short d)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	register long r4 __asm__("r4") = d;
	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3), "r"(r4)
	: "ip", "lr", "cc", "memory");
	return r0;
}

/* trap_14_wllwwwww / wllwwwwlw / wllwwwwwlw / wwwwwww (Floprd/Flopwr/
 * Flopfmt/Flopver/Rsconf) are not available on ARM yet -- see the file
 * comment above. */

#endif /* _MINT_ARCH_ARM_OSBIND_H */
