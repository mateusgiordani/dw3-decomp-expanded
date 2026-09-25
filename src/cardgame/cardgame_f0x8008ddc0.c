/*
 * CARDGAME:0x8008ddc0 CARDGAME_F0x8008ddc0
 * 236 bytes at CARDGAME.PRO offset 0xb110 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008ddc0
 *  Symbols     D_80055c48=0x80055c48
 *  Compare     236 bytes from 0x8008ddc0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008ddc0
 */

#include "common/types.h"

/*
 * portable C recovery from PAL bytes.
 *
 * Boundary sweep #58: prologue 27bdffd8 addiu sp,-0x28, epilogue jr ra +
 * 27bd0028 addiu sp,+0x28; prev CARDGAME:0x8008dd38 size 0x88 ends exactly at
 * 0x8008ddc0, next CARDGAME:0x8008deb4 at +0xEC (+8 caller-setup gap).
 *
 * x-ref to: 2 jal sites in CARDGAME_F0x80084320 (side 0 and side 1).
 *
 * Revision 7 (GCC 2.8.1 SN32 / ASPSX 2.79 O2, exact): the vector call is a
 * switch whose default shares the case 0 body, written first, so the tests fall
 * into the 0x9c0001 call and case 1 jumps to the 0x9c0000 call; and the vector
 * is referenced through the symbol D_80055c48 so its lui can occupy the case 1
 * branch delay slot (--symbol D_80055c48=0x80055c48).
 */
typedef void (*CardSlotFn_ddc0)(void *p2, int32_t idx, int32_t side);
typedef void (*ExeVecFn_ddc0)(uint32_t arg);

extern ExeVecFn_ddc0 D_80055c48;

void CARDGAME_F0x8008ddc0(void *ctx, void *p2, int32_t side)
{
    int32_t hit = 0;
    int32_t i;

    for (i = 0; i < 12; i++) {
        if (*(int8_t *)((char *)ctx + i + 0x46f) != 0) {
            if (i < 6) {
                if (i < *(uint8_t *)((char *)ctx + 0x72c)) {
                    (*(CardSlotFn_ddc0 *)((char *)p2 + 0xf38))(p2, i, side);
                    hit = 1;
                }
            } else {
                if ((i - 6) < *(uint8_t *)((char *)ctx + 0x79e)) {
                    (*(CardSlotFn_ddc0 *)((char *)p2 + 0xf38))(p2, i, side);
                    hit = 1;
                }
            }
        }
    }
    if (hit != 0) {
        switch (side) {
        case 0:
        default:
            D_80055c48(0x9c0001);
            break;
        case 1:
            D_80055c48(0x9c0000);
            break;
        }
    }
    *(int32_t *)((char *)ctx + 0x424) = 0;
}
