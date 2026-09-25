/*
 * CARDGAME:0x8009c1cc CARDGAME_F0x8009c1cc
 * 72 bytes at CARDGAME.PRO offset 0x1951c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009c1cc
 *  Symbols     CARDGAME_F0x8009c1cc=0x8009c1cc D_80055c48=0x80055c48
 *  Compare     72 bytes from 0x8009c1cc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009c1cc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Original delivery remains preserved in worker commit
 * 4dd5402419861832977644d605c52e525694147e.
 */

#include "common/types.h"

extern void (*D_80055c48)(uint32_t);
void CARDGAME_F0x8009c1cc(uint8_t *ctx)
{
    D_80055c48(0x4001a);
    *(uint16_t *)(ctx + 0xe02) = 6;
    *(uint16_t *)(ctx + 0xe00) = 6;
    *(uint16_t *)(ctx + 0xe0a) = 5;
}
