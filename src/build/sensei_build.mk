# ===================================================================
#  SENSEI -- build all programs
#
#  Reproduces the toolchain of the distributed executables:
#  GCC 14.2.0 (MSYS2 MinGW-w64 x86_64), -g, no optimisation, C17.
#
#  Usage, from 01_source/ :
#      make            build everything into bin/
#      make dist       assemble runnable folders with the DLLs
#      make clean
#
#  Set OPT=-O2 to build optimised. Results may then differ in the last
#  significant figures from the published tables; see BUILD.md §3.
# ===================================================================

CC      ?= gcc
OPT     ?=
CFLAGS  ?= -g -std=c17 $(OPT)
INCLUDE := -I include
LIBDIR  := -L libs
LIBS    := -lsuitesparseconfig -lcholmod -lcolamd -lamd -lccolamd -lcamd \
           -lepanet2 -lm
LDFLAGS := -static-libgcc

CORE    := core/functions_SENSEI.c core/messages_SENSEI.c
HEADERS := include/functions_SENSEI.h include/messages_SENSEI.h

BIN     := bin

# program name  ->  directory holding its .c
PROGS := \
  Calibration_SENSEI:calibration \
  GenerarMedidas_Calibration_SENSEI:calibration \
  Multiperiod_SE_SENSEI:multiperiod_se \
  Emitter_Optimization_Multiperiod_SE:multiperiod_se \
  GenerarMedidas_Multiperiod_SENSEI:multiperiod_se \
  Snapshot_SE_SENSEI:snapshot_se \
  GenerarMedidas_Snapshot_SENSEI:snapshot_se

ifeq ($(OS),Windows_NT)
  EXT := .exe
else
  EXT :=
endif

TARGETS := $(foreach p,$(PROGS),$(BIN)/$(word 1,$(subst :, ,$(p)))$(EXT))

.PHONY: all clean dist
all: $(TARGETS)

# One rule per program, generated.
define RULE
$(BIN)/$(1)$(EXT): $(2)/$(1).c $(CORE) $(HEADERS) | $(BIN)
	$(CC) $(CFLAGS) $$< $(CORE) -o $$@ $(INCLUDE) $(LIBDIR) $(LIBS) $(LDFLAGS)
endef
$(foreach p,$(PROGS),$(eval $(call RULE,$(word 1,$(subst :, ,$(p))),$(word 2,$(subst :, ,$(p))))))

$(BIN):
	mkdir -p $(BIN)

# --- Runnable folders with the runtime DLLs (Windows) ---------------
MINGW_BIN ?= /mingw64/bin
RUNTIME_DLLS := libcholmod.dll libamd.dll libcamd.dll libcolamd.dll \
                libccolamd.dll libsuitesparseconfig.dll libopenblas.dll \
                libgfortran-5.dll libquadmath-0.dll libgomp-1.dll \
                libwinpthread-1.dll

dist: all
	@for p in $(foreach p,$(PROGS),$(word 1,$(subst :, ,$(p)))); do \
	  mkdir -p dist/$$p; \
	  cp $(BIN)/$$p$(EXT) dist/$$p/; \
	  cp libs/epanet2.dll dist/$$p/; \
	  for d in $(RUNTIME_DLLS); do \
	    [ -f $(MINGW_BIN)/$$d ] && cp $(MINGW_BIN)/$$d dist/$$p/ || \
	      echo "  missing: $$d (copy it by hand from your MinGW bin)"; \
	  done; \
	  echo "dist/$$p ready"; \
	done

clean:
	rm -rf $(BIN) dist
