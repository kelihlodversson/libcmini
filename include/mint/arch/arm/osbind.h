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
 * Most calls take 0-4 arguments, covered directly by pTOS's BIOS/XBIOS
 * trap entry (_biostrap/_xbiostrap), which only forwards r1-r4 to the
 * target handler. The handful that need more (Rwabs/Lrwabs, Floprd/
 * Flopwr/Flopfmt/Flopver, Rsconf) instead pass a single pointer to a
 * packed argument struct (biosargs.h) as their one real argument -- see
 * https://github.com/kelihlodversson/pTOS/issues/217.
 */

#ifndef _MINT_ARCH_ARM_OSBIND_H
#define _MINT_ARCH_ARM_OSBIND_H 1

#include "biosargs.h"

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

/*
 * Rwabs/Lrwabs (BIOS function 0x04, lrwabs() on the pTOS side) need 6 real
 * arguments; pass them via a bios_lrwabs_args struct instead (biosargs.h).
 */
static __inline__ long trap_13_wwlwww(short n, short a, long b, short c, short d, short e)
{
	struct bios_lrwabs_args args;
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1");

	args.r_w = a;
	args.adr = (void *)b;
	args.numb = c;
	args.first = d;
	args.drive = e;
	args.lfirst = 0;
	r1 = (long)&args;

	__asm__ volatile ("svc #13"
	: "+r"(r0)
	: "r"(r1)
	: "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_13_wwlwwwl(short n, short a, long b, short c, short d, short e, long f)
{
	struct bios_lrwabs_args args;
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1");

	args.r_w = a;
	args.adr = (void *)b;
	args.numb = c;
	args.first = d;
	args.drive = e;
	args.lfirst = f;
	r1 = (long)&args;

	__asm__ volatile ("svc #13"
	: "+r"(r0)
	: "r"(r1)
	: "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

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

/*
 * Floprd/Flopwr/Flopver (XBIOS 0x08/0x09/0x13) need 7 real arguments;
 * pass them via a xbios_flop_io_args struct instead (biosargs.h). All
 * three share this same wire shape, differing only in the function
 * number and their real handler's read/write direction.
 */
static __inline__ long trap_14_wllwwwww(short n, long a, long b, short c, short d, short e, short f, short g)
{
	struct xbios_flop_io_args args;
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1");

	args.buf = (void *)a;
	args.filler = b;
	args.dev = c;
	args.sect = d;
	args.track = e;
	args.side = f;
	args.count = g;
	r1 = (long)&args;

	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1)
	: "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

/*
 * Flopfmt (XBIOS 0x0a) needs 9 real arguments; pass them via a
 * xbios_flopfmt_args struct instead (biosargs.h).
 */
static __inline__ long trap_14_wllwwwwwlw(short n, long a, long b, short c, short d, short e, short f, short g, long h, short i)
{
	struct xbios_flopfmt_args args;
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1");

	args.buf = (void *)a;
	args.skew = (void *)b;
	args.dev = c;
	args.spt = d;
	args.track = e;
	args.side = f;
	args.interlv = g;
	args.magic = h;
	args.virgin = i;
	r1 = (long)&args;

	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1)
	: "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

/*
 * Rsconf (XBIOS 0x0f) needs 6 real arguments; pass them via a
 * xbios_rsconf_args struct instead (biosargs.h).
 */
static __inline__ long trap_14_wwwwwww(short n, short a, short b, short c, short d, short e, short f)
{
	struct xbios_rsconf_args args;
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1");

	args.baud = a;
	args.ctrl = b;
	args.ucr = c;
	args.rsr = d;
	args.tsr = e;
	args.scr = f;
	r1 = (long)&args;

	__asm__ volatile ("svc #14"
	: "+r"(r0)
	: "r"(r1)
	: "r2", "r3", "r4", "ip", "lr", "cc", "memory");
	return r0;
}

/*
 * Safe binding to switch back from supervisor to user mode. Mirrors
 * the m68k version's defensiveness (see arch/m68k/osbind.h's own
 * __m68k_SuperToUser): back up sp before the call and restore it
 * after, all within one atomic asm block, so there's no window for
 * the compiler to have changed sp between two separate C-level calls
 * the way plain Super(0) followed by Super(oldssp) would allow. Not
 * confirmed necessary on pTOS's ARM Super() (0x20) handler
 * specifically, but costs nothing and matches the existing safety
 * margin instead of assuming it away.
 */
static __inline__ void SuperToUser(long ptr)
{
	register long r0 __asm__("r0") = 0x20;
	register long r1 __asm__("r1") = ptr;
	register long sp_backup __asm__("r2");
	__asm__ volatile (
		"mov	r2, sp\n"
		"svc	#1\n"
		"mov	sp, r2\n"
	: "+r"(r0), "=r"(sp_backup)
	: "r"(r1)
	: "r3", "r4", "ip", "lr", "cc", "memory");
}

#endif /* _MINT_ARCH_ARM_OSBIND_H */
