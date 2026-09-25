/*
 * CARDGAME:0x80097a24 CARDGAME_F0x80097a24
 * 168 bytes at CARDGAME.PRO offset 0x14d74 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80097a24
 *  Symbols     D_80044f5c=0x80044f5c F0x8001f648=0x8001f648
 *  Compare     168 bytes from 0x80097a24 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80097a24
 */

#include "common/types.h"

typedef void (*PairCall)(int32_t, int32_t);
typedef void (*TripleCall)(int32_t, int32_t, int32_t);
typedef void (*QuadCall)(int32_t, int32_t, int32_t, int32_t);
extern int32_t (*D_80044f5c)(int32_t);
extern void F0x8001f648(uint8_t *);

void CARDGAME_F0x80097a24(int32_t a0, int32_t a1, int32_t a2, int32_t a3)
{
    uint8_t buf[0xa0];

    F0x8001f648(buf);
    (*(PairCall *)(buf + 0x7c))(0x100, 1);
    (*(PairCall *)(buf + 0x74))(0x280, 0);
    (*(TripleCall *)(buf + 0x8c))(0x1000, 0x1000, 0x1000);
    (*(QuadCall *)(buf + 0x84))(D_80044f5c(0x025d0002), 0x18, a2, a3);
}
