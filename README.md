[![Build Status](https://github.com/freemint/libcmini/actions/workflows/build.yml/badge.svg?branch=master)](https://github.com/freemint/libcmini/actions) 

* Latest snapshot: [Download (mint)](https://atari.joska.no/snapshots/libcmini/libcmini-mint-latest.tar.bz2)
* Latest snapshot: [Download (mintelf)](https://atari.joska.no/snapshots/libcmini/libcmini-mintelf-latest.tar.bz2)
* [Archive](https://atari.joska.no/snapshots/libcmini/)

# libcmini

libcmini aims to be a small-footprint C library for the m68k-atari-mint (cross) toolchain, similar to the C library
Pure-C came with. Many GEM programs do not need full MiNT support and only a handful of C library functions.

By default, gcc compiled programs on the Atari ST platform link to mintlib. Mintlib aims to implement all POSIX
functionality to make porting Unix/Linux programs easier (which is a very good thing to have). For small GEM-based
programs, this comes with a drawback, however: program size gets huge due to the underlying UNIX compatibility layer.

For programs that don't need this, libcmini tries to provide an alternative. It's far from finished, but already quite 
usable.

## Example
This small program

```
#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("Hello World\r\n");
}
```

compiled with the standard m68k-atari-mint toolchain 

```
m68k-atari-mint-gcc -o hello.tos -s hello.c 
```

results in a binary size of 118220 bytes. Huge. The same thing compiled with libcmini:

```
m68k-atari-mint-gcc -nostdlib $LIBCMINI/crt0.o hello.c -o hello.tos -s -L$LIBCMINI -lcmini -lgcc
```
(note that - since we compile with -nostdlib - you have to add the gcc runtime support library libgcc.a to your command line for the processors that need it.
Sometimes you will have to specify ` -lgcc -lcmini -lgcc` to avoid 'undefined reference' error messages.)

creates a binary with 11794 bytes. About a tenth of the size.

Remember that gcc does not automatically look up multi-lib versions of
libraries in directories specified by -L. If you want to do the same as
above for a different architecture, you have to explicitly add the
correct sub-directory:

```
m68k-atari-mint-gcc -nostdlib -mcpu=5475 $LIBCMINI/crt0.o hello.c -o hello.tos -s -L$LIBCMINI/m5475 -lcmini -lgcc
```

Same applies if you are using any other switch that requires a
different library version, like --mshort and --mfastcall.

## Binary Releases Downloads

binary releases (in tar.gz format) can be downloaded from here: https://github.com/freemint/libcmini/releases
source is also available there.

## What's that libcmini.creator stuff about?

I'm using the QtCreator IDE for my Atari cross development work (most of the time, if I'm not in vi mode). The `libcmini.config`, `libcmini.creator`, `libcmini.files` and `libcmini.includes` files are used by QtCreator to keep track of the project.
If you are not using QtCreator (I would recommend it since it adds a few nice features to Atari cross development), just ignore these files.

## Fastcall?
(contributed by Frederik Olsson)

Fastcall requires [patched GCC 4.6.4](https://github.com/PeyloW/gcc-4.6.4) with -mfastcall support.

GCC passes all arguments on the stack by default. The Atari ST, and even the Falcon 030, have a slow 16 bit wide memory bus.
As a result memory operations are very expensive. Most functions take only a few arguments that can be passed in registers.

-mfastcall uses d0-2/a0-1/fp0-2 for passing integer and pointer arguments when possible, and always return pointers in a0.
a2 is not used for passing arguments, but is considered clobbered, this allows a2 to be used as call target function pointer.

The result is faster code execution, and slightly smaller memory footprint. Especially when used in combination with -mshort to 
avoid library calls for most integer operations.

With -mfastcall libcmini itself is 3% smaller binary, and the tests/bench test executes 20% faster. 

## Compilation

The build is configured the way the Linux kernel is, using [Kconfig](https://en.wikipedia.org/wiki/Kconfig): you pick one of the ready-made configurations under `configs/`, optionally adjust it, and then build. This requires Python 3 with [kconfiglib](https://github.com/ulfalizer/Kconfiglib) (`pip3 install kconfiglib`) and the appropriate cross toolchain in your `PATH` — `m68k-atari-mint-`/`m68k-atari-mintelf-` for m68k, or `arm-none-eabi-` for [pTOS](https://github.com/kelihlodversson/pTOS) on ARM (see below).

```sh
ls configs/                  # list the ready-made configurations
make mintelf_defconfig       # load one (mint_defconfig, mintelf_defconfig, ptos_arm_defconfig)
make menuconfig               # optional: adjust it interactively
make                          # build the image
```

`make menuconfig` (or editing `configs/<name>_defconfig` and reloading it) exposes:

  * **Target architecture** — m68k (Atari/MiNT) or ARM (pTOS). See "ARM / pTOS" below.
  * On m68k: `BUILD_CF`, `BUILD_FAST`, `BUILD_SOFT_FLOAT`, `BUILD_SHORT` — which multilib variants to build alongside the plain 68000 one (ColdFire m5475, `-mfastcall`, software floating point, and 16-bit `int` respectively); `ONLY_68K` to skip all of them for faster iteration; `COMPILE_ELF` to use the `m68k-atari-mintelf-` toolchain and produce ELF binaries instead of the default `m68k-atari-mint-` a.out ones.
  * `STDIO_WITH_LONG_LONG` — add `long long` support to the `printf`/`scanf` family (currently known to fail).
  * `STDIO_MAP_NEWLINE` — translate `\n` to the Atari/DOS `\r\n` line ending on stdio output (and back on input). This is standard practice on TOS but not on MiNT; if you turn it off, put `\r\n` in your own print statements on TOS to avoid odd-looking console output.

The generated `.config`, `obj/auto.conf` and `obj/autoconf.h` are build products — never edit them by hand, and don't commit them.

### Legacy flat variables

Without a `.config` at all, `make` falls back to the old defaults (m68k a.out, every multilib variant) and the flat `VAR=Y/N` variables this project used before the Kconfig migration still work, unchanged, as `make` arguments — the same names as the Kconfig options above: `COMPILE_ELF`, `ONLY_68K`, `BUILD_CF`, `BUILD_FAST`, `BUILD_SOFT_FLOAT`, `BUILD_SHORT`, `STDIO_WITH_LONG_LONG`, `STDIO_MAP_NEWLINE`. Instead of `Y`, `yes` also works. For example:

```sh
VERBOSE=yes make ONLY_68K=yes STDIO_MAP_NEWLINE=N
```

`VERBOSE=Y` (also settable as an environment variable) prints every command the build runs, either way — with or without a `.config`.

### ARM / pTOS

`make ptos_arm_defconfig && make` builds `libcmini.a`/`libiiomini.a`/`crt0.o`/`minicrt0.o` for [pTOS](https://github.com/kelihlodversson/pTOS) on ARM, using the `arm-none-eabi-` cross toolchain (Ubuntu/Debian package it as `gcc-arm-none-eabi`). Unlike the m68k targets there's no multilib fan-out, so the outputs land directly under `build/` rather than in a per-variant subdirectory.

The ARM port is functionally complete — GEMDOS/BIOS/XBIOS bindings, `crt0`, `setjmp`/`longjmp`, etc. — but two things are still open, tracked in [issue #1](https://github.com/kelihlodversson/libcmini/issues/1):

  * A handful of rare BIOS/XBIOS calls needing more than 4 arguments (`Rwabs`/`Lrwabs`, `Floprd`/`Flopwr`/`Flopfmt`/`Flopver`, `Rsconf`) aren't available yet, pending a pTOS kernel-side ABI extension. Calling one is a compile error, not a silently wrong call.
  * The port hasn't been tested end to end against a running pTOS yet — only built with the real toolchain and, where practical, exercised in isolation under `qemu-arm`.

## Installation

The basic installation command is `make install`. The default install
location is `/usr`, i.e. `/usr/include` and `/usr/lib`. `make install`
should basically get the same options as used for the compilation if
some architectures were omitted, otherwise it will regenerate the
missing ones.

The installation destination can be specified with either the `PREFIX*` options or the `DESTDIR` option:
  * `PREFIX=<dir>` - Install in `$PREFIX/include` and `$PREFIX/lib`, unless one or more of the following options are given:
  * `PREFIX_FOR_INCLUDE=<dir>` - Directory for the include files. When multiple architectures are present,
only one copy of the include files are installed. Default is `$PREFIX/include`.
  * `PREFIX_FOR_LIB=<dir>` - Directory for the library files. Default is `$PREFIX/lib`.
  * `PREFIX_FOR_STARTUP=<dir>` - Directory for the startup file (`crt0.o`). Default is `$PREFIX/lib`.
  * `DESTDIR=<dir` - Installs the include files in `$(DESTDIR)/usr/include` and the library and startup file in
    `$(DESTDIR)/usr/lib`.

The `DESTDIR` and `PREFIX*` options cannot be used together. They can also be given as environment variables.

If the library has been built for multiple architectures, there will be
a subdirectory for each one (except the basic 68000) in the library
directory. For the `elf` version, this applies also to the startup file.
ARM builds (no multilib fan-out) install straight into `PREFIX_FOR_LIB`/`PREFIX_FOR_STARTUP` with no subdirectory, the same as the basic 68000 case.

This all works the same whether the build was configured via `make <name>_defconfig`/`menuconfig` or the legacy flat variables.

Example `make install` command corresponding to the above compile command:

```
VERBOSE=yes make ONLY_68K=yes PREFIX=/opt/libcmini install
```
The `STDIO_MAP_NEWLINE` option is only relevant at compile time, so this was left out.

## Contribution

Contributions are always welcome.

Please make sure you provide all new code using 'four spaces' indentation without TAB characters and trailing spaces removed.
