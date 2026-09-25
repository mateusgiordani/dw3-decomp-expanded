/*
 * STCRDABM:0x80083bfc STCRDABM_func_80083bfc
 * 536 bytes at STCRDABM.PRO offset 0xf4c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80083bfc
 *  Symbols     F0x8001ae38=0x8001ae38
 *  Compare     536 bytes from 0x80083bfc against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80083bfc
 */

#include "common/types.h"

extern void *F0x8001ae38(int32_t a0, int32_t a1, int32_t a2, int32_t a3);

/*
 * Builds 17 EXE object handles via F0x8001ae38 into out[0..16], keyed by the
 * sign-extended halfword at arg0+0x50, then re-arms (count-2) entries of the
 * table at *(arg0+0x24) through each entry's slot 0x15c with *(arg0+0x54)-3.
 * Sole PAL caller: STCRDABM_func_80085540 (jal at 0x800855dc).
 *
 * NOTE: `i = 0` after the 17 calls (not before) is required for the prologue
 * scheduling match (li a1/a2/a3 before saves, s2 cleared in the first jal delay
 * slot).
 */
void STCRDABM_func_80083bfc(int32_t arg0, int32_t *arg1) {
    int32_t *ptr;
    int32_t limit;
    int32_t i;
    int32_t obj;

    arg1[0] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x10, 0x19);
    arg1[1] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0xd0, 0x19);
    arg1[2] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x34, 0x9e);
    arg1[3] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x35, 0x9e);
    arg1[4] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x4a, 0x9e);
    arg1[5] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x12, 0x6c);
    arg1[6] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x121, 0x6c);
    arg1[7] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x88, 0xa1);
    arg1[8] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x115, 0xa1);
    arg1[9] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x126, 0xa1);
    arg1[10] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x115, 0xc7);
    arg1[11] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x12c, 0xc7);
    arg1[12] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0x50, 0xb8);
    arg1[13] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0xce, 0xb8);
    arg1[14] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0xf0, 0xb8);
    arg1[15] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0xce, 0xc5);
    arg1[16] = (int32_t)F0x8001ae38(*(int16_t *)(arg0 + 0x50), 1, 0xf0, 0xc5);
    i = 0;
    ptr = *(int32_t **)(arg0 + 0x24);
    limit = *(int32_t *)(arg0 + 0x20) - 2;
    if (limit > 0) {
        do {
            i++;
            obj = *ptr;
            ((void (*)(int32_t, int32_t))*(int32_t *)(obj + 0x15c))(obj, *(int32_t *)(arg0 + 0x54) - 3);
            ptr++;
        } while (i < (*(int32_t *)(arg0 + 0x20) - 2));
    }
}
