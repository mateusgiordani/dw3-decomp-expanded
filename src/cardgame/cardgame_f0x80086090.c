/*
 * CARDGAME:0x80086090 CARDGAME_F0x80086090
 * 68 bytes at CARDGAME.PRO offset 0x33e0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80086090
 *  Symbols     D_8005CCB0=0x8005ccb0 D_800A5844=0x800a5844
 *  Compare     68 bytes from 0x80086090 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80086090
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Returns entry [D_8005CCB0][a][b][c] of the int16 table at 0x800a5844 (shape
 * [][3][4][2]).
 */

#include <stdint.h>

extern int16_t D_800A5844[][3][4][2];
extern int32_t D_8005CCB0;

int32_t CARDGAME_F0x80086090(int32_t a, int32_t b, int32_t c)
{
    return D_800A5844[D_8005CCB0][a][b][c];
}
