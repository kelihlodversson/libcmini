/* Standard pTOS ELF entry. The legacy CRT remains usable for PRG and
 * basepage entry while native callers migrate to Pexecve. */
#if defined(__ELF__)
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <mint/mintbind.h>
#include "lib.h"

#define AT_BASE 7
#define AT_EXECFN 31
#define AT_PTOS_BASEPAGE 0x5000
#define AT_PTOS_ENTRY_TYPE 0x5006
#define PTOS_ENTRY_ACCESSORY 1

extern int main(int, char **, char **);
extern long _stksize;
const char *__libc_execfn;
static int startup_ready;
static int has_interpreter;
static uint32_t startup_entry_type;

uint32_t __CDECL __ptos_entry_type(void);

uint32_t __CDECL __ptos_entry_type(void)
{
    return startup_entry_type;
}

static void no_startup(void) __attribute__((noreturn));
static void no_startup(void)
{
    _exit(127);
}

void __CDECL __libc_elf_publish_vectors(uint32_t argc, char **argv, char **envp);

void __CDECL __libc_elf_publish_vectors(uint32_t argc, char **argv, char **envp)
{
    const uint32_t *aux;
    char **last = envp;
    unsigned int i;
    uint32_t entry_type = 0;

    if (startup_ready)
        return;
    for (i = 0; i < 16384 && *last; i++, last++)
        ;
    if (i == 16384)
        no_startup();
    aux = (const uint32_t *)(last + 1);
    for (i = 0; i < 32 && aux[0]; i++, aux += 2) {
        switch (aux[0]) {
        case AT_BASE: has_interpreter = aux[1] != 0; break;
        case AT_EXECFN: __libc_execfn = (const char *)(uintptr_t)aux[1]; break;
        case AT_PTOS_BASEPAGE: _base = (BASEPAGE *)(uintptr_t)aux[1]; break;
        case AT_PTOS_ENTRY_TYPE: entry_type = aux[1]; break;
        }
    }
    if (i == 32 || !_base || entry_type > PTOS_ENTRY_ACCESSORY)
        no_startup();
    startup_entry_type = entry_type;
    _app = entry_type != PTOS_ENTRY_ACCESSORY;
    __libc_argc = argc;
    __libc_argv = argv;
    environ = envp;
    if (argc) {
        char *short_name = argv[0];
        char *p;

        for (p = short_name; *p; p++)
            if (*p == '/' || *p == '\\')
                short_name = p + 1;
        program_invocation_name = argv[0];
        program_invocation_short_name = short_name;
    }
    startup_ready = 1;
}

void __CDECL __libc_start_elf(const uint32_t *stack, ExitFn finalizer)
    __attribute__((noreturn));

void __CDECL __libc_start_elf(const uint32_t *stack, ExitFn finalizer)
{
    uint32_t argc = *stack++;
    char **argv = (char **)stack;

    if (argc > 16384 || argv[argc])
        no_startup();
    __libc_elf_publish_vectors(argc, argv, argv + argc + 1);
    (void)Pdomain(1);
    if (_stksize > 0)
        (void)Psetlimit(PSETLIMIT_STACK, _stksize);
    if (has_interpreter) {
        if (!finalizer)
            no_startup();
        __libc_rtld_fini = finalizer;
    }
#ifdef __GCC_HAVE_INITFINI_ARRAY_SUPPORT
    else
        __main();
#endif
    exit(main(argc, argv, environ));
}
#endif
