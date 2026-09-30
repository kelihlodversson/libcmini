/*
 * arch/x86_64/osbind.h - x86-64 trap primitives for GEMDOS/BIOS/XBIOS bindings
 *
 * mint/osbind.h includes this (instead of arch/m68k/osbind.h or
 * arch/arm/osbind.h) on x86_64 and uses these to build the same
 * GEMDOS/BIOS/XBIOS function definitions as on m68k and ARM.
 *
 * pTOS's x86-64 trap entry uses the `syscall` instruction for GEMDOS calls
 * (trap #1 on m68k, svc #1 on ARM). The kernel installs the syscall
 * entry point via IA32_LSTAR MSR (bios/arch/x86_64/trap.c).
 *
 * Unlike m68k, where arguments are pushed onto the stack with a width of
 * their own, and unlike ARM which passes 4 arguments in 32-bit registers,
 * x86-64 passes arguments in 64-bit registers: rdi, rsi, rdx, r10, r8, r9.
 * However, since pTOS uses ILP32 userspace (32-bit int/long/pointer),
 * the actual values passed are 32-bit. The kernel's trap dispatcher
 * (bios/arch/x86_64/trap.c) extracts the function number and arguments
 * from the syscall frame.
 *
 * Most calls take 0-4 arguments, covered directly by the trap dispatcher.
 * The handful that need more (Rwabs/Lrwabs, Floprd/Flopwr/Flopfmt/Flopver,
 * Rsconf) instead pass a single pointer to a packed argument struct
 * (biosargs.h) as their one real argument.
 */

#ifndef _MINT_ARCH_X86_64_OSBIND_H
#define _MINT_ARCH_X86_64_OSBIND_H 1

#include "biosargs.h"

/* ---- GEMDOS (syscall with rax = 0x00000001 << 32 | function_number) --- */

static __inline__ long trap_1_w(short n)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	__asm__ volatile ("syscall"
	: "+a"(rax)
	:
	: "rdi", "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_ww(short n, short a)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi)
	: "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wl(short n, long a)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi)
	: "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wlw(short n, long a, short b)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi)
	: "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_www(short n, short a, short b)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi)
	: "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wll(short n, long a, long b)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi)
	: "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wwll(short n, short a, long b, long c)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx)
	: "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wlww(short n, long a, short b, short c)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx)
	: "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wlwww(short n, long a, short b, short c, short d)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	register long r10 __asm__("r10") = d;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx), "r"(r10)
	: "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wwlll(short n, short a, long b, long c, long d)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	register long r10 __asm__("r10") = d;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx), "r"(r10)
	: "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wwwll(short n, short a, short b, long c, long d)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	register long r10 __asm__("r10") = d;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx), "r"(r10)
	: "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

/* ---- BIOS (syscall with rax = 0x0000000D << 32 | function_number) --- */

static __inline__ long trap_13_w(short n)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000D << 32) | (n & 0xFFFF);
	__asm__ volatile ("syscall"
	: "+a"(rax)
	:
	: "rdi", "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_13_ww(short n, short a)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000D << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi)
	: "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_13_wl(short n, long a)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000D << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi)
	: "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_13_www(short n, short a, short b)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000D << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi)
	: "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_13_wwl(short n, short a, long b)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000D << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi)
	: "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

/*
 * Rwabs/Lrwabs (BIOS function 0x04) need 6 real arguments;
 * pass them via a bios_lrwabs_args struct instead (biosargs.h).
 */
static __inline__ long trap_13_wwlwww(short n, short a, long b, short c, short d, short e)
{
	struct bios_lrwabs_args args;
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000D << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = (long)&args;

	args.r_w = a;
	args.adr = (void *)b;
	args.numb = c;
	args.first = d;
	args.drive = e;
	args.lfirst = 0;

	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi)
	: "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_13_wwlwwwl(short n, short a, long b, short c, short d, short e, long f)
{
	struct bios_lrwabs_args args;
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000D << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = (long)&args;

	args.r_w = a;
	args.adr = (void *)b;
	args.numb = c;
	args.first = d;
	args.drive = e;
	args.lfirst = f;

	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi)
	: "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

/* ---- XBIOS (syscall with rax = 0x0000000E << 32 | function_number) --- */

static __inline__ long trap_14_w(short n)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	__asm__ volatile ("syscall"
	: "+a"(rax)
	:
	: "rdi", "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_14_ww(short n, short a)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi)
	: "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_14_wl(short n, long a)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi)
	: "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_14_www(short n, short a, short b)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi)
	: "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_14_wwl(short n, short a, long b)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi)
	: "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_14_wllw(short n, long a, long b, short c)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx)
	: "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_14_wwll(short n, short a, long b, long c)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx)
	: "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_14_wwwl(short n, short a, short b, long c)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx)
	: "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_14_wlll(short n, long a, long b, long c)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx)
	: "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_14_wllww(short n, long a, long b, short c, short d)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	register long r10 __asm__("r10") = d;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx), "r"(r10)
	: "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_14_wwwwl(short n, short a, short b, short c, long d)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	register long r10 __asm__("r10") = d;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx), "r"(r10)
	: "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_14_wlwlw(short n, long a, short b, long c, short d)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	register long r10 __asm__("r10") = d;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx), "r"(r10)
	: "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
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
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = (long)&args;

	args.buf = (void *)a;
	args.filler = b;
	args.dev = c;
	args.sect = d;
	args.track = e;
	args.side = f;
	args.count = g;

	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi)
	: "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

/*
 * Flopfmt (XBIOS 0x0a) needs 9 real arguments; pass them via a
 * xbios_flopfmt_args struct instead (biosargs.h).
 */
static __inline__ long trap_14_wllwwwwwlw(short n, long a, long b, short c, short d, short e, short f, short g, long h, short i)
{
	struct xbios_flopfmt_args args;
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = (long)&args;

	args.buf = (void *)a;
	args.skew = (void *)b;
	args.dev = c;
	args.spt = d;
	args.track = e;
	args.side = f;
	args.interlv = g;
	args.magic = h;
	args.virgin = i;

	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi)
	: "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

/*
 * Rsconf (XBIOS 0x0f) needs 6 real arguments; pass them via a
 * xbios_rsconf_args struct instead (biosargs.h).
 */
static __inline__ long trap_14_wwwwwww(short n, short a, short b, short c, short d, short e, short f)
{
	struct xbios_rsconf_args args;
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x000E << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = (long)&args;

	args.baud = a;
	args.ctrl = b;
	args.ucr = c;
	args.rsr = d;
	args.tsr = e;
	args.scr = f;

	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi)
	: "rsi", "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

/*
 * Safe binding to switch back from supervisor to user mode.
 * On x86-64, Super() is handled via syscall with function 0x20.
 * We need to back up the stack pointer before the call and restore it
 * after, all within one atomic asm block.
 */
static __inline__ void SuperToUser(long ptr)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | 0x20;
	register long rdi __asm__("rdi") = ptr;
	register long rsi __asm__("rsi");
	__asm__ volatile (
		"movq	%%rsp, %0\n"
		"syscall\n"
		"movq	%0, %%rsp\n"
	: "+a"(rax), "=r"(rsi)
	: "D"(rdi)
	: "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
}

#endif /* _MINT_ARCH_X86_64_OSBIND_H */
