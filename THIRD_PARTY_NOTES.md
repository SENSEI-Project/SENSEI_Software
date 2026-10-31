# Third-Party Notices

SENSEI Software is released under the MIT License (see `LICENSE`). The
components listed below are **not** covered by that licence. Each is governed
by its own terms, and users are responsible for complying with them.

This file supersedes an earlier, generic version that named no component.

---

## EPANET

SENSEI calls the EPANET 2.2 toolkit through `epanet2.dll` and compiles against
the public EPANET headers (`epanet2_2.h`, `epanet2_enums.h`, `types.h` in
`src/include/`).

EPANET is developed by the United States Environmental Protection Agency and
maintained as open source by the Open Water Analytics community at
<https://github.com/OpenWaterAnalytics/EPANET>, under the MIT License.

**Important:** the `epanet2.dll` distributed inside the ZIP archives in this
repository (418,304 bytes) is **not** a stock OWA-EPANET build. It comes from
a private fork maintained during the project, whose sources we are not able to
publish. A stock EPANET 2.2 build produces a different binary (474,624 bytes)
and has not been tested against SENSEI. See `PROVENANCE.md` and
`src/build/BUILD.md` §5.

## SuiteSparse (CHOLMOD, AMD, CAMD, COLAMD, CCOLAMD, SuiteSparse_config)

SENSEI solves the normal equations with CHOLMOD and its ordering libraries,
from SuiteSparse by Timothy A. Davis and contributors
(<https://github.com/DrTimothyAldenDavis/SuiteSparse>).

**SuiteSparse is not uniformly licensed, and this matters for redistribution.**
Different CHOLMOD modules carry different terms: Core and Cholesky are
LGPL-2.1-or-later, while Supernodal, MatrixOps and Modify are GPL-2.0-or-later.
Which modules a given binary pulls in depends on how CHOLMOD was built and
which factorisation path is taken at run time.

Anyone redistributing a compiled SENSEI binary, including the ZIP archives in
this repository, should satisfy themselves about the obligations that attach
to the SuiteSparse build they are linking against. We flag this because it is
easy to miss, not because we have determined that a problem exists.

## OpenBLAS

CHOLMOD is built against OpenBLAS in the reference toolchain
(<https://github.com/OpenMathLib/OpenBLAS>), BSD 3-Clause.

## MinGW-w64 runtime

The Windows binaries link the MinGW-w64 runtime and `libgcc`, `libgfortran`,
`libquadmath`, `libgomp` and `libwinpthread`. These carry their own licences,
principally GPL with the GCC Runtime Library Exception, and the MinGW-w64
runtime licence.

---

## Reporting a problem with this notice

If you believe a component is misattributed, missing, or that an obligation
above is stated incorrectly, please open an issue. We would rather correct it
than leave it wrong.
