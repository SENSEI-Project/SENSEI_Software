# Provenance and reproducibility

*What is known about which code produced which result, and what has not been
verified. Last updated 31 October 2026.*

This file exists because the honest answer to "can I reproduce the published
numbers from this repository?" is "probably, and here is exactly what stands
between you and certainty." Everything below is reconstructed from the debug
information embedded in the distributed binaries, from file timestamps, and
from the build task that produced them.

---

## The build chain

The debug information in the distributed executables names the compilation
units, the flags and the machine:

```
GNU C17 14.2.0 -mtune=generic -march=nocona -g
C:\Users\user\Desktop\SENSEI/functions_SENSEI.c
C:\Users\user\Desktop\SENSEI/messages_SENSEI.c
C:\Users\user\Desktop\SENSEI\Multiperiod_SE\Multiperiod_SE_SENSEI.c
```

- **GCC 14.2.0**, MSYS2 MinGW-w64, x86_64.
- **C17, `-g`, no optimisation.** (The other toolchain string in the
  binaries, `GNU C99 ... -O2 -std=gnu99`, belongs to the MinGW runtime
  objects, not to SENSEI code.)
- Three translation units per program: the program's `.c`,
  `functions_SENSEI.c` and `messages_SENSEI.c`.
- Built in `C:\Users\user\Desktop\SENSEI`, a developer machine, not the
  corresponding author's.

Linking: `-I include -L libs`, against `suitesparseconfig, cholmod, colamd,
amd, ccolamd, camd, epanet2`, plus `-lm -static-libgcc`. The full recipe is
in `src/build/BUILD.md`.

## Chronology

| Date | Event |
|---|---|
| **3–5 Nov 2025** | The results reported in the paper are generated |
| **3 Dec 2025** | The distributed executables are compiled |
| 5–11 Dec 2025 | Headers and the four programs are edited |
| **15 Dec 2025, 10:37** | The four development-tree executables are rebuilt in one batch |
| **17 Dec 2025, 09:29** | `functions_SENSEI.c` is edited again |
| 17 Dec 2025, 09:54 | Only `GenerarMedidas_Multiperiod` is rebuilt |
| 31 Oct 2026 | Sources published in this repository |

Two facts follow, and both matter.

**The `functions_SENSEI.c` published here is two days newer than the
development-tree executables.** Those binaries do not contain the last edit to
the shared file. The drift is real, narrow, and dated.

**The published results predate both compilations.** They were produced in
early November 2025. Neither the distributed executables (3 December) nor the
development build (15 December) is demonstrably the binary that generated the
tables in the paper.

## What this means for reproduction

Nobody has claimed otherwise, and no result is in question. But a reader
should know that the chain *source → binary → published table* has not been
closed end to end. The missing step is a single verification run:

1. Build from `src/` following `src/build/BUILD.md`, without optimisation.
2. Run the four C-Town scenarios shipped in `Multiperiod_SE.zip`: the
   baseline, the persistent +0.20 m bias at tank T3, the undetected shutdown
   of pump PU2, and the emitter leak at node J59.
3. Compare the hourly weighted-least-squares objective against the values
   published in the paper.

If they agree, this section can be replaced by a statement that they do. If
they do not, something changed in the code between November and December 2025
that affects published numbers, and that has to be understood before the
numbers are relied on.

**As of this release, step 3 has not been performed.** Treat the executables
in the ZIP archives as the reference artefacts, and this source tree as the
code they were built from, modulo the two deltas recorded here.

## Deltas between this source tree and the December 2025 binaries

**1. The interactive pauses are now opt-in.** The programs called
`system("pause")` at each stage so a user could follow the computation. 200
such calls were replaced by `SENSEI_PAUSE()`, which consults the
`SENSEI_INTERACTIVE` environment variable; the supporting definition was added
to `core/functions_SENSEI.c` and `include/functions_SENSEI.h`. With
`SENSEI_INTERACTIVE=1` the behaviour is identical to the original. Unset, the
programs run unattended, which is what batch reproduction requires — and
without which no reviewer could reproduce anything, since every linear solve
would wait for a keypress.

The change is scripted and reversible: `src/tools/make_pauses_optional.py`.
39 already-commented-out occurrences were left untouched. The transformed
sources pass `gcc -fsyntax-only -std=c17` with no new diagnostics.

**2. `functions_SENSEI.c` is the 17 December 2025 revision**, as noted above.

**3. The EPANET headers were removed from `src/include/`** when the sources
were published, together with an unused copy of EPANET's internal `types.h`
that nothing in SENSEI includes. They are third-party files and we do not
redistribute them. This changes nothing about the code: the two headers are
restored by the user from upstream before building.

Nothing else differs. No algorithm, constant, tolerance or output format was
touched.

## The EPANET dependency

SENSEI links against an `epanet2.dll` of 418,304 bytes from a private fork
maintained during the project, not against stock OWA-EPANET (474,624 bytes).
**The fork's sources are not available and we cannot publish them.**

The DLL continues to ship inside the executable ZIP archives, where it always
has. It is not committed as a loose file: **this repository contains no
third-party code or binaries in its source tree**, by choice. Neither the
EPANET headers nor the library are here. Anyone rebuilding from source
supplies them, following `src/DEPENDENCIES.md`, which gives the download
location and checksums for the header versions SENSEI was developed against.

This is the weakest link in the chain and the one a reviewer will hit first.
We would rather say so here than have it discovered.

## Known open points in the implementation

Recorded for transparency; none of them invalidates a published number, and
each is being looked at.

- **Emitter optimisation.** The leak-diagnosis routine does not free the
  emitter coefficient as an unknown of the estimation problem. It is a single
  scalar, constant over the leak window, fitted by finite-difference gradient
  descent on the accumulated objective in five fixed-length steps with no
  convergence test. The paper describes it this way; earlier drafts did not.
- **Posterior covariance with null weights.** When a measurement weight is
  zero, the corresponding variance is infinite and the guards currently set
  the posterior CV to zero, which is the most optimistic value rather than the
  most conservative. Avoid zero weights.
- **Jacobian sparsification threshold.** Entries are dropped on an absolute
  test, `fabs(derivative) > 0.0001`. On a badly scaled problem this can leave
  the Jacobian artificially rank-deficient. A relative test would be safer.
- **The analytic Jacobian and `b·K`.** The derivative of the head-loss law
  carries the exponent, `∂(k|Q|^(b-1)Q)/∂Q = b·k|Q|^(b-1)`. The paper uses
  `b·K` consistently. Verify that the C assembly does the same before relying
  on the analytic path; the numerical Jacobian, which is what the programs use
  by default, is unaffected.
