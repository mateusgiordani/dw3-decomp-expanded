/*
 * CARDGAME:0x8009a5b0 CARDGAME_F0x8009a5b0
 * 124 bytes at CARDGAME.PRO offset 0x17900 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009a5b0
 *  Symbols     D_8004df9c=0x8004df9c
 *  Compare     124 bytes from 0x8009a5b0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009a5b0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * 0x8009a5b8 is the internal framed entry.
 *
 * The direct caller at 0x8009ba10 targets the global callback-load prefix.
 */

#include "common/types.h"

typedef int32_t (*CardTick9a5b0)(void);
extern CardTick9a5b0 D_8004df9c;
int32_t CARDGAME_F0x8009a5b0(void *unused, uint8_t *ctx, int32_t limit)
{
    int32_t result = 0;
    int32_t value;
    int32_t elapsed;
    (void)unused;
    elapsed = D_8004df9c();
    value = *(int32_t *)(ctx + 0x28);
    value += elapsed;
    *(int32_t *)(ctx + 0x28) = value;
    if (value >= limit) {
        ctx[0x42] = 1;
        result = 1;
        *(int32_t *)(ctx + 0x2c) = 0;
        *(int32_t *)(ctx + 0x28) = 0;
        *(int16_t *)(ctx + 0x26) = 0;
        ctx[0x47] = 0;
    }
    return result;
}
