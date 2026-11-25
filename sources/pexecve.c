#include <errno.h>
#include <mint/osbind.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PEXECVE_NATIVE 0x193
#define GEMDOS_EINVFN (-32L)
#define GEMDOS_ENSMEM (-39L)
#define GEMDOS_EIMBA (-40L)
#define GEMDOS_ERANGE (-64L)

#if defined(__m68k__)
#define LEGACY_ENV_MAX 32766U
#define VECTOR_ENTRIES_MAX 1024U

static size_t write_index(char *out, unsigned int index)
{
    char reversed[4];
    size_t count = 0, i;

    do {
        reversed[count++] = '0' + index % 10;
        index /= 10;
    } while (index);
    if (out)
        for (i = 0; i < count; i++)
            out[i] = reversed[count - 1 - i];
    return count;
}

static void legacy_tail(char tail[128], char *const argv[], unsigned int argc)
{
    unsigned int i, used = 1;
    int quoted = 0;

    for (i = 1; i < argc; i++)
        if (!argv[i][0] || strpbrk(argv[i], " \t\r\n'\""))
            quoted = 1;

    /* Keep the conventional text for programs that read the basepage
     * directly. The marker and ARGV block carry the exact arguments. */
    for (i = 1; i < argc && used < 127; i++) {
        const char *arg = argv[i];

        if (i > 1) tail[used++] = ' ';
        if (quoted && used < 127) tail[used++] = '\'';
        while (*arg && used < 127) {
            char ch = *arg++;
            tail[used++] = ch;
            if (quoted && ch == '\'' && used < 127)
                tail[used++] = '\'';
        }
        if (quoted && used < 127) tail[used++] = '\'';
    }
}

/* Extended ARGV is the legacy transport for an explicit argv[0] and empty
 * arguments. Its double-NUL environment cannot represent empty env entries. */
static long legacy_exec(short mode, const char *path, char *const argv[],
                        char *const envp[])
{
    size_t env_size = 0, arg_size = 0, indices = 0, size, n;
    unsigned int argc = 0, envc = 0, i;
    char tail[128] = {127, 0};
    char *block, *out;
    long result;

    if (!argv[0])
        return GEMDOS_ERANGE;
    while (envp[envc]) {
        if (envc == VECTOR_ENTRIES_MAX || !envp[envc][0] ||
            !strncmp(envp[envc], "ARGV=", 5))
            return GEMDOS_ERANGE;
        n = strnlen(envp[envc++], LEGACY_ENV_MAX - env_size);
        if (n == LEGACY_ENV_MAX - env_size)
            return GEMDOS_ERANGE;
        env_size += n + 1;
    }
    while (argv[argc]) {
        if (argc >= VECTOR_ENTRIES_MAX - envc)
            return GEMDOS_ERANGE;
        n = strnlen(argv[argc], LEGACY_ENV_MAX - arg_size);
        if (n == LEGACY_ENV_MAX - arg_size)
            return GEMDOS_ERANGE;
        if (!n)
            indices += (indices ? 1 : 5) + write_index(NULL, argc);
        arg_size += n ? n + 1 : 2; /* nonempty placeholder for NULL:index */
        if (arg_size > LEGACY_ENV_MAX)
            return GEMDOS_ERANGE;
        argc++;
    }
    /* Check before adding on platforms with a narrower size_t. */
    if (indices + 7 > LEGACY_ENV_MAX - env_size ||
        arg_size > LEGACY_ENV_MAX - env_size - indices - 7)
        return GEMDOS_ERANGE;
    size = env_size + indices + arg_size + 7; /* ARGV=, its NUL and final NUL */
    block = malloc(size);
    if (!block)
        return GEMDOS_ENSMEM;
    out = block;
    for (i = 0; i < envc; i++) {
        n = strlen(envp[i]) + 1;
        memcpy(out, envp[i], n);
        out += n;
    }
    memcpy(out, "ARGV=", 5);
    out += 5;
    if (indices) {
        memcpy(out, "NULL:", 5);
        out += 5;
        for (i = 0; i < argc; i++) {
            if (argv[i][0])
                continue;
            out += write_index(out, i);
            *out++ = ',';
        }
        --out;
    }
    *out++ = 0;
    for (i = 0; i < argc; i++) {
        const char *arg = argv[i][0] ? argv[i] : " ";
        n = strlen(arg) + 1;
        memcpy(out, arg, n);
        out += n;
    }
    *out = 0;
    legacy_tail(tail, argv, argc);
    result = Pexec(mode, path, tail, block);
    free(block);
    return result;
}
#endif

long Pexecve(short mode, const char *path, char *const argv[], char *const envp[])
{
    long result;

    if (mode != 0 && mode != 100 && mode != 200)
        return GEMDOS_EINVFN;
    if (!path || !argv || !envp)
        return GEMDOS_EIMBA;
    result = trap_1_wwlll(PEXECVE_NATIVE, mode, (long)path, (long)argv, (long)envp);
#if defined(__m68k__)
    if (result == GEMDOS_EINVFN)
        return legacy_exec(mode, path, argv, envp);
#endif
    return result;
}

int execve(const char *path, char *const argv[], char *const envp[])
{
    long result = Pexecve(200, path, argv, envp);

    /* Old GEMDOS ERANGE differs from the libc errno namespace. */
    switch (result) {
    case GEMDOS_ERANGE: errno = ERANGE; break;
    case GEMDOS_ENSMEM: errno = ENOMEM; break;
    case GEMDOS_EIMBA: errno = EFAULT; break;
    case GEMDOS_EINVFN: errno = ENOSYS; break;
    default: errno = result < 0 ? (int)-result : ENOSYS; break;
    }
    return -1;
}
