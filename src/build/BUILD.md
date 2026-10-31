# Building SENSEI from source

SENSEI is a set of seven command-line programs that share two translation
units. Each program is one `.c` file compiled together with
`core/functions_SENSEI.c` and `core/messages_SENSEI.c`. There is no build
system beyond a compiler invocation, and none is needed.

These instructions reproduce the toolchain the distributed executables were
built with, recovered from the debug information embedded in those binaries:
**GCC 14.2.0, MSYS2 MinGW-w64, x86_64, `-g`, no optimisation, C17**.

**Before anything else, read [`../DEPENDENCIES.md`](../DEPENDENCIES.md).**
This repository ships no third-party code or binaries; you obtain EPANET and
SuiteSparse yourself, and that page says exactly what to download and from
where.

Read `../../PROVENANCE.md` too if you intend to reproduce published numbers.
It records what is known about which binary produced which result, and what
has not yet been verified.

---

## 1. Toolchain

### Windows (the reference platform)

Install [MSYS2](https://www.msys2.org/), then from the **MSYS2 MinGW 64-bit**
shell:

```bash
pacman -Syu
pacman -S --needed \
    mingw-w64-x86_64-gcc \
    mingw-w64-x86_64-suitesparse \
    make
```

`mingw-w64-x86_64-suitesparse` supplies CHOLMOD and its ordering libraries
(AMD, CAMD, COLAMD, CCOLAMD, SuiteSparse_config) and pulls in OpenBLAS.

Check the compiler:

```bash
gcc --version      # expect 14.2.0 or later
```

### Linux and macOS

The code is portable C17. The interactive pauses now use `getchar()` rather
than the Windows shell builtin (§6), so they compile anywhere. What is **not**
portable is the EPANET library: see §5 before you start.

Build EPANET 2.2 from
[OWA-EPANET](https://github.com/OpenWaterAnalytics/EPANET), install
SuiteSparse from your package manager, and point `-L`/`-l` at the result.

---

## 2. Layout the build expects

```
src/
├── core/        functions_SENSEI.c, messages_SENSEI.c
├── include/     functions_SENSEI.h, messages_SENSEI.h        (shipped)
│                epanet2_2.h, epanet2_enums.h                 <-- you add these
├── libs/        epanet2.lib, epanet2.dll                     <-- you create and fill this
├── calibration/     Calibration_SENSEI.c
│                    GenerarMedidas_Calibration_SENSEI.c
├── multiperiod_se/  Multiperiod_SE_SENSEI.c
│                    Emitter_Optimization_Multiperiod_SE.c
│                    GenerarMedidas_Multiperiod_SENSEI.c
├── snapshot_se/     Snapshot_SE_SENSEI.c
│                    GenerarMedidas_Snapshot_SENSEI.c
├── build/       sensei_build.mk, BUILD.md, tasks.json
├── tools/       make_pauses_optional.py
└── DEPENDENCIES.md
```

The two EPANET headers and the `libs/` directory are not in this repository
by design: they are third-party artefacts and we do not redistribute them.
`DEPENDENCIES.md` tells you where to get them, where to put them, and how to
check you got the right ones. SuiteSparse needs nothing copied into the tree.

## 3. Build

From `src/`:

```bash
make -f build/sensei_build.mk          # all seven programs into bin/
make -f build/sensei_build.mk dist     # runnable folders with the DLLs
make -f build/sensei_build.mk clean
```

(The file is named `sensei_build.mk` rather than `Makefile` only so that it
travels cleanly through tooling that treats `Makefile` as special. Rename it
if you prefer plain `make`.)

Or compile one program by hand. This is the exact recipe, and the only one:

```bash
gcc -g -std=c17 \
    multiperiod_se/Multiperiod_SE_SENSEI.c \
    core/functions_SENSEI.c \
    core/messages_SENSEI.c \
    -o bin/Multiperiod_SE_SENSEI.exe \
    -I include -L libs \
    -lsuitesparseconfig -lcholmod -lcolamd -lamd -lccolamd -lcamd \
    -lepanet2 -lm -static-libgcc
```

Substitute the program name for the other six. Nothing else changes: the
include path, the library path and the link line are identical for all of
them.

Two notes on the flags. There is **no optimisation flag**: the distributed
binaries were built with `-g` alone, and that is what these instructions
reproduce. Adding `-O2` is safe and roughly halves the runtime, but it changes
floating-point scheduling, so results can differ in the last significant
figures; build without it if you are reproducing the published tables. The
original build also passed `-static-libstdc++`, which does nothing for a C
program; it is dropped here.

## 4. Runtime DLLs (Windows)

The executables need these alongside them, or on `PATH`. All but
`epanet2.dll` come from `C:\msys64\mingw64\bin`:

```
epanet2.dll               (see §5)
libcholmod.dll            libamd.dll        libcamd.dll
libcolamd.dll             libccolamd.dll    libsuitesparseconfig.dll
libopenblas.dll           libgfortran-5.dll libquadmath-0.dll
libgomp-1.dll             libwinpthread-1.dll
```

`make -f build/sensei_build.mk dist` assembles a runnable folder per program
with the DLLs copied in.

## 5. The EPANET dependency — read this

SENSEI links against an `epanet2.dll` of 418,304 bytes, which is **not** a
stock build of OWA-EPANET. It comes from a private fork maintained during the
project, and **we are not able to publish its sources**. A stock EPANET 2.2
build produces a different library (474,624 bytes) and has not been tested
against SENSEI.

The fork's DLL ships inside the executable ZIP archives at the root of this
repository, where it has always been distributed. It is not committed as a
loose file here: this repository contains no third-party binaries.

If exact reproduction of the published tables matters to you, take the DLL
from those archives. If you build EPANET from upstream instead, verify
against the small example networks shipped in the archives before trusting
results on your own models — the API surface SENSEI uses is standard, but the
fork has not been diffed against upstream line by line.

Full detail, including checksums for the headers:
[`../DEPENDENCIES.md`](../DEPENDENCIES.md). Licensing:
[`../../THIRD_PARTY_NOTES.md`](../../THIRD_PARTY_NOTES.md).

## 6. Interactive pauses

The programs were written to pause at each stage so that a user could follow
the computation step by step while learning the method. That behaviour is
preserved but is **opt-in**:

```bash
SENSEI_INTERACTIVE=1 ./Multiperiod_SE_SENSEI.exe     # pause at each stage
./Multiperiod_SE_SENSEI.exe                           # run to completion
```

On Windows `cmd`: `set SENSEI_INTERACTIVE=1` before running.

Unset, or set to anything other than `1`/`y`/`t`, the programs run unattended,
which is what batch reproduction of the published results requires.

This is the one change made to the sources relative to the December 2025
development tree from which the executables were built: 200 calls to
`system("pause")` became `SENSEI_PAUSE()`, and the supporting definition was
added to `core/functions_SENSEI.c` and `include/functions_SENSEI.h`. The
transformation is scripted and reversible — see `tools/make_pauses_optional.py`
— and it is recorded in `../../PROVENANCE.md`. With `SENSEI_INTERACTIVE=1`
the behaviour is identical to the original.

## 7. Verifying the build

Extract `Snapshot_SE.zip` from the root of this repository and run the
smallest example in it: one tank, two pipes, two demand nodes. Replace the
shipped executable with the one you just built, keeping the DLLs and the
`config_snapshot_SE.txt` in place, and compare the `*_RESULTS.csv` it writes
against the copy that came in the archive.

If it matches, the toolchain, the linear algebra and the EPANET binding are
all working.

Then run the C-Town cases in `Multiperiod_SE.zip` and `Calibration.zip` the
same way. `PROVENANCE.md` lists the four scenarios used in the paper and what
each one should produce.
