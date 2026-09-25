/*
 * CARDGAME:0x8009beac CARDGAME_F0x8009beac
 * 176 bytes at CARDGAME.PRO offset 0x191fc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009beac
 *  Symbols     D_80055c48=0x80055c48
 *  Compare     176 bytes from 0x8009beac against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009beac
 */

#include "common/types.h"

extern void (*D_80055c48)(uint32_t);

void CARDGAME_F0x8009beac(uint8_t *ptr, int32_t idx, uint16_t arg2, uint32_t arg3, uint16_t arg4, uint16_t arg5) {
    D_80055c48(0x40019);
    ptr += (idx * 3) << 3;
    *(uint16_t *)(ptr + 0xe0c) = arg4;
    *(uint16_t *)(ptr + 0xe0e) = arg5;
    *(uint16_t *)(ptr + 0xe12) = 0x1000;
    *(uint8_t *)(ptr + 0xe23) = 1;
    *(uint16_t *)(ptr + 0xe10) = 0;
    *(uint16_t *)(ptr + 0xe18) = arg2;
    *(uint32_t *)(ptr + 0xe1c) = arg3;
    *(uint16_t *)(ptr + 0xe16) = 12;
    *(uint16_t *)(ptr + 0xe14) = 12;
}
