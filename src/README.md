# src — the SENSEI C implementation

The canonical C sources behind the executables distributed at the root of
this repository.

```
core/            functions_SENSEI.c  (312 KB, the numerical core)
                 messages_SENSEI.c   (user-facing messages)
include/         SENSEI headers + the EPANET headers they compile against
calibration/     Calibration_SENSEI.c, GenerarMedidas_Calibration_SENSEI.c
multiperiod_se/  Multiperiod_SE_SENSEI.c, Emitter_Optimization_Multiperiod_SE.c,
                 GenerarMedidas_Multiperiod_SENSEI.c
snapshot_se/     Snapshot_SE_SENSEI.c, GenerarMedidas_Snapshot_SENSEI.c
build/           BUILD.md, sensei_build.mk, tasks.json
tools/           make_pauses_optional.py
```

Each `*_SENSEI.c` is a standalone `main()`, compiled together with the two
files in `core/`. The `GenerarMedidas_*` programs synthesize measurement sets
from a network, which is how the example scenarios were produced.

**Build instructions: `build/BUILD.md`.** Read `../PROVENANCE.md` first if you
intend to reproduce published results.

Two things are deliberately absent and both are explained in `build/BUILD.md`:
there is no `libs/` directory (§5, the EPANET fork), and the interactive
pauses are opt-in rather than unconditional (§6).
