#include <time.h>
#include <mint/osbind.h>

/*
 * _hz_200 is a normal TOS system variable, not a hardware register: a
 * 200 Hz tick counter the kernel increments and low-memory-privileged
 * code reads directly. It lives at a fixed, documented address, but
 * that address is part of each kernel's own memory layout, not a
 * portable constant -- pTOS lays the whole low-memory system variable
 * table out differently on ARM than on m68k (every LONG on a 4-byte
 * boundary, to avoid alignment faults on STRD/LDRD/VSTR/VLDR; see the
 * comment above the "#if ARCH_ARM" branch in pTOS's tosvars.ld), so
 * _hz_200 itself moves from 0x4ba to 0x4c0.
 */
#if defined(__arm__)
# define _hz_200               ((volatile unsigned long *) 0x4c0L)
#else
# define _hz_200               ((volatile unsigned long *) 0x4baL)
#endif


static long get_clock(void)
{
	return *_hz_200;
}


clock_t clock(void)
{
	return Supexec(get_clock);
}

