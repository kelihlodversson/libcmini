/*
 * biosargs.h - argument-marshaling structs for wide-argument x86-64 BIOS/XBIOS calls
 *
 * pTOS's x86-64 trap entry (bios/arch/x86_64/trap.c's x86_64_trap_dispatch)
 * only delivers 4 real arguments in registers (rdi, rsi, rdx, r10). The
 * handful of BIOS/XBIOS calls that need more than that (Rwabs/Lrwabs,
 * Floprd/Flopwr/Flopfmt/Flopver, Rsconf) instead take a pointer to one
 * of these structs as their single real argument, on x86-64 only.
 *
 * This is part of the trap ABI shared with pTOS's own copy of this file
 * (bios/arch/x86_64/biosargs.h in the pTOS repo) -- keep the field order
 * and types in sync between the two. See pTOS issue #329.
 *
 * Note: On x86-64 ILP32, pointers are 32-bit but the kernel is LP64.
 * The struct fields below use the same types as the ARM version,
 * since libcmini's userspace is ILP32 (32-bit pointers).
 */

#ifndef _MINT_ARCH_X86_64_BIOSARGS_H
#define _MINT_ARCH_X86_64_BIOSARGS_H 1

struct bios_lrwabs_args         /* Rwabs/Lrwabs -- BIOS function 0x04 */
{
	long r_w;
	void *adr;
	long numb;
	long first;
	long drive;
	long lfirst;
};

struct xbios_flop_io_args       /* Floprd/Flopwr/Flopver -- XBIOS 0x08/0x09/0x13 */
{
	void *buf;
	long filler;
	long dev;
	long sect;
	long track;
	long side;
	long count;
};

struct xbios_flopfmt_args       /* Flopfmt -- XBIOS 0x0a */
{
	void *buf;
	void *skew;
	long dev;
	long spt;
	long track;
	long side;
	long interlv;
	long magic;
	long virgin;
};

struct xbios_rsconf_args        /* Rsconf -- XBIOS 0x0f */
{
	long baud;
	long ctrl;
	long ucr;
	long rsr;
	long tsr;
	long scr;
};

#endif /* _MINT_ARCH_X86_64_BIOSARGS_H */
