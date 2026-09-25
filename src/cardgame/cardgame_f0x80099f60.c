/*
 * CARDGAME:0x80099f60 CARDGAME_F0x80099f60
 * 156 bytes at CARDGAME.PRO offset 0x172b0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80099f60
 *  Symbols     CARDGAME_F0x80098898=0x80098898 CARDGAME_F0x80099440=0x80099440
 *              CARDGAME_F0x80099be4=0x80099be4 CARDGAME_F0x80099f60=0x80099f60
 *  Compare     156 bytes from 0x80099f60 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80099f60
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Original delivery remains preserved in worker commit
 * b435f45bca8840da79aab0a3a9c67253c89f5439.
 */

#include "common/types.h"

extern void CARDGAME_F0x80099be4(uint8_t *, void *, int32_t, uint8_t *);
extern void CARDGAME_F0x80099440(uint8_t *, void *, int32_t);
extern void CARDGAME_F0x80098898(uint8_t *, void *, int32_t);
void CARDGAME_F0x80099f60(uint8_t *ctx, void *other)
{
    uint8_t *first;
    CARDGAME_F0x80099be4(ctx, other, 0, ctx + 0xa8);
    CARDGAME_F0x80099be4(ctx, other, 1, ctx + 0xfc);
    first = ctx + 0x60;
    CARDGAME_F0x80099440(first, other, 0);
    ctx += 0xb4;
    CARDGAME_F0x80099440(ctx, other, 1);
    CARDGAME_F0x80098898(first, other, 0);
    CARDGAME_F0x80098898(ctx, other, 1);
}
