/*
 * setstack.S - ARM accessory stack switch
 */

#include "libc-symbols.h"

        .syntax unified
        .text
        .align  2
        .globl  C_SYMBOL_NAME(_setstack)
        .type   C_SYMBOL_NAME(_setstack), %function
C_SYMBOL_NAME(_setstack):
        bic     r0, r0, #7
        mov     sp, r0
        bx      lr
