/*
 * CARDGAME:0x8009dec4 CARDGAME_F0x8009dec4
 * 164 bytes at CARDGAME.PRO offset 0x1b214 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009dec4
 *  Symbols     (none)
 *  Compare     164 bytes from 0x8009dec4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009dec4
 */

#include "common/types.h"

int32_t CARDGAME_F0x8009dec4(uint8_t *a0, uint8_t *a1) {
    int32_t s1 = 0;
    uint8_t *s0 = a0;
    uint8_t *s2 = a1;
    if (*(int32_t *)(s0 + 0x4e8) != 0) goto L1;
    *(uint8_t *)(s0 + 0x499) = 2;
    {
        /* Slot writer 0x8009d4c0 installs 0x8009c350, which accesses a0. */
        uint8_t *obj = *(uint8_t **)(s2 + 0x18);
        (*(void (**)(uint8_t *))(obj + 0xec4))(obj);
    }
    *(int32_t *)(s0 + 0x4e8) = *(int32_t *)(s0 + 0x4e8) + 1;
L1:
    if (*(int16_t *)(*(int32_t *)(s2 + 0x18) + 0x64) != 0) goto L2;
    if (*(uint8_t *)(s0 + 0x498) != 0) goto L2;
    s1 = 1;
L2:
    return s1;
}
