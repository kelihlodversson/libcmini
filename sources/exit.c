/*
 * exit.c
 *
 *  Created on: 31.05.2013
 *      Author: mfro
 */

#include <stdlib.h>
#include <stdio.h>
#include <mint/osbind.h>
#include <mint/mintbind.h>
#include "lib.h"

ExitFn *_at_exit = NULL;
int _num_at_exit = 0;

void exit(int status)
{
    long result;
    /* Pop before invoking: recursive exit cannot execute a callback twice.
     * Keep the allocation base intact if a callback registers another one. */
    while (_num_at_exit) {
        ExitFn function = _at_exit[--_num_at_exit];
        function();
    }

    result = Ptermgrp(status);
    if (result == -32L || result == -25L)
        (void)Pterm(status);

    /* Successful termination never returns. An unexpected kernel response
     * (including a returning legacy Pterm) must not return from exit or claim
     * group termination succeeded. Fail stopped, yielding cooperatively. */
    for (;;)
        (void)Syield();
}
