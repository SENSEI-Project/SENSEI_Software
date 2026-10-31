# Dependencies you need to obtain

SENSEI deliberately ships **no third-party code or binaries**. Everything in
`src/` is ours. The two external dependencies below you download yourself,
once, before building. This page tells you exactly what, from where, and how
to check you got the right thing.

---

## 1. EPANET 2.2 — headers and link library

SENSEI calls the EPANET toolkit for every hydraulic solve. It includes one
header directly:

```c
#include "epanet2_2.h"      // which in turn includes epanet2_enums.h
```

### What to download

From the Open Water Analytics EPANET repository:

> <https://github.com/OpenWaterAnalytics/EPANET>

Take the two public API headers from the `include/` directory of the `v2.2`
tag (see the next section for why that tag):

| File | Put it in |
|---|---|
| `epanet2_2.h` | `src/include/` |
| `epanet2_enums.h` | `src/include/` |

You also need a compiled library to link against — on Windows an import
library `epanet2.lib` plus the matching `epanet2.dll`, on Linux or macOS the
shared object. Either build EPANET from that repository following its own
instructions, or take a prebuilt release from its Releases page.

| File | Put it in |
|---|---|
| `epanet2.lib` (Windows import library) | `src/libs/` |
| `epanet2.dll` (Windows runtime) | `src/libs/`, and beside the executables |

You will have to create `src/libs/` — it does not exist in this repository and
is listed in `.gitignore`.

### Which version to take, and why it matters

**Take the headers from the `v2.2` tag.** This has been verified, not assumed:
SENSEI calls 78 distinct symbols of the EPANET API (functions and enumeration
constants), and **all 78 are present in upstream v2.2**. Nothing SENSEI uses
was added after it.

```
https://github.com/OpenWaterAnalytics/EPANET/tree/v2.2/include
```

A later 2.2.x or 2.3 header will also work — the API only grows — but v2.2 is
the version this was checked against.

### A note on the copy SENSEI was developed with

The header in the original development tree declares itself as version 2.2 in
its comment block, but it is **not** the v2.2 release header: it carries 14
symbols that v2.2 does not have, among them `EN_setvertex`,
`EN_timetonextevent`, `EN_setreportcallback`, `EN_getcontrolenabled` and
`EN_openX`. Those arrived in later EPANET versions, so that copy is a 2.3-era
or development snapshot mislabelled in its header comment.

**SENSEI uses none of those 14.** The discrepancy therefore has no effect on
building or on results, and v2.2 headers are sufficient. It is recorded here
only so that nobody is surprised by a checksum mismatch.

For reference, the checksums of the copies used during development were:

```
127966b91af90c4580bd694972baa5419ae14ba77fe4b6dc652a08687f4b2cd7  epanet2_2.h      (76,644 bytes)
5555215a50e0cf0cc34380f85c5e4adbd0c3bd26f5725790ff2020051422b883  epanet2_enums.h  (22,202 bytes)
```

On Windows you can check a file with:

```cmd
certutil -hashfile src\include\epanet2_2.h SHA256
```

**Do not expect these to match the v2.2 release headers** — per the paragraph
above, they will not, and that is fine. If the build fails with unknown
symbols or wrong argument counts, that is the real signal to investigate.

### The one thing you should know before you start

The `epanet2.dll` that produced the published results is **not** a stock
OWA-EPANET build. It is a private fork maintained during the project
(418,304 bytes), whose sources we are not able to publish. A stock EPANET 2.2
build produces a different library (474,624 bytes) and **has not been tested
against SENSEI**.

What we can say is that the fork does not extend the API surface SENSEI
relies on: every one of the 78 EPANET symbols SENSEI calls exists in upstream
v2.2. Whether the fork changes any of their *behaviour* is not something we
can verify without its sources, and that is precisely the gap.

That fork's DLL ships inside the executable archives at the root of this
repository (`Calibration.zip`, `Multiperiod_SE.zip`, `Snapshot_SE.zip`), where
it has always been distributed. If exact reproduction of the published tables
matters to you, take it from there rather than building EPANET yourself. If
you do build upstream EPANET, verify against the small example networks in
those archives before trusting results on your own models.

This is the weakest link in the reproducibility chain. See `../PROVENANCE.md`.

---

## 2. SuiteSparse — CHOLMOD and its ordering libraries

SENSEI factorises the normal equations with CHOLMOD:

```c
#include <suitesparse/cholmod.h>
```

and links against `cholmod`, `amd`, `camd`, `colamd`, `ccolamd` and
`suitesparseconfig`. CHOLMOD pulls in a BLAS; the reference toolchain uses
OpenBLAS.

### What to install

The sane route is your platform's package, not a source build.

**Windows (MSYS2, the reference platform)** — from the *MSYS2 MinGW 64-bit*
shell:

```bash
pacman -S --needed mingw-w64-x86_64-suitesparse
```

That single package supplies all six libraries and pulls OpenBLAS with it.
Headers land in `/mingw64/include/suitesparse/`, libraries in
`/mingw64/lib/`, DLLs in `/mingw64/bin/`. No `-I` or `-L` flag is needed for
them: the compiler finds them on its default paths.

**Debian / Ubuntu**

```bash
sudo apt install libsuitesparse-dev libopenblas-dev
```

**Fedora**

```bash
sudo dnf install suitesparse-devel openblas-devel
```

**macOS**

```bash
brew install suitesparse openblas
```

**From source**, if you must:
<https://github.com/DrTimothyAldenDavis/SuiteSparse>

### Licensing — worth a look before you redistribute

SuiteSparse is not uniformly licensed. Inside CHOLMOD, the Core and Cholesky
modules are LGPL-2.1-or-later while Supernodal, MatrixOps and Modify are
GPL-2.0-or-later. Which of them a binary pulls in depends on how CHOLMOD was
built and which factorisation path runs.

If you only build SENSEI for your own use this does not concern you. If you
redistribute a compiled binary, satisfy yourself about the obligations that
attach to the SuiteSparse build you linked against. See
`../THIRD_PARTY_NOTES.md`.

---

## 3. Compiler and toolchain

GCC 14.2.0 or later, C17. On Windows, MSYS2 MinGW-w64 x86_64:

```bash
pacman -S --needed mingw-w64-x86_64-gcc make
```

---

## Summary: the tree once you are ready to build

```
src/
├── core/            (in this repository)
├── include/         functions_SENSEI.h, messages_SENSEI.h   (in this repository)
│                    epanet2_2.h, epanet2_enums.h            <-- YOU ADD THESE
├── libs/                                                    <-- YOU CREATE THIS
│                    epanet2.lib, epanet2.dll                <-- YOU ADD THESE
├── calibration/     (in this repository)
├── multiperiod_se/  (in this repository)
├── snapshot_se/     (in this repository)
├── build/           (in this repository)
└── tools/           (in this repository)
```

SuiteSparse needs nothing copied into the tree: the compiler finds it on its
default include and library paths once the package is installed.

Then go to `build/BUILD.md`.
