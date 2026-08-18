/*
 * biosargs.h - argument-marshaling structs for wide-argument ARM BIOS/XBIOS calls
 *
 * pTOS's ARM trap entry (_biostrap/_xbiostrap in bios/arch/arm/vectorsasm.S)
 * only delivers 4 real arguments in registers. The handful of BIOS/XBIOS
 * calls that need more than that (Rwabs/Lrwabs, Floprd/Flopwr/Flopver,
 * Flopfmt, Rsconf) instead take a pointer to one of these structs as
 * their single real argument, on ARM only.
 *
 * This is part of the trap ABI shared with pTOS's own copy of this file
 * (bios/arch/arm/biosargs.h in the pTOS repo) -- keep the field order and
 * types in sync between the two. See
 * https://github.com/kelihlodversson/libcmini/issues/4 and
 * https://github.com/kelihlodversson/pTOS/issues/217.
 */

#ifndef _MINT_ARCH_ARM_BIOSARGS_H
#define _MINT_ARCH_ARM_BIOSARGS_H 1

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

#endif /* _MINT_ARCH_ARM_BIOSARGS_H */
