/*
 * STCRDSHP:0x8008300c STCRDSHP_func_8008300c
 * 700 bytes at STCRDSHP.PRO offset 0x35c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008300c
 *  Symbols     F0x8001ae38=0x8001ae38 F0x8001b364=0x8001b364
 *              STCRDSHP_func_8008300c=0x8008300c
 *  Compare     700 bytes from 0x8008300c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x8008300c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Parent reconstruction restores every PAL output slot, reloads the signed
 * halfword ID before calls, and reads the +0x58 word.
 */

#include "common/types.h"
extern int32_t F0x8001ae38(int32_t, int32_t, int32_t, int32_t);
extern int32_t F0x8001b364(int32_t, int32_t, int32_t, int32_t);
void STCRDSHP_func_8008300c(uint8_t *object, uint32_t *out)
{
    int32_t i;
    for (i = 0; i < 8; i++) {
        out[i] = F0x8001ae38(*(int16_t *)(object + 0x54), 1,
            (int16_t)((i % 2) * 0x83 + 0x37), (int16_t)((i / 2) * 0x0e + 0x39));
    }
    out[8] = F0x8001b364(*(int16_t *)(object + 0x54),
        *(int32_t *)(object + 0x58) - 1, 0x1d, 0x39);
    (*(void (**)(uint32_t, int32_t))(out[8] + 0x80))(out[8], 0);
    out[0x10] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x92, 0x75);
    out[0x11] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x93, 0x75);
    out[0x12] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0xa6, 0x75);
    out[0xe] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x2d, 0x71);
    out[0xf] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x102, 0x71);
    out[0x9] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x8f, 0x8a);
    out[0xa] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x10b, 0x8a);
    out[0xb] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x121, 0x8a);
    out[0xc] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x14, 0xc2);
    out[0xd] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x14, 0xd0);
    out[0x13] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x88, 0x80);
    out[0x14] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x115, 0x80);
    out[0x15] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x126, 0x80);
    out[0x16] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x115, 0xa6);
    out[0x17] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x12c, 0xa6);
    out[0x18] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0x50, 0x97);
    out[0x19] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0xce, 0x97);
    out[0x1a] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0xf0, 0x97);
    out[0x1b] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0xce, 0xa4);
    out[0x1c] = F0x8001ae38(*(int16_t *)(object + 0x54), 1, 0xf0, 0xa4);
}
