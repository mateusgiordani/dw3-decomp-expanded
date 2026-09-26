/*
 * CARDGAME:0x800a3d90 CARDGAME_F0x800a3d90
 * 268 bytes at CARDGAME.PRO offset 0x210e0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a3d90
 *  Symbols     CARDGAME_F0x800a3934=0x800a3934
 *  Compare     268 bytes from 0x800a3d90 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a3d90
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x38 (s0-s8, ra).
 *
 * For one side (count p1+side*0x72+0x72c) keeps only the flagged card (flags
 * p1+0x446, or p1+0x44c for side 1) whose score from CARDGAME_F0x800a3934(p1,
 * side, 1 << i) is lowest (ties go to the later card).
 *
 * Matching note: the side offset `o` is a named variable set in the loop
 * condition, which adds it base-first (p1 + o) as in PAL.
 */

#include <stdint.h>

int32_t CARDGAME_F0x800a3934(int32_t p1, int32_t side, int32_t mask);

#define U8(p, o)     (*(uint8_t *)((p) + (o)))

void CARDGAME_F0x800a3d90(int32_t p1, int32_t side)
{
    int8_t *flags;
    int32_t best;
    int32_t bi;
    int32_t i;
    int32_t v;
    int32_t o;

    best = 0xfff;
    bi = 0;
    if (side == 1)
        flags = (int8_t *)(p1 + 0x44c);
    else
        flags = (int8_t *)(p1 + 0x446);
    for (i = 0; o = side * 0x72, i < U8(p1 + o, 0x72c); i++) {
        if (flags[i] != 0) {
            v = CARDGAME_F0x800a3934(p1, side, 1 << i);
            if (v <= best) {
                best = v;
                flags[bi] = 0;
                bi = i;
                flags[i] = 1;
            } else {
                flags[i] = 0;
            }
        }
    }
}
