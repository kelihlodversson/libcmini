"""Check libcmini's production poll adapters with a modeled GEMDOS trap."""
import os
from pathlib import Path
import shlex
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
with tempfile.TemporaryDirectory() as directory:
    build = Path(directory)
    (build / "mint").mkdir()
    (build / "mint/ostruct.h").touch()
    for arch in ("arm", "x86_64", "m68k"):
        headers = build / "mint/arch" / arch
        headers.mkdir(parents=True)
        (headers / "osbind.h").touch()
        (headers / "mintbind.h").write_text(
            "long trap_1_wllll(short, long, long, long, long);\n"
        )
        program = build / "test"
        subprocess.run(
            [*shlex.split(os.getenv("HOSTCC", "cc")), "-Wall", "-Wextra", "-Werror",
             "-I", directory, "-idirafter", root / "include", f"-D__{arch}__",
             root / "sources/fpoll_time64.c", root / "tests/fpoll_time64/test.c",
             "-o", program],
            check=True,
        )
        subprocess.run([program], check=True)
        print(f"{arch} Fpoll_time64 binding and applicable legacy adapters: PASS")
