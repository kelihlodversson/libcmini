#!/usr/bin/env python3
"""Exercise production exit with nonreturning syscall models, not target traps."""
from pathlib import Path
import os, shlex, subprocess, tempfile
root = Path(__file__).resolve().parents[2]
with tempfile.TemporaryDirectory() as tmp:
    path = Path(tmp)
    (path/'mint').mkdir()
    (path/'mint/osbind.h').write_text('void mock_term(short);\n#define Pterm(s) mock_term((short)(s))\n')
    (path/'mint/mintbind.h').write_text('long mock_group(short);\nlong mock_yield(void);\n#define Ptermgrp(s) mock_group((short)(s))\n#define Syield() mock_yield()\n')
    (path/'test.c').write_text(r'''
#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
typedef void (*ExitFn)(void);
extern ExitFn *_at_exit;
extern int _num_at_exit;
void ptos_exit(int) __attribute__((noreturn));
static jmp_buf done;
static long response;
static int groups, terms, yields, callbacks, recurse, registering, seen;
static ExitFn *handlers;
int ptos_atexit(ExitFn);
static short argument;
long mock_group(short status) {
    argument=status; groups++;
    if (response==1234) longjmp(done,1); /* real successful call cannot return */
    return response;
}
void mock_term(short status) { assert(status==argument); terms++; longjmp(done,2); }
long mock_yield(void) {
    yields++;
    if(response==-99 && yields<4) return -32; /* old kernel lacks Syield */
    longjmp(done,3);
}
static void first(void) { assert(callbacks==1); callbacks++; }
static void second(void) { assert(callbacks==0); callbacks++; if(registering) assert(ptos_atexit(first)==0); if(recurse) ptos_exit(8); }
int main(void) {
    const long results[]={1234,-32,-25,-36,0,7,-99};
    static int i;
    for(i=0;i<9;i++) {
        response=(i>=7)?1234:results[i%7]; recurse=(i==7); registering=(i==8); groups=terms=yields=callbacks=0;
        handlers=malloc(2*sizeof(*handlers)); assert(handlers);
        handlers[0]=registering?second:first; handlers[1]=second;
        _at_exit=handlers; _num_at_exit=registering?1:2;
        seen=setjmp(done);
        if(!seen) { ptos_exit(0x2345); assert(0); }
        assert(groups==1 && callbacks==2 && _num_at_exit==0 && _at_exit==NULL);
        assert(argument==(recurse?8:0x2345));
        if(response==1234) assert(seen==1 && !terms && !yields);
        else if(response==-32 || response==-25) assert(seen==2 && terms==1 && !yields);
        else assert(seen==3 && yields==(response==-99?4:1) && !terms);
    }
    return 0;
}
''')
    target=path/'test'
    subprocess.run(shlex.split(os.environ.get('HOSTCC','cc'))+['-std=gnu90','-O2','-Wall','-Wextra','-Werror','-fno-builtin','-Dexit=ptos_exit','-Datexit=ptos_atexit','-I'+str(path),str(root/'sources/exit.c'),str(root/'sources/atexit.c'),str(path/'test.c'),'-o',str(target)],check=True)
    subprocess.run([str(target)],check=True)
print('exit: success, EINVFN/EINVAL fallback, unexpected returns, LIFO and recursive callbacks: PASS (host models)')
