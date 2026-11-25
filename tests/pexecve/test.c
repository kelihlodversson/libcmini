#include <assert.h>
#include <errno.h>
#include <mint/osbind.h>
#include <string.h>

int libcmini_execve(const char *path, char *const argv[], char *const envp[]);

static int native_calls, legacy_calls;
static long native_result;
static short expected_mode;
static char *argv[] = {"chosen name", "", "two words", "literal\"quote", "", NULL};
static char *envp[] = {"KEY=two words", "FLAG=", NULL};

#ifndef __m68k__
extern char **environ;
static const char *const *legacy_argv, *const *legacy_envp;

static void check_vector(char *const *actual, const char *const *expected)
{
    for (; *expected; expected++, actual++)
        assert(*actual && !strcmp(*actual, *expected));
    assert(!*actual);
}
#endif

long test_trap(short fn, short mode, long path, long a, long b)
{
    assert(mode == expected_mode && !strcmp((const char *)path, "C:\\APP.PRG"));
    if (fn == 0x193) {
        native_calls++;
#ifndef __m68k__
        if (legacy_argv) {
            check_vector((char **)a, legacy_argv);
            check_vector((char **)b, legacy_envp);
            return native_result;
        }
#endif
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

#ifndef __m68k__
static void test_legacy_wrapper(const char *path)
{
    static const char text[] = "\t'two words' \"double quoted\" '' 'O''Brien' plain";
    static const char *const classic[] = {
        "C:\\APP.PRG", "two words", "double quoted", "", "O'Brien", "plain", NULL
    };
    static const char *const inherited[] = {"INHERITED=1", NULL};
    static const char *const explicit_env[] = {"KEY=two words", "FLAG=", NULL};
    static const char *const extended[] = {"chosen name", "", "two words", "", NULL};
    static const char *const path_only[] = {"C:\\APP.PRG", NULL};
    static const char *const empty[] = {NULL};
    static const char block[] = "KEY=two words\0FLAG=\0ARGV=NULL:1,3\0chosen name\0 \0two words\0 \0";
    static char *current_env[] = {"INHERITED=1", NULL};
    char **saved_environ = environ;
    char tail[sizeof(text)], full_tail[127], oversized[32768];
    const char *max_argv[65];
    int i, calls;

    tail[0] = sizeof(text) - 1;
    memcpy(tail + 1, text, sizeof(text) - 1);
    environ = current_env;
    expected_mode = 0;
    native_result = 37;
    legacy_argv = classic;
    legacy_envp = inherited;
    assert(Pexec(0, path, tail, 0L) == 37);
    assert(environ == current_env);

    legacy_envp = explicit_env;
    assert(Pexec(0, path, tail, block) == 37);
    legacy_argv = extended;
    expected_mode = 100;
    native_result = 654;
    assert(Pexec(100, path, "\177", block) == 654);
    expected_mode = 200;
    native_result = -32;
    assert(Pexec(200, path, "\177", block) == -32);

    expected_mode = 0;
    legacy_argv = path_only;
    legacy_envp = empty;
    native_result = -33;
    assert(Pexec(0, path, "", "") == -33);
    environ = NULL;
    assert(Pexec(0, path, NULL, NULL) == -33);
    environ = saved_environ;

    full_tail[0] = 126;
    max_argv[0] = path;
    for (i = 0; i < 63; i++) {
        full_tail[2 * i + 1] = 'a';
        full_tail[2 * i + 2] = ' ';
        max_argv[i + 1] = "a";
    }
    max_argv[64] = NULL;
    legacy_argv = max_argv;
    assert(Pexec(0, path, full_tail, "\0") == -33);

    calls = native_calls;
    assert(Pexec(5, 0L, 1L, 1L) == -32);
    assert(Pexec(106, 0L, &calls, 0L) == -32);
    assert(Pexec(0, NULL, 1L, 1L) == -40);
    assert(Pexec(0, path, "\200", NULL) == -66);
    assert(Pexec(0, path, "\002'X", NULL) == -66);
    assert(Pexec(0, path, "\177", "KEY=1\0") == -66);
    assert(Pexec(0, path, "\177", "ARGV=\0") == -66);
    assert(Pexec(0, path, "\177", "ARGV=NULL:0,\0APP\0") == -66);
    assert(Pexec(0, path, "\177", "ARGV=NULL:10\0APP\0") == -66);
    memset(oversized, 'a', sizeof(oversized));
    assert(Pexec(0, path, "", oversized) == -64);
    assert(native_calls == calls && !legacy_calls);
}
#endif

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
    assert(libcmini_execve(path, argv, envp) == -1 && errno == EFAULT && legacy_calls == 2);
#else
    assert(Pexecve(100, path, argv, envp) == -32 && !legacy_calls);
#endif
    expected_mode = 200;
    native_result = -64;
    assert(libcmini_execve(path, argv, envp) == -1 && errno == ERANGE);
#ifndef __m68k__
    test_legacy_wrapper(path);
#endif
    return 0;
}
