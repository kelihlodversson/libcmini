#ifndef _MINT_POLL_H
#define _MINT_POLL_H

#include <features.h>
#include <stdint.h>

struct pollfd {
    int32_t fd;
    uint16_t events;
    uint16_t revents;
};
typedef struct pollfd POLLFD;

#define POLLIN      0x001
#define POLLPRI     0x002
#define POLLOUT     0x004
#define POLLERR     0x008
#define POLLHUP     0x010
#define POLLNVAL    0x020
#define POLLRDNORM  0x040
#define POLLRDBAND  0x080
#define POLLWRNORM  0x100
#define POLLWRBAND  0x200
#define POLLMSG     0x400

__BEGIN_DECLS

/* GEMDOS result; a negative return is a kernel error. Timeout is in ms:
 * zero snapshots readiness, -1 waits indefinitely. */
long Fpoll_time64(POLLFD *fds, uint32_t nfds, int64_t timeout_ms);

#if !defined(__m68k__)
/* The legacy adapters use only the canonical pTOS trap. Fselect accepts
 * optional pointers to 32-bit masks and treats a zero timeout as infinite. */
long Fpoll(POLLFD *fds, uint32_t nfds, int32_t timeout_ms);
int Fselect(unsigned short timeout, void *read_mask, void *write_mask, void *except_mask);
#endif

__END_DECLS
#endif
