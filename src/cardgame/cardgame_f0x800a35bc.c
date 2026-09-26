/*
 * CARDGAME:0x800a35bc CARDGAME_F0x800a35bc
 * 192 bytes at CARDGAME.PRO offset 0x2090c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a35bc
 *  Symbols     CARDGAME_F0x800a32d8=0x800a32d8 CARDGAME_F0x800a3488=0x800a3488
 *              D_800A5D94=0x800a5d94
 *  Compare     192 bytes from 0x800a35bc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a35bc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x18 (s0, ra).
 *
 * Per-frame update of a fade object keyed by its state word p1[0xc]: state 1
 * steps the fade (CARDGAME_F0x800a3488) while p1[0x50] is 1, draws it with the
 * rectangle at 0x800a5d94 (CARDGAME_F0x800a32d8) and, once the fade reports
 * done (p1[0x50] == 2), calls the object's method at +0x28 with 3; states 2 and
 * 3 do nothing; any other state calls the method at +0x38.
 *
 * Matching note: `case 0:` shares the default body placed first, which gives
 * PAL's compare order (==1, <2, <4).
 */

#include <stdint.h>

typedef struct {
    int16_t x;
    int16_t y;
} Vec2;

typedef void (*method1_t)(int32_t);
typedef void (*method2_t)(int32_t, int32_t);

extern Vec2 D_800A5D94[2];

void CARDGAME_F0x800a3488(int32_t p1);
void CARDGAME_F0x800a32d8(int32_t p1, Vec2 pos, Vec2 size);

#define S32(p, o)    (*(int32_t *)((p) + (o)))

void CARDGAME_F0x800a35bc(int32_t p1)
{
    switch (S32(p1, 0xc)) {
    case 0:
    default:
        ((method1_t)S32(p1, 0x38))(p1);
        break;
    case 1:
        if (S32(p1, 0x50) == 1)
            CARDGAME_F0x800a3488(p1);
        CARDGAME_F0x800a32d8(p1, D_800A5D94[0], D_800A5D94[1]);
        if (S32(p1, 0x50) == 2)
            ((method2_t)S32(p1, 0x28))(p1, 3);
        break;
    case 2:
    case 3:
        break;
    }
}
