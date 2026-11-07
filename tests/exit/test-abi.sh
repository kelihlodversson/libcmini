#!/bin/sh
# Existing compilers only; compilation/disassembly is not native execution.
set -eu
cd "$(dirname "$0")/../.."
work=$(mktemp -d "${TMPDIR:-/tmp}/libcmini-exit-abi.XXXXXX")
trap 'rm -rf "$work"' EXIT HUP INT TERM
if command -v m68k-atari-mintelf-gcc >/dev/null 2>&1; then
    for flags in '' '-mshort' '-mfastcall' '-mshort -mfastcall'; do
        if ! m68k-atari-mintelf-gcc $flags -E - </dev/null >/dev/null 2>&1; then
            echo "m68k $flags: unavailable compiler mode (not executed)"
            continue
        fi
        m68k-atari-mintelf-gcc -m68020 $flags -Os -Wall -Wextra -Werror \
            -Iinclude -c sources/exit.c -o "$work/m68k.o"
        m68k-atari-mintelf-objdump -dr "$work/m68k.o" > "$work/m68k.dis"
        echo "m68k $flags exit/bindings: PASS (compile only)"
    done
else
    echo 'm68k compiler unavailable (not executed)'
fi
if command -v arm-none-eabi-gcc >/dev/null 2>&1; then
    arm-none-eabi-gcc -marm -march=armv7-a -mfloat-abi=hard -mfpu=vfp -Os \
        -Wall -Wextra -Werror -Iinclude -c sources/exit.c -o "$work/arm.o"
    echo 'ARM exit/bindings: PASS (compile only)'
else
    echo 'ARM compiler unavailable (not executed)'
fi
${HOSTCC:-cc} -mx32 -Os -Wall -Wextra -Werror -Iinclude -c sources/exit.c -o "$work/x32.o"
echo 'x32 exit/bindings: PASS (compile only)'
