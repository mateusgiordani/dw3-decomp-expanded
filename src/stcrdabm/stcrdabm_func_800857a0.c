/*
 * STCRDABM:0x800857a0 STCRDABM_func_800857a0
 * 256 bytes at STCRDABM.PRO offset 0x2af0 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float
 *  Variant     base
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x800857a0
 *  Symbols     D0x80044b38=0x80044b38 D0x8005cca8=0x8005cca8
 *  Compare     256 bytes from 0x800857a0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x800857a0
 */

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
