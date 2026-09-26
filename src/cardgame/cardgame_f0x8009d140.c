/*
 * CARDGAME:0x8009d140 CARDGAME_F0x8009d140
 * 28 bytes at CARDGAME.PRO offset 0x1a490 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009d140
 *  Symbols     D_800A5B14=0x800a5b14
 *  Compare     28 bytes from 0x8009d140 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009d140
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Returns entry i of the int16 table at 0x800a5b14.
 */

#include <stdint.h>

extern int16_t D_800A5B14[];

int32_t CARDGAME_F0x8009d140(int32_t i)
{
    return D_800A5B14[i];
}
