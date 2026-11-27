#include <mint/mintbind.h>
#include <mint/poll.h>
#include <string.h>

#define GEMDOS_FPOLL_TIME64 0x194

typedef char pollfd_abi_size[sizeof(POLLFD) == 8 ? 1 : -1];

long Fpoll_time64(POLLFD *fds, uint32_t nfds, int64_t timeout_ms)
{
    uint64_t bits = (uint64_t)timeout_ms;

#if defined(__x86_64__) && defined(__ILP32__)
    /* The native kernel examines full register slots. The x32 function
     * calling convention need not clear the high halves of its arguments. */
    register uint64_t call __asm__("rax") = (UINT64_C(1) << 32) | GEMDOS_FPOLL_TIME64;
    register uint64_t low __asm__("r10") = (uint32_t)bits;
    uint64_t address = (uintptr_t)fds;

    __asm__ volatile ("movl %k1, %k1\n\tsyscall" : "+a"(call), "+D"(address)
                      : "S"((uint64_t)nfds), "d"(bits >> 32), "r"(low)
                      : "r8", "r9", "rcx", "r11", "cc", "memory");
    return (long)call;
#else
    return trap_1_wllll(GEMDOS_FPOLL_TIME64, (long)(uintptr_t)fds, (long)nfds,
                        (long)(uint32_t)(bits >> 32), (long)(uint32_t)bits);
#endif
}

#if !defined(__m68k__)
long Fpoll(void *fds, unsigned long nfds, long timeout_ms)
{
    int32_t interval = (int32_t)timeout_ms;

    return Fpoll_time64(fds, (uint32_t)nfds, interval < 0 ? -1 : interval);
}

int Fselect(unsigned short timeout, void *read_mask, void *write_mask, void *except_mask)
{
    static const uint16_t event[3] = { POLLIN, POLLOUT, POLLPRI };
    void *const output[3] = { read_mask, write_mask, except_mask };
    uint32_t masks[3] = {0}, requested, i, j, nfds = 0;
    POLLFD fds[32];
    long result;

    /* Copy all inputs before any output, including aliased bitmaps. */
    for (i = 0; i < 3; i++)
        if (output[i])
            memcpy(&masks[i], output[i], sizeof(masks[i]));

    requested = masks[0] | masks[1] | masks[2];
    for (i = 0; i < 32; i++) {
        uint32_t bit = UINT32_C(1) << i;

        if (!(requested & bit))
            continue;
        fds[nfds].fd = i;
        fds[nfds].events = 0;
        fds[nfds].revents = 0;
        for (j = 0; j < 3; j++)
            if (masks[j] & bit)
                fds[nfds].events |= event[j];
        nfds++;
    }

    result = Fpoll_time64(fds, nfds, timeout ? (int64_t)timeout : -1);
    if (result < 0)
        return (int)result;

    result = 0;
    masks[0] = masks[1] = masks[2] = 0;
    for (i = 0; i < nfds; i++) {
        if (fds[i].revents & POLLNVAL) {
            masks[0] = masks[1] = masks[2] = 0;
            result = -37; /* GEMDOS EIHNDL */
            break;
        }
        for (j = 0; j < 3; j++) {
            uint16_t ready = event[j] | (j < 2 ? POLLERR | POLLHUP : 0);

            if ((fds[i].events & event[j]) && (fds[i].revents & ready)) {
                masks[j] |= UINT32_C(1) << fds[i].fd;
                result++;
            }
        }
    }
    for (i = 0; i < 3; i++)
        if (output[i])
            memcpy(output[i], &masks[i], sizeof(masks[i]));
    return (int)result;
}
#endif
