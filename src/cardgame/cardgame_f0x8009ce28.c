/*
 * CARDGAME:0x8009ce28 CARDGAME_F0x8009ce28
 * 108 bytes at CARDGAME.PRO offset 0x1a178 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009ce28
 *  Symbols     CARDGAME_F0x8009d310=0x8009d310 D_80055c48=0x80055c48
 *              _DAT_80055c48=0x80055c48
 *  Compare     108 bytes from 0x8009ce28 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009ce28
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Portable C recovery (rev 2, cardgame-human-v1).
 */

#include "common/types.h"

extern int32_t (*D_80055c48)(uint32_t);

int32_t CARDGAME_F0x8009ce28(uint8_t *base, int32_t index) {
    uint8_t *rec;

    (void)D_80055c48(0x4001c);
    rec = base + index * 76;
    rec[0x14a] = 11;
    *(int32_t *)(rec + 0x134) = 0;
    *(int32_t *)(rec + 0x130) = 0;
    return 0;
}
