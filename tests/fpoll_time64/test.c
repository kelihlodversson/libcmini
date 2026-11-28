#include <assert.h>
#include <limits.h>
#include <mint/mintbind.h>
#include <mint/poll.h>
#include <stddef.h>

static int calls, response;
static uint64_t timeout_bits;
static long supplied_count;

long trap_1_wllll(short fn, long address, long nfds, long high, long low)
{
    POLLFD *fds = (void *)(uintptr_t)address;

    assert(fn == 0x194);
    calls++;
    supplied_count = nfds;
    timeout_bits = ((uint64_t)(uint32_t)high << 32) | (uint32_t)low;
    if (!response)
        return 0;
    if (response == POLLERR || response == POLLHUP) {
        assert(nfds == 1 && fds[0].fd == 31 && fds[0].events == POLLPRI);
        fds[0].revents = response;
        return 1;
    }
    if (response == 3) {
        assert(nfds == 1 && fds[0].fd == 5 && fds[0].events == (POLLIN | POLLOUT | POLLPRI));
        fds[0].revents = POLLIN;
        return 1;
    }
    assert(nfds == 2 && fds[0].fd == 5 && fds[1].fd == 31);
    assert(fds[0].events == (POLLIN | POLLOUT) && fds[1].events == POLLPRI);
    if (response < 0)
        return response;
    fds[0].revents = POLLIN | POLLOUT;
    fds[1].revents = response == 1 ? POLLPRI : POLLNVAL;
    return 2;
}

int main(void)
{
    assert(sizeof(POLLFD) == 8 && offsetof(POLLFD, revents) == 6);
    assert(Fpoll_time64(NULL, 0, INT64_MAX) == 0);
    assert(timeout_bits == INT64_MAX && supplied_count == 0);
    assert(Fpoll_time64(NULL, 0, INT64_C(0x100005678)) == 0);
    assert(timeout_bits == UINT64_C(0x100005678));
    assert(Fpoll_time64(NULL, 0, -2) == 0);
    assert(timeout_bits == UINT64_MAX - 1 && calls == 3);

#if !defined(__m68k__)
    {
        uint32_t read_mask = 1U << 5, write_mask = read_mask, except_mask = UINT32_C(1) << 31;

        assert(Fpoll(NULL, 0, INT32_MIN) == 0 && timeout_bits == UINT64_MAX);
        assert(Fpoll(NULL, 0, 0) == 0 && !timeout_bits);
        assert(Fpoll(NULL, 0, INT32_MAX) == 0 && timeout_bits == INT32_MAX);
        assert(Fselect(0, NULL, NULL, NULL) == 0 && timeout_bits == UINT64_MAX);
        response = 1;
        assert(Fselect(65535, &read_mask, &write_mask, &except_mask) == 3);
        assert(timeout_bits == 65535 && read_mask == (1U << 5) && write_mask == read_mask);
        assert(except_mask == (UINT32_C(1) << 31));
        response = -40;
        assert(Fselect(5, &read_mask, &read_mask, &except_mask) == -40);
        assert(read_mask == (1U << 5) && except_mask == (UINT32_C(1) << 31));
        response = 2;
        assert(Fselect(5, &read_mask, &write_mask, &except_mask) == -37);
        assert(!read_mask && !write_mask && !except_mask && calls == 10);

        except_mask = UINT32_C(1) << 31;
        response = POLLERR;
        assert(Fselect(0, NULL, NULL, &except_mask) == 1);
        assert(except_mask == (UINT32_C(1) << 31) && timeout_bits == UINT64_MAX);
        response = POLLHUP;
        assert(Fselect(25, NULL, NULL, &except_mask) == 1);
        assert(except_mask == (UINT32_C(1) << 31) && timeout_bits == 25);

        read_mask = UINT32_C(1) << 5;
        response = 1;
        assert(Fselect(0, &read_mask, &read_mask, &except_mask) == 3);
        assert(read_mask == (UINT32_C(1) << 5) && except_mask == (UINT32_C(1) << 31));
        response = 3;
        assert(Fselect(0, &read_mask, &read_mask, &read_mask) == 1);
        assert(read_mask == (UINT32_C(1) << 5));
    }
#endif
    return 0;
}
