#include <assert.h>
#include <errno.h>
#include <mint/osbind.h>
#include <string.h>
#include <unistd.h>

static int native_calls, legacy_calls;
static long native_result;
static short expected_mode;
static char *argv[] = {"chosen name", "", "two words", "literal\"quote", "", NULL};
static char *envp[] = {"KEY=two words", "FLAG=", NULL};

long test_trap(short fn, short mode, long path, long a, long b)
{
    assert(mode == expected_mode && !strcmp((const char *)path, "C:\\APP.PRG"));
    if (fn == 0x193) {
        native_calls++;
        assert((char **)a == argv && (char **)b == envp);
        return native_result;
    }
    assert(fn == 0x4b);
    legacy_calls++;
    {
        const char expected[] = "KEY=two words\0FLAG=\0ARGV=NULL:1,4\0chosen name\0 \0"
                                "two words\0literal\"quote\0 \0";
        assert(*(const unsigned char *)a == 127);
        assert(!strcmp((const char *)a + 1, "'' 'two words' 'literal\"quote' ''"));
        assert(!memcmp((const char *)b, expected, sizeof(expected)));
    }
    return mode == 200 ? -40 : 67;
}

int main(void)
{
    const char *path = "C:\\APP.PRG";

    assert(Pexecve(3, NULL, NULL, NULL) == -32);
    assert(Pexecve(0, path, argv, NULL) == -40);
    assert(!native_calls && !legacy_calls);
    native_result = 23;
    assert(Pexecve(0, path, argv, envp) == 23 && native_calls == 1);
    native_result = -33;
    assert(Pexecve(0, path, argv, envp) == -33 && !legacy_calls);
    native_result = -32;
    expected_mode = 100;
#ifdef __m68k__
    assert(Pexecve(100, path, argv, envp) == 67 && legacy_calls == 1);
    envp[1] = "";
    assert(Pexecve(100, path, argv, envp) == -64 && legacy_calls == 1);
    envp[1] = "FLAG=";
    expected_mode = 200;
    assert(execve(path, argv, envp) == -1 && errno == EFAULT && legacy_calls == 2);
#else
    assert(Pexecve(100, path, argv, envp) == -32 && !legacy_calls);
#endif
    expected_mode = 200;
    native_result = -64;
    assert(execve(path, argv, envp) == -1 && errno == ERANGE);
    return 0;
}
