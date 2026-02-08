#include <stdio.h>
#include <stdint.h>
#include <string.h>

extern void q15_axpy_rvv(const int16_t *a,
                         const int16_t *b,
                         int16_t *y,
                         int n,
                         int16_t alpha);

extern void q15_axpy_scalar(const int16_t *a,
                            const int16_t *b,
                            int16_t *y,
                            int n,
                            int16_t alpha);

static inline uint64_t rdcycle(void)
{
    uint64_t c;
    asm volatile ("rdcycle %0" : "=r"(c));
    return c;
}

#define N 256

int main(void)
{
    int16_t a[N], b[N];
    int16_t y_ref[N], y_vec[N];
    int16_t alpha = 12345;

    for (int i = 0; i < N; i++) {
        a[i] = (i * 37) - 16384;
        b[i] = (i * 13) - 8192;
        y_ref[i] = 0;
        y_vec[i] = 0;
    }

    q15_axpy_scalar(a, b, y_ref, N, alpha);

    uint64_t start = rdcycle();
    q15_axpy_rvv(a, b, y_vec, N, alpha);
    uint64_t end = rdcycle();

    for (int i = 0; i < N; i++) {
        if (y_ref[i] != y_vec[i]) {
            printf("FAIL at %d: ref=%d vec=%d\n",
                   i, y_ref[i], y_vec[i]);
            return 1;
        }
    }

    printf("PASS\n");
    printf("cycles: %lu\n", (unsigned long)(end - start));
    return 0;
}

