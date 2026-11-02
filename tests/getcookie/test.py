#!/usr/bin/env python3
"""Exercise the x32 Getcookie path with a mocked Ssystem, using the host CC."""
from pathlib import Path
import os
import shlex
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
with tempfile.TemporaryDirectory() as directory:
    work = Path(directory)
    headers = work / "mint"
    headers.mkdir()
    (headers / "osbind.h").write_text("")
    (headers / "cookie.h").write_bytes((root / "include/mint/cookie.h").read_bytes())
    (headers / "mintbind.h").write_text(
        "#define S_GETCOOKIE 8\nlong Ssystem(short, long, long *);\n")
    harness = work / "test.c"
    harness.write_text(r'''
#include <assert.h>
#include <stddef.h>
#include <mint/cookie.h>
static long status, stored;
static int calls;
long Ssystem(short mode, long tag, long *out)
{
    assert(mode == 8 && tag == C_MiNT && out != NULL);
    calls++;
    *out = stored;
    return status;
}
int main(void)
{
    long value;
    stored = 0x1234;
    assert(Getcookie(C_MiNT, &value) == C_FOUND && value == stored);
    stored = 0;
    assert(Getcookie(C_MiNT, &value) == C_FOUND && value == 0);
    stored = -1;
    assert(Getcookie(C_MiNT, &value) == C_FOUND && value == -1);
    assert(Getcookie(C_MiNT, NULL) == C_FOUND);
    status = -1;
    value = 42;
    assert(Getcookie(C_MiNT, &value) == C_NOTFOUND && value == 0);
    status = -32;
    value = 42;
    assert(Getcookie(C_MiNT, &value) == C_NOTFOUND && value == 0);
    assert(Getcookie(C_MiNT, NULL) == C_NOTFOUND);
    assert(calls == 7);
    return 0;
}
''')
    executable = work / "test"
    subprocess.run(shlex.split(os.environ.get("HOSTCC", "cc")) + [
        "-Wall", "-Wextra", "-Werror", "-D__x86_64__", "-I" + str(work),
        str(root / "sources/getcook.c"), str(harness), "-o", str(executable),
    ], check=True)
    subprocess.run([str(executable)], check=True)
print("Getcookie: 7 cases passed")
