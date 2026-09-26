/*
 * CARDGAME:0x800952f4 CARDGAME_F0x800952f4
 * 104 bytes at CARDGAME.PRO offset 0x12644 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800952f4
 *  Symbols     (none)
 *  Compare     104 bytes from 0x800952f4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800952f4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Two-step state p1[0x422]: in state 1, once p1[0x498] is clear and the object
 * p2 reports state 2 (+0x64), advances to state 2 and bumps the slot index
 * p1[0x575]; state 2 returns 1. Returns 0 otherwise.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

int32_t CARDGAME_F0x800952f4(int32_t p1, int32_t p2)
{
    int32_t ret;

    ret = 0;
    switch (U8(p1, 0x422)) {
    case 1:
        if (U8(p1, 0x498) == 0 && S16(p2, 0x64) == 2) {
            U8(p1, 0x422) = 2;
            U8(p1, 0x575)++;
        }
        break;
    case 2:
        ret = 1;
        break;
    }
    return ret;
}
