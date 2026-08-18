/*
 * arch/arm/mintbind.h - ARM trap primitives for the MiNT-extension
 * GEMDOS bindings in mint/mintbind.h
 *
 * Same register ABI as arch/arm/osbind.h (function number in r0,
 * arguments in registers, svc #1 for GEMDOS -- see that file's comment
 * for the full reasoning), but able to go up to 6 arguments instead of
 * 4: unlike BIOS/XBIOS, whose trap entry (_biostrap/_xbiostrap in
 * pTOS's bios/arch/arm/vectorsasm.S) calls the target handler directly
 * with a plain 4-register AAPCS call, GEMDOS's entry (_enter in pTOS's
 * bdos/arch/arm/rwa.S) stores all of r0-r7 to a stack block and hands
 * BDOS's osif() a pointer to it -- and osif() (bdos/bdosmain.c) reads
 * arguments out of that block by index ("pw[N]"), not through a plain
 * AAPCS call, so it isn't limited to 4. See
 * https://github.com/kelihlodversson/libcmini/issues/1.
 */

#ifndef _MINT_ARCH_ARM_MINTBIND_H
#define _MINT_ARCH_ARM_MINTBIND_H 1

static __inline__ long trap_1_wwl(short n, short a, long b)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2)
	: "r3", "r4", "r5", "r6", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wllw(short n, long a, long b, short c)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3)
	: "r4", "r5", "r6", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wwlw(short n, short a, long b, short c)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3)
	: "r4", "r5", "r6", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wwww(short n, short a, short b, short c)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3)
	: "r4", "r5", "r6", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wwwl(short n, short a, short b, long c)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3)
	: "r4", "r5", "r6", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wlll(short n, long a, long b, long c)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3)
	: "r4", "r5", "r6", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wlllw(short n, long a, long b, long c, short d)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	register long r4 __asm__("r4") = d;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3), "r"(r4)
	: "r5", "r6", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wllll(short n, long a, long b, long c, long d)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	register long r4 __asm__("r4") = d;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3), "r"(r4)
	: "r5", "r6", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wwllll(short n, short a, long b, long c, long d, long e)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	register long r4 __asm__("r4") = d;
	register long r5 __asm__("r5") = e;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5)
	: "r6", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wlllll(short n, long a, long b, long c, long d, long e)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	register long r4 __asm__("r4") = d;
	register long r5 __asm__("r5") = e;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5)
	: "r6", "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wwlllll(short n, short a, long b, long c, long d, long e, long f)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	register long r4 __asm__("r4") = d;
	register long r5 __asm__("r5") = e;
	register long r6 __asm__("r6") = f;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5), "r"(r6)
	: "ip", "lr", "cc", "memory");
	return r0;
}

static __inline__ long trap_1_wllllll(short n, long a, long b, long c, long d, long e, long f)
{
	register long r0 __asm__("r0") = n;
	register long r1 __asm__("r1") = a;
	register long r2 __asm__("r2") = b;
	register long r3 __asm__("r3") = c;
	register long r4 __asm__("r4") = d;
	register long r5 __asm__("r5") = e;
	register long r6 __asm__("r6") = f;
	__asm__ volatile ("svc #1"
	: "+r"(r0)
	: "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5), "r"(r6)
	: "ip", "lr", "cc", "memory");
	return r0;
}

#endif /* _MINT_ARCH_ARM_MINTBIND_H */
