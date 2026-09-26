/*
 * CARDGAME:0x800a4440 CARDGAME_F0x800a4440
 * 84 bytes at CARDGAME.PRO offset 0x21790 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a4440, jump table (.rodata) at 0x80083ba4
 *  Symbols     (none)
 *  Compare     84 bytes from 0x800a4440 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a4440
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Returns the value for `event` (jump table 0x80083ba4 over events 18..56): 18
 * -> 60, 20 -> 15, 56 -> 10, 21 and 46 -> 30, otherwise 0.
 */

#include <stdint.h>

int32_t CARDGAME_F0x800a4440(int32_t p1, int32_t event)
{
    int32_t v;

    v = 0;
    switch (event) {
    case 18:
        v = 60;
        break;
    case 20:
        v = 15;
        break;
    case 56:
        v = 10;
        break;
    case 21:
    case 46:
        v = 30;
        break;
    }
    return v;
}
