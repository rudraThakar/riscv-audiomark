RISCV_PREFIX ?= riscv64-unknown-elf
CC := $(RISCV_PREFIX)-gcc

CFLAGS = -O2 -march=rv64gcv_zvl128b -mabi=lp64d -Wall

LDFLAGS =

SRC = src/harness.c src/q15_axpy_scalar.c src/q15_axpy_rvv.c
OUT = q15_axpy.elf

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
