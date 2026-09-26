/*
 * CARDGAME:0x800a3488 CARDGAME_F0x800a3488
 * 192 bytes at CARDGAME.PRO offset 0x207d8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a3488
 *  Symbols     D_8004df9c=0x8004df9c
 *  Compare     192 bytes from 0x800a3488 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a3488
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x18 (s0, ra).
 *
 * Steps a timed colour fade: subtracts the elapsed ticks (callback at
 * 0x8004df9c) from the timer p1[0x54]; while it is positive, each of the three
 * channels at +0x64 is start (+0x5e) minus (start - target (+0x61)) * timer /
 * duration (+0x58). When it runs out, state p1[0x50] becomes 2 if p1[0x5d] is
 * set, else 0 with the start colour restored.
 */

#include <stdint.h>

typedef int32_t (*CardTick)(void);
extern CardTick D_8004df9c;

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

void CARDGAME_F0x800a3488(int32_t p1)
{
    int32_t i;

    S32(p1, 0x54) -= D_8004df9c();
    if (S32(p1, 0x54) > 0) {
        for (i = 0; i < 3; i++) {
            U8(p1 + i, 0x64) = U8(p1 + i, 0x5e)
                - (U8(p1 + i, 0x5e) - U8(p1 + i, 0x61)) * S32(p1, 0x54) / S32(p1, 0x58);
        }
    } else if (U8(p1, 0x5d) != 0) {
        S32(p1, 0x50) = 2;
    } else {
        S32(p1, 0x50) = 0;
        U8(p1, 0x64) = U8(p1, 0x5e);
        U8(p1, 0x65) = U8(p1, 0x5f);
        U8(p1, 0x66) = U8(p1, 0x60);
    }
}
