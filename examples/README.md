# examples — the datasets behind the published results

Every case study reported in the SENSEI paper and its Supplemental Materials,
with its EPANET model, its measurement files, its configuration, and the
output SENSEI produced. Nothing here is third-party: all of it was generated
for this work.

2.2 MB, 111 files. If you only want to see the software run, start with
`snapshot_se/Examples_Red1/Example1` — one tank, two pipes, two demand nodes,
and it finishes instantly.

---

## How a scenario is run

Each tool reads a configuration file **whose name must not change**, placed
next to the executable:

| Tool | Configuration file |
|---|---|
| `Snapshot_SE_SENSEI` | `config_snapshot_SE.txt` |
| `Multiperiod_SE_SENSEI` | `config_multiperiod_SE.txt` |
| `Calibration_SENSEI` | `config_calibration.txt` |

The file is a list of `key; value` lines. The ones that select a scenario:

```
PATH;        SE_Examples_our_C-town      <- folder holding the case
info_File;   our_C-town.inp              <- the EPANET model
dm_File;     NORMAL/dm_our_C-town.csv    <- demand measurements
wlm_File;    NORMAL/wlm_our_C-town.csv   <- tank water levels
pm_File;     NORMAL/pm_our_C-town.csv    <- pressures
fm_File;     NORMAL/fm_our_C-town.csv    <- instantaneous flows
fm_avg_File; -                           <- window-averaged flows
```

A `-` means "not used" or "take the default". The numerical settings below
the separator (`epsilon`, `max_it`, `tol`, `lambda_init`, `max_attempts`,
`emitter_ID`, …) behave the same way: leave them at `-` to reproduce the
published runs.

**To switch scenario, change the four `*_File` lines.** That is the whole
mechanism: the folders below differ only in their measurement files, not in
the model.

### Measurement file format

All four kinds share one layout, semicolon-separated, no header row:

```
ID ; CV ; value(t1) ; value(t2) ; … ; value(tN)
```

- **ID** — the EPANET element identifier (`J411`, `P310`, `T1`, …).
- **CV** — coefficient of variation of that sensor, which sets its weight.
  `0.05` for demand pseudo-measurements, `0.01` for tank levels and
  pressures, `0.02` for DMA inflows, in the published runs.
- **value(tᵢ)** — one reading per period. A value of `0.000000` marks a
  period with no reading, not a measured zero.

Tank-level files (`wlm_`) carry one more column than the others: levels are
recorded at both ends of every window, so a 12-period run has 13 levels.
That is the storage coupling the method relies on.

### Outputs

Each run writes three files next to the inputs:

- `*_estimated_values.csv` — the estimated state
- `*_normalized_residuals.csv` — studentized residuals, the diagnostic signal
- `*_posterior_CV.csv` — posterior coefficients of variation

The copies committed here are the ones that produced the published numbers.
Compare against them.

---

## Which scenario produces which result

### Main manuscript

| Folder | Produces |
|---|---|
| `calibration/C-town_Calib_Example` | Table 1 (valve, DMA roughness, tank diameters), Figs. 2–3, Table S6 |
| `multiperiod_se/…/NORMAL` | Table 2 col. "Normal", Fig. 4a |
| `multiperiod_se/…/BIAS_T3` | Table 2 col. "T3 bias", Fig. 4b, Table S7 |
| `multiperiod_se/…/BIAS_T3_CORREGIDO` | Table 2 col. "T3 corrected" |
| `multiperiod_se/…/PU2_CLOSED` | Table 2 col. "PU2 shutdown", Table S8 |
| `multiperiod_se/…/LEAK_J59_emitter1` | Table 2 col. "J59 leak" |
| `multiperiod_se/…/LEAK_J59_emitter1_propJ*` | Table 3, the six tested candidates |

The six `propJ*` folders hold **outputs only**. Their inputs are byte-identical
to `LEAK_J59_emitter1`: the scenario is the same leak, re-estimated with an
emitter freed at a different candidate node. Point the config at
`LEAK_J59_emitter1` and set `emitter_ID` to the node you want to test.

`our_C-town_PU2closed.inp` is the model used to *generate* the PU2 scenario's
measurements. Estimation itself runs against `our_C-town.inp` — the estimator
is not told the pump closed, which is the point of the experiment.

### Supplemental Materials

| Folder | Produces |
|---|---|
| `snapshot_se/Examples_Red1/Example1` | §S1, Table S1 — recovering solvability |
| `snapshot_se/Examples_Red1/Example2` | §S1, Table S2 — realistic sensor accuracies |
| `snapshot_se/Examples_Red1/Example2_NO_OBSERVABLE` | §S2, Table S3 — observability with low-weight measurements |
| `snapshot_se/Examples_Net1` (`*1_*`) | §S3, Table S4 — leak with emitter 2 at J21 |
| `snapshot_se/Examples_Net1` (`*2_*`, `*3_*`) | §S3, Table S5 — diagnostic re-estimation |
| `snapshot_se/Examples_Net1/Net1_RESULTS3_J21/J31/J32.csv` | §S3, the three candidate locations |
| `multiperiod_se/…/LEAK_J59_emitter0_1` | Fig. S9 — the small-leak scenario |

Red1 is toy network A in the Supplemental Materials; Net1 is toy network B.

---

## Two naming notes

`LEAK_J59_emitter0_1` is the emitter coefficient 0.1 case. The folder was
`LEAK_J59_emitter0,1` locally — Spanish decimal comma — renamed here because
a comma in a path is a nuisance in scripts.

`Example2_NO_OBSERVABLE` replaces a folder name with spaces, for the same
reason.

---

## Before you can run any of this

You need the executables and an EPANET library. Either extract one of the ZIP
archives at the repository root, which ship both, or build from `../src/`
following `../src/DEPENDENCIES.md` and `../src/build/BUILD.md`.

Run unattended — the default — or set `SENSEI_INTERACTIVE=1` to step through
each stage of the computation.

Read `../PROVENANCE.md` before treating a rerun as a reproduction of the
published numbers. It states precisely what has and has not been verified.
