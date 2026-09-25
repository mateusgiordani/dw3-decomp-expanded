/*
 * CARDGAME:0x800a0548 CARDGAME_F0x800a0548
 * 72 bytes at CARDGAME.PRO offset 0x1d898 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a0548
 *  Symbols     CARDGAME_F0x8009edcc=0x8009edcc CARDGAME_F0x800a0548=0x800a0548
 *  Compare     72 bytes from 0x800a0548 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a0548
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Original delivery remains preserved in worker commit
 * 1a80e22775ac5a0af3b27b0e8fd872d513c2ae08.
 */

#include "common/types.h"

extern void CARDGAME_F0x8009edcc(uint8_t *);
typedef void (*CardCall814)(uint8_t *, uint8_t *, uint32_t, int32_t);
void CARDGAME_F0x800a0548(uint8_t *ctx)
{
    ctx[0x574] = 0;
    CARDGAME_F0x8009edcc(ctx);
    (*(CardCall814 *)(ctx + 0x814))(ctx, ctx + 0x600,
        (uint32_t)(int32_t)*(int16_t *)(ctx + 0x5a6) << 16, 0);
}
