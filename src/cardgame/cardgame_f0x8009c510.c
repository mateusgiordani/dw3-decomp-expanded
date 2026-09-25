/*
 * CARDGAME:0x8009c510 CARDGAME_F0x8009c510
 * 200 bytes at CARDGAME.PRO offset 0x19860 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009c510
 *  Symbols     CARDGAME_F0x8009c510=0x8009c510 DAT_8004B600=0x8004b600
 *  Compare     200 bytes from 0x8009c510 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009c510
 */

#include "common/types.h"

extern void (*DAT_8004B600)(void *, int32_t);

int CARDGAME_F0x8009c510(uint8_t *a0, int32_t a1, int32_t a2, int32_t a3) {
    void (*fn)(void *, int32_t);
    int32_t off;
    int32_t off2;
    uint8_t *elem;
    fn = DAT_8004B600;
    off = a1 * 76;
    off2 = off + 0x108;
    fn(a0 + off2, 76);
    elem = a0 + off;
    elem[0x14d] = 1;
    elem[0x14a] = 1;
    *(int32_t *)(elem + 0x108) = a2;
    *(int32_t *)(elem + 0x10c) = a3;
    *(int16_t *)(elem + 0x124) = 0x1000;
    *(int16_t *)(elem + 0x120) = 0x1000;
    *(int16_t *)(elem + 0x126) = 0x1000;
    *(int16_t *)(elem + 0x122) = 0x1000;
    *(int16_t *)(elem + 0x142) = 0;
    *(int16_t *)(elem + 0x140) = 0;
    *(int16_t *)(elem + 0x12c) = (int16_t)a1;
    elem[0x14b] = 0;
    elem[0x14c] = 0;
    elem[0x14f] = 0;
    elem[0x151] = 0;
    *(int16_t *)(elem + 0x12e) = 0;
    return 0;
}
