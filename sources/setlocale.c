#if defined(__x86_64__)
/* libcmini ships no locale.h of its own, and the x32 build has no
 * freestanding toolchain sysroot to supply one (the host's glibc one needs
 * bits/locale.h), so declare the one function this file defines. */
char *setlocale(int category, const char *locale);
#else
#include "locale.h"
#endif
#include "string.h"

/*
 * this is basically a dummy function as standard TOS doesn't 
 * know about locales
 */
char *setlocale(int category, const char *locale)
{
    static char slocale[100];

    strncpy(slocale, locale, 100);

    return slocale;
}

