/*
 * CARDGAME:0x80083e34 CARDGAME_F0x80083e34
 * 276 bytes at CARDGAME.PRO offset 0x1184 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80083e34, jump table (.rodata) at 0x80082cb0
 *  Symbols     D_800A4DF4=0x800a4df4
 *  Compare     276 bytes from 0x80083e34 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80083e34
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Card record accessor: returns byte `kind` (0..3) of the 0x2c-byte record `id`
 * in the table at 0x800a4df4, or byte `index` of its 40-byte data block for
 * kind 4 (5-entry jump table at 0x80082cb0); 0 for other kinds.
 */

#include <stdint.h>

struct CardRec {
    uint8_t b0;
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;
    uint8_t data[40];
};

extern struct CardRec D_800A4DF4[];

uint8_t CARDGAME_F0x80083e34(int32_t id, int32_t kind, int32_t index)
{
    uint8_t ret;

    ret = 0;
    switch (kind) {
    case 0:
        ret = D_800A4DF4[id].b0;
        break;
    case 1:
        ret = D_800A4DF4[id].b1;
        break;
    case 2:
        ret = D_800A4DF4[id].b2;
        break;
    case 3:
        ret = D_800A4DF4[id].b3;
        break;
    case 4:
        ret = D_800A4DF4[id].data[index];
        break;
    }
    return ret;
}
