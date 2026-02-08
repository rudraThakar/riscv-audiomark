# RISC-V Audiomark – Q15 AXPY using RVV

This repository contains my solution to the **RISC-V Audiomark coding challenge**, which focuses on implementing a **saturating multiply–accumulate (AXPY)** operation using the **RISC-V Vector Extension (RVV)**.

The implemented function computes, for all `i`:

y[i] = sat_q15(a[i] + alpha * b[i])



where `a`, `b`, and `y` are `int16_t` (Q15) vectors and `alpha` is a scalar `int16_t`.

---

## Overview

The solution consists of:

- A **scalar reference implementation** for correctness
- A **pure RVV-based implementation** written using RVV v1.0 intrinsics
- A small test harness that verifies **bit-for-bit correctness**

The RVV implementation is written to be **vector-length agnostic** and is intended to run on **RV64 (or RV32) systems with RVV support enabled**.

This repository intentionally uses the **direct RVV implementation** (without conditional fallbacks), as the focus of the challenge is RVV correctness and design rather than portability to non-RVV systems.

---

## RVV Implementation Details

The RVV implementation follows this structure:

1. Dynamically set the vector length using `vsetvl`
2. Load input vectors `a` and `b` using `vle16`
3. Widen `a` to 32-bit to avoid overflow
4. Perform a widened multiply `alpha * b` using `vwmul`
5. Accumulate the result with `a` using `vadd`
6. Explicitly **saturate** the result to the Q15 range `[-32768, 32767]`
7. Narrow the saturated result back to `int16_t` using `vnclip`
8. Store the output using `vse16`

Key design choices:

- **Vector-length agnostic**: the code does not assume any fixed VLEN
- **Explicit saturation**: saturation is handled before narrowing
- **RVV v1.0 intrinsics**: uses canonical `__riscv_*` intrinsic names
- **No implicit rounding or scaling assumptions**

The RVV implementation is bit-exact with the scalar reference for all tested inputs.

---

## Correctness

Correctness is validated by:

- Running the scalar reference implementation
- Running the RVV implementation on the same inputs
- Comparing outputs element-by-element
- Ensuring **bit-for-bit equality**

The test harness reports:
- Verification status (PASS / FAIL)
- Maximum absolute difference (expected to be zero)

---

## Toolchain

The code was compiled using the **RISC-V GNU embedded toolchain (GCC 15)** with RVV enabled.

Example build command:

```bash
riscv64-unknown-elf-gcc \
  -O2 -march=rv64gcv_zvl128b -mabi=lp64d \
  src/harness.c src/q15_axpy_scalar.c src/q15_axpy_rvv.c \
  -o q15_axpy.elf

