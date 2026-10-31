# src — the SENSEI C implementation

The canonical C sources behind the executables distributed at the root of this
repository. **Everything here is ours**: this tree contains no third-party
code and no binaries.

```
core/            functions_SENSEI.c  (305 KB, the numerical core)
                 messages_SENSEI.c   (user-facing messages)
include/         functions_SENSEI.h, messages_SENSEI.h
calibration/     Calibration_SENSEI.c, GenerarMedidas_Calibration_SENSEI.c
multiperiod_se/  Multiperiod_SE_SENSEI.c, Emitter_Optimization_Multiperiod_SE.c,
                 GenerarMedidas_Multiperiod_SENSEI.c
snapshot_se/     Snapshot_SE_SENSEI.c, GenerarMedidas_Snapshot_SENSEI.c
build/           BUILD.md, sensei_build.mk, tasks.json
tools/           make_pauses_optional.py
DEPENDENCIES.md  what to download before you build, and from where
```

Each `*_SENSEI.c` is a standalone `main()`, compiled together with the two
files in `core/`. The `GenerarMedidas_*` programs synthesize measurement sets
from a network, which is how the example scenarios were produced.

## Before you build

1. **[`DEPENDENCIES.md`](DEPENDENCIES.md)** — EPANET headers and library, and
   SuiteSparse. You download these yourself; they are not here.
2. **[`build/BUILD.md`](build/BUILD.md)** — the exact toolchain and link line.
3. **[`../PROVENANCE.md`](../PROVENANCE.md)** — read this if you intend to
   reproduce published numbers rather than just run the tools.

## One behavioural note

The programs pause at each stage so you can follow the computation. This is
opt-in: set `SENSEI_INTERACTIVE=1` to enable it, leave it unset to run
unattended. Batch reproduction needs it unset. See `BUILD.md` §6.
