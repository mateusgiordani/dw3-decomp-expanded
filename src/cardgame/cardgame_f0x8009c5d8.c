/*
 * CARDGAME:0x8009c5d8 CARDGAME_F0x8009c5d8
 * 124 bytes at CARDGAME.PRO offset 0x19928 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009c5d8
 *  Symbols     D_80055c48=0x80055c48
 *  Compare     124 bytes from 0x8009c5d8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009c5d8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * full PAL range 124 bytes, base 0x80082cb0.
 */

#include "common/types.h"

extern int32_t (*D_80055c48)(uint32_t);

/* Access view at base + index*76, not an array element type: sizeof this view
 * is not the stride. Only the PAL byte/halfword/word offsets below are known. */
typedef struct {
    uint8_t unknown[0x120];
    uint16_t f120, f122;
    uint8_t unknown124[10];
    uint16_t f12e;
    uint32_t f130, f134;
    uint8_t unknown138[18];
    uint8_t f14a;
} CardInitView9c5d8;

int32_t CARDGAME_F0x8009c5d8(uint8_t *base, int32_t index)
{
    CardInitView9c5d8 *row;
    D_80055c48(0x8004613e);
    row = (CardInitView9c5d8 *)(base + index * 76);
    row->f14a = 4;
    row->f130 = 0;
    row->f134 = 0;
    row->f120 = 0x1000;
    row->f122 = 0x1000;
    row->f12e = 0;
    return 0;
}
