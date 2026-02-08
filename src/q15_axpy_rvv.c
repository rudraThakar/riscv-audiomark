#include <stdint.h>
#include <stddef.h>
#include <riscv_vector.h>

void q15_axpy_rvv(const int16_t *a,
                  const int16_t *b,
                  int16_t *y,
                  int n,
                  int16_t alpha)
{
    int i = 0;

    while (i < n) {
        size_t vl = __riscv_vsetvl_e16m1(n - i);

        vint16m1_t va16 = __riscv_vle16_v_i16m1(&a[i], vl);
        vint16m1_t vb16 = __riscv_vle16_v_i16m1(&b[i], vl);

        vint32m2_t va32 = __riscv_vwadd_vx_i32m2(va16, 0, vl);
        vint32m2_t prod32 = __riscv_vwmul_vx_i32m2(vb16, alpha, vl);
        vint32m2_t prod_q15 = __riscv_vsra_vx_i32m2(prod32, 15, vl);

        vint32m2_t sum32 = __riscv_vadd_vv_i32m2(va32, prod_q15, vl);

        vint32m2_t vmax = __riscv_vmv_v_x_i32m2(32767, vl);
        vint32m2_t vmin = __riscv_vmv_v_x_i32m2(-32768, vl);
        sum32 = __riscv_vmin_vv_i32m2(sum32, vmax, vl);
        sum32 = __riscv_vmax_vv_i32m2(sum32, vmin, vl);

        vint16m1_t y16 = __riscv_vnclip_wx_i16m1(
    sum32, 0, __RISCV_VXRM_RNE, vl);

        __riscv_vse16_v_i16m1(&y[i], y16, vl);

        i += vl;
    }
}

