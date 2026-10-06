/*
 * arch/x86_64/mintbind.h - x86-64 trap primitives for the MiNT-extension
 * GEMDOS bindings in mint/mintbind.h
 *
 * Same register ABI as arch/x86_64/osbind.h (function number in upper 32 bits
 * of rax, arguments in registers, syscall for GEMDOS), but able to go
 * up to 6 arguments instead of 4: unlike BIOS/XBIOS, whose trap entry
 * (bios/arch/x86_64/trap.c's x86_64_trap_dispatch) calls the target
 * handler directly with 4 registers, GEMDOS's entry (bdos/bdosmain.c's
 * osif) reads arguments out of the syscall frame by index, not through
 * a plain call, so it isn't limited to 4.
 *
 * pTOS's x86-64 port uses ILP32 userspace (32-bit int/long/pointer),
 * so the actual values passed are 32-bit even though registers are 64-bit.
 * The kernel's trap dispatcher (bios/arch/x86_64/trap.c) extracts the
 * function number and arguments from the syscall frame.
 */

#ifndef _MINT_ARCH_X86_64_MINTBIND_H
#define _MINT_ARCH_X86_64_MINTBIND_H 1

static __inline__ long trap_1_wwl(short n, short a, long b)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	__asm__ volatile ("xorl %%edx, %%edx\n\txorl %%r10d, %%r10d\n\tsyscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi)
	: "rdx", "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wllw(short n, long a, long b, short c)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	__asm__ volatile ("xorl %%r10d, %%r10d\n\tsyscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx)
	: "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wwlw(short n, short a, long b, short c)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	__asm__ volatile ("xorl %%r10d, %%r10d\n\tsyscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx)
	: "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wwww(short n, short a, short b, short c)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	__asm__ volatile ("xorl %%r10d, %%r10d\n\tsyscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx)
	: "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wwwl(short n, short a, short b, long c)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	__asm__ volatile ("xorl %%r10d, %%r10d\n\tsyscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx)
	: "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wlll(short n, long a, long b, long c)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	__asm__ volatile ("xorl %%r10d, %%r10d\n\tsyscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx)
	: "r10", "r8", "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wlllw(short n, long a, long b, long c, short d)
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

static __inline__ long trap_1_wllll(short n, long a, long b, long c, long d)
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

static __inline__ long trap_1_wwllll(short n, short a, long b, long c, long d, long e)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	register long r10 __asm__("r10") = d;
	register long r8 __asm__("r8") = e;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx), "r"(r10), "r"(r8)
	: "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wlllll(short n, long a, long b, long c, long d, long e)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	register long r10 __asm__("r10") = d;
	register long r8 __asm__("r8") = e;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx), "r"(r10), "r"(r8)
	: "r9", "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wwlllll(short n, short a, long b, long c, long d, long e, long f)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	register long r10 __asm__("r10") = d;
	register long r8 __asm__("r8") = e;
	register long r9 __asm__("r9") = f;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx), "r"(r10), "r"(r8), "r"(r9)
	: "rcx", "r11", "cc", "memory");
	return (long)rax;
}

static __inline__ long trap_1_wllllll(short n, long a, long b, long c, long d, long e, long f)
{
	register unsigned long long rax __asm__("rax") = ((unsigned long long)0x0001 << 32) | (n & 0xFFFF);
	register long rdi __asm__("rdi") = a;
	register long rsi __asm__("rsi") = b;
	register long rdx __asm__("rdx") = c;
	register long r10 __asm__("r10") = d;
	register long r8 __asm__("r8") = e;
	register long r9 __asm__("r9") = f;
	__asm__ volatile ("syscall"
	: "+a"(rax)
	: "D"(rdi), "S"(rsi), "d"(rdx), "r"(r10), "r"(r8), "r"(r9)
	: "rcx", "r11", "cc", "memory");
	return (long)rax;
}

#endif /* _MINT_ARCH_X86_64_MINTBIND_H */
