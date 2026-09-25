#include "common/types.h"

/* STCRDABM:0x800857a0 (256 bytes, boundary 0x800857a0..0x800858a0)
 * PAL-SLES-03936; reviewed body-800857a0, base 0x80082cb0.
 * Polls the EXE helper vector at base 0x80044b38 + 0x404 with fixed ids
 * 0x7f6..0x7fa, then with *(0x8005cca8) + 0x16/+0x1d/+0x24.
 * Returns 1 on the first nonzero helper result, else 0.
 */
typedef int32_t (*STCRDABM_HelperFn)(int32_t);

extern uint8_t D0x80044b38[];
extern int32_t D0x8005cca8;

int32_t STCRDABM_func_800857a0(void) {
    uint8_t *base;

    base = D0x80044b38;
    if ((*(STCRDABM_HelperFn *)(base + 0x404))(0x7f6) != 0) {
        return 1;
    }
    if ((*(STCRDABM_HelperFn *)(base + 0x404))(0x7f7) != 0) {
        return 1;
    }
    if ((*(STCRDABM_HelperFn *)(base + 0x404))(0x7f8) != 0) {
        return 1;
    }
    if ((*(STCRDABM_HelperFn *)(base + 0x404))(0x7f9) != 0) {
        return 1;
    }
    if ((*(STCRDABM_HelperFn *)(base + 0x404))(0x7fa) != 0) {
        return 1;
    }
    if ((*(STCRDABM_HelperFn *)(base + 0x404))(D0x8005cca8 + 0x16) != 0) {
        return 1;
    }
    if ((*(STCRDABM_HelperFn *)(base + 0x404))(D0x8005cca8 + 0x1d) != 0) {
        return 1;
    }
    return (*(STCRDABM_HelperFn *)(base + 0x404))(D0x8005cca8 + 0x24) != 0;
}
