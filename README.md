# SENSEI — Smart watEr NetworkS using artificial intEllIgence (SENSEI_Software)

<!--
TIP: Put your funding banner at docs/funding.png (or change the path below).
Recommended size: ~1400×300 px, PNG.
-->
![Funding](docs/ProjectFunding.png)

This repository contains the **SENSEI software** released in the context of the project:

> **SENSEI: Smart watEr NetworkS using artificial intEllIgence** (code **CNS2022-135472**), funded by **MCIN/AEI/10.13039/501100011033** and the European Union **«Next Generation EU»/PRTR**.

SENSEI provides **three executable tools** (Windows binaries) to support **model calibration** and **state estimation (SE)** for water distribution networks (EPANET `.inp` models), using measurements and/or pseudomeasurements stored in `.csv` files:

1. **Calibration** (multi-period, Levenberg–Marquardt with staged/coordinate-descent options)  
2. **Pseudo-dynamic (multiperiod) State Estimation** (sequential over time periods, Levenberg–Marquardt)  
3. **Snapshot (instantaneous) State Estimation** (single time instant, Gauss–Newton)

The user guides shipped with the executables (PDFs in each folder) are the **authoritative reference** for inputs, configuration, and expected outputs:
- Calibration guide (PDF): see `Calibration/Calibration_UserGuide_ENG.pdf`. fileciteturn0file2  
- Multiperiod SE guide (PDF): see `Multiperiod_SE/Multiperiod_SE_UserGuide_ENG.pdf`. fileciteturn0file1  
- Snapshot SE guide (PDF): see `Snapshot_SE/Snapshot_SE_UserGuide_ENG.pdf`. fileciteturn0file0  

---

## Quick start (Windows)

Each tool lives in its own folder and is designed to run **as-is**:

- `Calibration/Calibration_SENSEI.exe`
- `Multiperiod_SE/Multiperiod_SE_SENSEI.exe`
- `Snapshot_SE/Snapshot_SE_SENSEI.exe`

Each folder also includes:
- a **fixed-name configuration file** (`config_*.txt`) that the executable reads, and
- required **dynamic libraries** (`epanet2.dll` plus SuiteSparse/OpenBLAS-related `.dll` files).

### Do / Don’t rules (important)

**Do not:**
- delete any `.dll` files,
- rename the configuration file (the `.exe` expects the exact filename),
- change the internal folder structure (paths are assumed by the executable). fileciteturn0file2 fileciteturn0file1 fileciteturn0file0  

**You may:**
- modify measurement/pseudomeasurement `.csv` values,
- rename example subfolders/files **as long as you update the corresponding names inside the config `.txt`**,
- change algorithm hyperparameters in the config file (recommended only after a first run with defaults). fileciteturn0file2 fileciteturn0file1 fileciteturn0file0  

---

## Repository layout (main deliverables)

This repository is organized into three top-level deliverable folders:

```
Calibration/
Multiperiod_SE/
Snapshot_SE/
```

### 1) `Calibration/` — calibration tool

**Purpose.** Calibrate an EPANET model using a time series of measurements/pseudomeasurements over **T periods**. The calibration can be configured in **stages** (coordinate-descent style) via an `options.csv` file. fileciteturn0file2  

**What you will find:**
- `Calibration_SENSEI.exe` (executable)
- `config_calibration.txt` (configuration; name must not change) fileciteturn0file2  
- `C-town_Calib_Example/` (example dataset, including `C-town0.inp`, measurement `.csv` files and auxiliary option files) fileciteturn0file2  
- `epanet2.dll` + additional required `.dll` libraries

**Main inputs (high level).**
- `info_File` (seed EPANET model, `.inp`)
- Measurement/pseudomeasurement files (optional except demands):
  - `dm_File` (demands; **mandatory**, one row per demand node) fileciteturn0file2  
  - `wlm_File` (tank levels; optional)
  - `pm_File` (pressures; optional)
  - `fm_File` (instantaneous flows; optional)
  - `fm_avg_File` (averaged flows; optional) fileciteturn0file2  
- Calibration option files (only needed if the corresponding option is enabled), e.g.:
  - `options_File` (stages / variable groups / chronology) fileciteturn0file2  
  - `clusters_pipes_File`, `common_HW_File`, `common_DW_File`, `main_pipes_File` (roughness grouping/initialization) fileciteturn0file2  

**Algorithm notes.**
- Numerical Jacobian via small relative perturbations (default `epsilon = 0.2%`). fileciteturn0file2  
- Levenberg–Marquardt with stage loops (`max_loops`, `max_it`, `max_attempts`, `lambda_init`, `tol`). fileciteturn0file2  

**Outputs.**
- Calibrated model `.inp` saved with timestamp suffix (e.g., `*_calibrated_model_YYYYMMDD_hhmmss.inp`). fileciteturn0file2  
- Uncertainty propagation `.csv` (posterior CVs for optimized variables). fileciteturn0file2  

**EPANET-specific requirements to be aware of.**
- If optimizing pump-curve variables, SENSEI assumes pump curves have **exactly 3 points**. fileciteturn0file2  
- Valve-setting/status handling has specific constraints due to EPANET toolkit limitations (see calibration guide). fileciteturn0file2  

---

### 2) `Multiperiod_SE/` — pseudo-dynamic (sequential) state estimation

**Purpose.** Estimate the most likely **state variables** over time given measurements/pseudomeasurements:
- average demands during each period,
- tank levels at the beginning of each period (and at the final instant),
and optionally pressures/flows. fileciteturn0file1  

This tool runs **sequentially by period**: it simulates only one interval at a time, carries forward relevant states, and can track control-element statuses across periods. fileciteturn0file1  

**What you will find:**
- `Multiperiod_SE_SENSEI.exe`
- `config_multiperiod_SE.txt` (name must not change) fileciteturn0file1  
- `SE_Examples_our_C-town/` with:
  - `our_C-town0.inp` (reference model)
  - example scenarios such as `NORMAL/`, `BIAS_FM_T3/`, `LEAK_J59_emitter1/` fileciteturn0file1  
- required `.dll` libraries

**Main inputs.**
- `info_File` (`.inp`) providing topology and time parameters (`DURATION`, `HYDRAULIC TIMESTEP`, `PATTERN TIMESTEP`). fileciteturn0file1  
- Measurement/pseudomeasurement `.csv` files:
  - `dm_File` (**mandatory**) — average demand per interval at all demand nodes; must be fully filled. fileciteturn0file1  
  - `wlm_File` (**mandatory**) — tank levels at the start of each interval and at final instant; must be fully filled. fileciteturn0file1  
  - Optional: `pm_File`, `fm_File`, `fm_avg_File` (blank cells mean missing measurement → zero weight). fileciteturn0file1  

**Algorithm notes.**
- Numerical Jacobian (default `epsilon = 0.2%`). fileciteturn0file1  
- Levenberg–Marquardt per period with regularization `lambda_init`, retry logic up to `max_attempts`, and convergence tolerance `tol`. fileciteturn0file1  
- Control-element status can be carried forward period-to-period to preserve continuity. fileciteturn0file1  

**Leak hypothesis support (optional).**
The config includes fields that allow re-running SE while estimating an emitter coefficient at a chosen node (to test a leak hypothesis), starting from a specified period. fileciteturn0file1  

**Outputs.**
Stored in `PATH` with timestamps to avoid overwriting:
- `*_estimated_values_YYYYMMDD_hhmmss.csv`
- `*_normalized_residuals_YYYYMMDD_hhmmss.csv`
- `*_posterior_CV_YYYYMMDD_hhmmss.csv` fileciteturn0file1  

---

### 3) `Snapshot_SE/` — instantaneous (snapshot) state estimation

**Purpose.** A compact, illustrative state-estimation tool for a **single instant** (no time horizon), used to see how adding/removing measurements affects the adjustment. fileciteturn0file0  

**What you will find:**
- `Snapshot_SE_SENSEI.exe`
- `config_snapshot_SE.txt` (name must not change) fileciteturn0file0  
- `Examples_Red1/` example network with two scenarios (e.g., `Example1/`, `Example2/`) and their measurement files fileciteturn0file0  
- required `.dll` libraries

**Main inputs.**
- `info_File` (`.inp`) for topology and fixed settings; time parameters are irrelevant (simulation duration is set to 0). fileciteturn0file0  
- Measurement/pseudomeasurement `.csv` files, each row = sensor:
  - `dm_File` (**mandatory**) — demand at demand nodes (must cover all demand nodes)
  - `wlm_File` (**mandatory**) — tank levels (must cover all tanks)
  - Optional: `pm_File` (pressures), `fm_File` (instantaneous flows) fileciteturn0file0  

**Algorithm notes.**
- Numerical Jacobian (default `epsilon = 0.2%`). fileciteturn0file0  
- Gauss–Newton (compact NLS), default `max_it = 5`, `tol = 3e-4`. fileciteturn0file0  

**Outputs.**
A single results `.csv` saved in `PATH` with timestamp suffix:
`<info_File>_results_YYYYMMDD_hhmmss.csv` with measured values, estimated values, posterior CV and normalized residual. fileciteturn0file0  

---

## How to reproduce the packaged examples

### Calibration example (C-town)
1. Open `Calibration/`
2. Run `Calibration_SENSEI.exe`
3. (Optional) inspect / edit:
   - `config_calibration.txt` to change `PATH` or select different input files
   - `C-town_Calib_Example/options.csv` to change enabled stages/options fileciteturn0file2  

### Multiperiod SE examples (our_C-town)
1. Open `Multiperiod_SE/`
2. Run `Multiperiod_SE_SENSEI.exe`
3. Switch scenario by editing `config_multiperiod_SE.txt` to point to:
   - `NORMAL/` or `BIAS_FM_T3/` or `LEAK_J59_emitter1/` measurement sets fileciteturn0file1  

### Snapshot SE examples (Red1)
1. Open `Snapshot_SE/`
2. Run `Snapshot_SE_SENSEI.exe`
3. Switch `Example1/` vs `Example2/` in `config_snapshot_SE.txt`. fileciteturn0file0  

---

## Using SENSEI on your own network/data

At a minimum you will need:
- an EPANET `.inp` model (`info_File`),
- `.csv` measurement and/or pseudomeasurement files following the formats in the corresponding guide,
- a correct `PATH` in the `config_*.txt` file pointing to your case folder,
- and correct relative paths inside the config to your `.inp` and `.csv` files.

> **Important:** keep the config filenames unchanged (e.g., `config_multiperiod_SE.txt`), and keep the folder structure intact. fileciteturn0file2 fileciteturn0file1 fileciteturn0file0  

---

## Contact and source files

This repository primarily distributes **executables** and example datasets for replication and use.

People interested in **source files for use, improvement and/or adaptation** may contact the project leader:

**Roberto Mínguez** — rminguez@est-econ.uc3m.es

---

## License

Add the license used for this repository here (e.g., MIT, BSD-3, GPL-3.0). If some components (e.g., EPANET toolkit) have separate licenses, note them explicitly.
