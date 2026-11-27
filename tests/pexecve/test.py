"""Run the production binding with a modeled GEMDOS transport."""
import os
from pathlib import Path
import shlex
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]

with tempfile.TemporaryDirectory() as directory:
    build = Path(directory)
    (build / "mint").mkdir()
    (build / "mint/ostruct.h").touch()
    trap_header = (
        "long test_trap(short, short, long, long, long);\n"
        "#define trap_1_wwlll test_trap\n"
    )
    for arch in ("m68k", "arm", "x86_64"):
        header = build / "mint/arch" / arch / "osbind.h"
        header.parent.mkdir(parents=True)
        header.write_text(trap_header)
    for arch in ("__m68k__", "__arm__", "__x86_64__"):
        program = build / "test"
        subprocess.run(
            [*shlex.split(os.getenv("HOSTCC", "cc")), "-Wall", "-Wextra", "-Werror",
             "-I", directory, "-idirafter", ROOT / "include", f"-D{arch}",
             "-Dexecve=libcmini_execve", ROOT / "sources/pexecve.c",
             ROOT / "sources/pexec_compat.c", ROOT / "tests/pexecve/test.c", "-o", program],
            check=True,
        )
        subprocess.run([program], check=True)
        print(f"{arch} Pexecve transport and Pexec compatibility: PASS")
