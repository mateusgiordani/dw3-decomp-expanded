/*
 * CARDGAME:0x8008daf4 CARDGAME_F0x8008daf4
 * 136 bytes at CARDGAME.PRO offset 0xae44 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008daf4
 *  Symbols     CARDGAME_F0x8008d938=0x8008d938 CARDGAME_F0x8008da2c=0x8008da2c
 *              CARDGAME_F0x8008daf4=0x8008daf4
 *  Compare     136 bytes from 0x8008daf4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008daf4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Original delivery remains preserved in worker commit
 * 32013dea50ca56abe6a17683ea2e7e808d38c14a.
 */

#include "common/types.h"

extern int32_t CARDGAME_F0x8008d938(void *, void *, int32_t);
extern void CARDGAME_F0x8008da2c(void *, void *, int32_t, int32_t);
int32_t CARDGAME_F0x8008daf4(void *ctx, void *other, int32_t side, int32_t index)
{
    /* Preserve PAL initialization order before preparing the adjusted index. */
    int32_t result = 0;
    int32_t adjusted = index;
    if (side != 0)
        adjusted += 6;
    if (CARDGAME_F0x8008d938(ctx, other, adjusted) != 0) {
        CARDGAME_F0x8008da2c(ctx, other, side, index);
        result = 1;
    }
    return result;
}
