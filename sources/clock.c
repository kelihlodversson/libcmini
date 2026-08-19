#include <time.h>
#include <mint/osbind.h>
#include <mint/mintbind.h>

/*
 * _hz_200 is a normal TOS system variable, not a hardware register: a
 * 200 Hz tick counter the kernel increments and low-memory-privileged
 * code reads directly.
 *
 * On m68k, real TOS software (and this file) reads it directly through
 * its fixed, documented low-memory address, in supervisor mode
 * (Supexec()). pTOS doesn't give ARM programs a fixed address for it at
 * all -- it isn't memory-mapped there, and Supexec() isn't implemented
 * either -- so on ARM it's read through Ssystem(S_GETLVAL, ...) instead,
 * by the same documented address, no supervisor mode required. See
 * kelihlodversson/pTOS#219.
 */
#if defined(__arm__)

clock_t clock(void)
{
	return (clock_t)Ssystem(S_GETLVAL, 0x4baL, 0L);
}

#else

#define _hz_200 ((volatile unsigned long *) 0x4baL)

static long get_clock(void)
{
	return *_hz_200;
}

clock_t clock(void)
{
	return Supexec(get_clock);
}

#endif
