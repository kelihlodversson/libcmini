#if !defined(__m68k__) && (defined(__arm__) || defined(__x86_64__))

#include <mint/osbind.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

#define GEMDOS_EINVFN (-32L)
#define GEMDOS_ENSMEM (-39L)
#define GEMDOS_EIMBA (-40L)
#define GEMDOS_ERANGE (-64L)
#define GEMDOS_EPLFMT (-66L)

#define LEGACY_TAIL_MAX 126U
#define LEGACY_ENV_MAX 32768U
#define VECTOR_ENTRIES_MAX 1024U

static long classic_args(const char *tail, size_t length, char *out, char **argv)
{
    long count = 0;

    while (length && *tail && *tail != '\r') {
        char quote = 0;

        if (*tail == ' ' || *tail == '\t') {
            tail++;
            length--;
            continue;
        }

        *argv++ = out;
        count++;
        while (length && *tail && *tail != '\r') {
            char ch = *tail++;
            length--;
            if (quote) {
                if (ch != quote) {
                    *out++ = ch;
                } else if (length && *tail == quote) {
                    *out++ = *tail++;
                    length--;
                } else {
                    quote = 0;
                }
            } else if (ch == '\'' || ch == '"') {
                quote = ch;
            } else if (ch == ' ' || ch == '\t') {
                break;
            } else {
                *out++ = ch;
            }
        }
        if (quote)
            return GEMDOS_EPLFMT;
        *out++ = '\0';
    }
    *argv = NULL;
    return count;
}

/* Count strings up to the terminating NUL or the ARGV environment marker. */
static long count_strings(const char **cursor, size_t *left, size_t max, int environment)
{
    size_t count = 0;

    while (*left) {
        size_t length;

        if (!**cursor)
            return count;
        length = strnlen(*cursor, *left);
        if (length == *left)
            break;
        if (environment && length >= 5 && !memcmp(*cursor, "ARGV=", 5))
            return count;
        if (count == max)
            break;
        *cursor += length + 1;
        *left -= length + 1;
        count++;
    }
    return GEMDOS_ERANGE;
}

static void fill_vector(char **vector, const char *str, size_t count)
{
    while (count--) {
        *vector++ = (char *)str;
        str += strlen(str) + 1;
    }
    *vector = NULL;
}

static long restore_empty_args(char **argv, size_t argc, const char *indices, char *empty)
{
    for (;;) {
        size_t index = 0;

        if (*indices < '0' || *indices > '9')
            return GEMDOS_EPLFMT;
        do {
            index = 10 * index + (*indices++ - '0');
            if (index >= argc)
                return GEMDOS_EPLFMT;
        } while (*indices >= '0' && *indices <= '9');
        argv[index] = empty;
        if (!*indices)
            return 0;
        if (*indices++ != ',')
            return GEMDOS_EPLFMT;
    }
}

long __libc_pexec(short mode, const char *prog, const char *tail, const char *env)
{
    unsigned int length;
    char text[LEGACY_TAIL_MAX + 1];
    /* At most 63 short-tail arguments, plus argv[0] and the terminator. */
    char *classic[LEGACY_TAIL_MAX / 2 + 2];
    char *empty_env[] = {NULL};
    char **vectors, **argv = classic;
    const char *cursor = env, *payload = NULL, *indices = NULL;
    size_t left = LEGACY_ENV_MAX;
    long envc, argc = 0, classic_argc = 0, result = 0;
    char empty_arg[] = "";

    if (mode != 0 && mode != 100 && mode != 200)
        return GEMDOS_EINVFN;
    if (!prog)
        return GEMDOS_EIMBA;
    length = tail ? (unsigned char)*tail : 0;
    if (length <= LEGACY_TAIL_MAX) {
        classic[0] = (char *)prog;
        classic_argc = classic_args(tail ? tail + 1 : NULL, length, text, classic + 1);
        if (classic_argc < 0)
            return classic_argc;
        classic_argc++;
        if (!env)
            return Pexecve(mode, prog, classic, environ ? environ : empty_env);
    } else if (length != 127 || !env) {
        return GEMDOS_EPLFMT;
    }

    envc = count_strings(&cursor, &left, VECTOR_ENTRIES_MAX - classic_argc, 1);
    if (envc < 0)
        return envc;
    if (length == 127) {
        size_t marker_length;

        if (!*cursor)
            return GEMDOS_EPLFMT;
        marker_length = strlen(cursor);
        if (marker_length != 5) {
            if (strncmp(cursor + 5, "NULL:", 5))
                return GEMDOS_EPLFMT;
            indices = cursor + 10;
        }
        payload = cursor + marker_length + 1;
        cursor = payload;
        left -= marker_length + 1;
        argc = count_strings(&cursor, &left, VECTOR_ENTRIES_MAX - envc, 0);
        if (argc <= 0)
            return argc < 0 ? argc : GEMDOS_EPLFMT;
    }

    vectors = malloc((envc + 1 + (argc ? argc + 1 : 0)) * sizeof(*vectors));
    if (!vectors)
        return GEMDOS_ENSMEM;
    fill_vector(vectors, env, envc);
    if (argc) {
        argv = vectors + envc + 1;
        fill_vector(argv, payload, argc);
        if (indices)
            result = restore_empty_args(argv, argc, indices, empty_arg);
    }
    if (result == 0)
        result = Pexecve(mode, prog, argv, vectors);
    free(vectors);
    return result;
}

#endif
