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
    (build / "mint/osbind.h").write_text(
        "long test_trap(short, short, long, long, long);\n"
        "long Pexecve(short, const char *, char *const [], char *const []);\n"
        "#define trap_1_wwlll test_trap\n"
        "#define Pexec(m,p,t,e) test_trap(0x4b, (m), (long)(p), (long)(t), (long)(e))\n"
    )
    for arch in ("__m68k__", "__arm__", "__x86_64__"):
        program = build / "test"
        subprocess.run(
            [*shlex.split(os.getenv("HOSTCC", "cc")), "-Wall", "-Wextra", "-Werror",
             "-I", directory, f"-D{arch}", "-Dexecve=libcmini_execve",
             ROOT / "sources/pexecve.c", ROOT / "tests/pexecve/test.c", "-o", program],
            check=True,
        )
        subprocess.run([program], check=True)
        print(f"{arch} Pexecve transport, fallback and errno: PASS")
