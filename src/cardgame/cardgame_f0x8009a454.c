/*
 * CARDGAME:0x8009a454 CARDGAME_F0x8009a454
 * 236 bytes at CARDGAME.PRO offset 0x177a4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009a454
 *  Symbols     CARDGAME_F0x8009a454=0x8009a454 D_8004DF9C=0x8004df9c
 *              D_800A5AC8=0x800a5ac8 EXE_F0x8002abcc=0x8002abcc
 *  Compare     236 bytes from 0x8009a454 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009a454
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Xrefs: 1 caller CARDGAME_F0x8009b890 via 0x8009b97c (case 7, return ignored);
 * callees: EXE F0x8002abcc (direct jal) + indirect via D_8004DF9C.
 */

#include "common/types.h"

extern int32_t EXE_F0x8002abcc(int32_t x);
extern int32_t (*D_8004DF9C)(void);
extern int16_t D_800A5AC8[];

int32_t CARDGAME_F0x8009a454(int32_t a0, int32_t *a1)
{
    int32_t tmp;
    int32_t flag;

    (void)a0;
    flag = 0;
    a1[0] = a1[4] + D_800A5AC8[a1[10] & 7];
    tmp = EXE_F0x8002abcc((a1[10] << 12) / 0x18);
    if (tmp < 0)
        tmp += 15;
    *(int16_t *)((char *)a1 + 0x1a) = (int16_t)(tmp >> 4) + 0x1000;
    a1[10] += D_8004DF9C();
    if (a1[10] >= 12) {
        /* Store-then-set: scheduler CSEs constant 1 with flag (move v0,s1;
           sb v0) and delays the 0x1000 materialization; flag-first form
           hoists li v0,0x1000 above the store (-1 word). */
        *(char *)((char *)a1 + 0x42) = 1;
        flag = 1;
        *(int16_t *)((char *)a1 + 0x26) = 0;
        *(char *)((char *)a1 + 0x47) = 0;
        *(int16_t *)((char *)a1 + 0x1a) = 0x1000;
        a1[0] = a1[4];
    }
    return flag;
}
