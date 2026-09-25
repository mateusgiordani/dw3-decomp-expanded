/*
 * CARDGAME:0x8009bac8 CARDGAME_F0x8009bac8
 * 224 bytes at CARDGAME.PRO offset 0x18e18 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x8009bac8
 *  Symbols     CARDGAME_F0x8009b76c=0x8009b76c CARDGAME_F0x8009b890=0x8009b890
 *              CARDGAME_F0x8009ba3c=0x8009ba3c CARDGAME_F0x8009bac8=0x8009bac8
 *  Compare     224 bytes from 0x8009bac8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009bac8
 */

#include <stdint.h>

extern void CARDGAME_F0x8009b890(int32_t a0, int32_t a1, int32_t a2);
extern void CARDGAME_F0x8009b76c(int32_t a0, int32_t a1);
extern void CARDGAME_F0x8009ba3c(int32_t a0, int32_t a1);

void CARDGAME_F0x8009bac8(int32_t ctx, int32_t arg)
{
    int32_t off;
    int32_t i;
    int32_t pass;
    int32_t row;

    for (i = 0x27, off = 0xc9c; i >= 0; i--, off -= 0x4c) {
        CARDGAME_F0x8009b890(ctx, arg, ctx + off);
        CARDGAME_F0x8009b76c(ctx, ctx + off);
    }
    for (pass = 0; pass < 2; pass++) {
        for (i = 0x27, off = 0xc9c, row = ctx + 0xb94; ; row -= 0x4c) {
            if ((pass == 0) ? (*(int16_t *)(row + 0x12e) != 0) : (*(int16_t *)(row + 0x12e) == 0)) {
                CARDGAME_F0x8009ba3c(ctx, ctx + off);
                off -= 0x4c;
                i--;
                if (i < 0) break;
            } else {
                off -= 0x4c;
                i--;
                if (i < 0) break;
            }
        }
    }
}
