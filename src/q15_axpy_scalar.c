#include <stdint.h>

static inline int16_t sat_q15(int32_t x)
{
    if (x > 32767) return 32767;
    if (x < -32768) return -32768;
    return (int16_t)x;
}

void q15_axpy_scalar(const int16_t *a,
                     const int16_t *b,
                     int16_t *y,
                     int n,
                     int16_t alpha)
{
    for (int i = 0; i < n; i++) {
        int32_t prod = (int32_t)alpha * b[i];   // Q30
        int32_t tmp  = (prod >> 15) + a[i];     // back to Q15
        y[i] = sat_q15(tmp);
    }
}

