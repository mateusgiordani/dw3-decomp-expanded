/*
 * CARDGAME:0x8009ba3c CARDGAME_F0x8009ba3c
 * 140 bytes at CARDGAME.PRO offset 0x18d8c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009ba3c
 *  Symbols     CARDGAME_F0x8009a998=0x8009a998 CARDGAME_F0x8009acac=0x8009acac
 *              CARDGAME_F0x8009af28=0x8009af28 CARDGAME_F0x8009b028=0x8009b028
 *              CARDGAME_F0x8009b168=0x8009b168 CARDGAME_F0x8009b678=0x8009b678
 *              CARDGAME_F0x8009ba3c=0x8009ba3c
 *  Compare     140 bytes from 0x8009ba3c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009ba3c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Original delivery remains preserved in worker commit
 * 21951aa3d2d8afdbe8bf1408b01cd51952d40af6.
 */

#include "common/types.h"

extern void CARDGAME_F0x8009acac(void *, uint8_t *);
extern void CARDGAME_F0x8009b028(void *, uint8_t *);
extern void CARDGAME_F0x8009af28(void *, uint8_t *);
extern void CARDGAME_F0x8009a998(void *, uint8_t *);
extern void CARDGAME_F0x8009b678(void *, uint8_t *);
extern void CARDGAME_F0x8009b168(void *, uint8_t *);
void CARDGAME_F0x8009ba3c(void *ctx, uint8_t *row)
{
    if (*(int16_t *)(row + 0x18) != 0 && *(int16_t *)(row + 0x1a) != 0) {
        CARDGAME_F0x8009acac(ctx, row);
        CARDGAME_F0x8009b028(ctx, row);
        CARDGAME_F0x8009af28(ctx, row);
        CARDGAME_F0x8009a998(ctx, row);
        CARDGAME_F0x8009b678(ctx, row);
        CARDGAME_F0x8009b168(ctx, row);
    }
}
