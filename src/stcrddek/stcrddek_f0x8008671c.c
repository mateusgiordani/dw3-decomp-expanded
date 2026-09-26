/*
 * STCRDDEK:0x8008671c STCRDDEK_F0x8008671c
 * 132 bytes at STCRDDEK.PRO offset 0x3a6c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008671c
 *  Symbols     STCRDDEK_func_80086664=0x80086664
 *              STCRDDEK_func_8008666c=0x8008666c
 *              STCRDDEK_func_800866a8=0x800866a8
 *  Compare     132 bytes from 0x8008671c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x8008671c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x20 (s0-s1, ra).
 *
 * Per-frame update keyed by the object's state word p1[0xc]: state 1 runs
 * STCRDDEK_func_800866a8(p1, p2) then STCRDDEK_func_8008666c(p1); states 2 and
 * 3 do nothing; any other state calls the method at +0x38 and then
 * STCRDDEK_func_80086664(p1, p2).
 *
 * Matching note: `case 0:` shares the default body placed first, which gives
 * PAL's compare order (==1, <2, <4).
 */

#include <stdint.h>

typedef void (*method1_t)(int32_t);

void STCRDDEK_func_80086664(int32_t p1, int32_t p2);
void STCRDDEK_func_800866a8(int32_t p1, int32_t p2);
void STCRDDEK_func_8008666c(int32_t p1);

#define S32(p, o)    (*(int32_t *)((p) + (o)))

void STCRDDEK_F0x8008671c(int32_t p1, int32_t p2)
{
    switch (S32(p1, 0xc)) {
    case 0:
    default:
        ((method1_t)S32(p1, 0x38))(p1);
        STCRDDEK_func_80086664(p1, p2);
        break;
    case 1:
        STCRDDEK_func_800866a8(p1, p2);
        STCRDDEK_func_8008666c(p1);
        break;
    case 2:
    case 3:
        break;
    }
}
