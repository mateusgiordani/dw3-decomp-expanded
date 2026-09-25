/*
 * STCRDDEK:0x800847c0 STCRDDEK_func_800847c0
 * 208 bytes at STCRDDEK.PRO offset 0x1b10 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800847c0
 *  Symbols     D0x80048d34=0x80048d34 D0x80049372=0x80049372
 *              STCRDDEK_func_80085210=0x80085210
 *  Compare     208 bytes from 0x800847c0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800847c0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * function_labels.csv: STCRDDEK,0x800847c0,function,STCRDDEK_func_800847c0,208.
 *
 * Callers: 3 direct jal sites inside STCRDDEK_func_80085210 (0x800852b8 a0=s2,
 * 0x8008587c, 0x800861c0), each passing a struct pointer.
 *
 * Tables are absolute EXE-data addresses (lui 0x8005, -G0, no small data):
 *
 * D0x80048d34 base crib, element read at [i+0x3a2] (true table 0x800490d6);
 *
 * D0x80049372 row table, 0x66-byte rows of int16, row picked by *(arg0+0x5c).
 *
 * Layout used (conservative, offsets only): +0x5c word selector, +0x7c word
 * survivor count, +0x88 int16 list (315 entries), +0x2fe int8 counts (315).
 */

#include <stdint.h>

extern uint8_t D0x80048d34[];
extern uint8_t D0x80049372[];

void STCRDDEK_func_800847c0(uint8_t *arg0)
{
    int16_t *dst;
    int i;
    uint8_t *tbl;
    int16_t *row;
    int j;
    int k;
    int16_t *out;

    i = 0;
    tbl = D0x80048d34;
    dst = (int16_t *)arg0;
    do {
        dst[0x44] = 0;
        *(arg0 + i + 0x2FE) = tbl[i + 0x3A2];
        dst++;
        i++;
    } while (i < 0x13B);

    row = (int16_t *)(D0x80049372 + (*(int32_t *)(arg0 + 0x5C)) * 0x66);
    j = 0;
    do {
        j++;
        k = *row++;
        (*(arg0 + k + 0x2FE))--;
    } while (j < 0x28);

    *(int32_t *)(arg0 + 0x7C) = 0;
    i = 0;
    out = (int16_t *)arg0;
    do {
        if (*(int8_t *)(arg0 + i + 0x2FE) > 0) {
            out[0x44] = (int16_t)i;
            out++;
            (*(int32_t *)(arg0 + 0x7C))++;
        }
        i++;
    } while (i < 0x13B);
}
