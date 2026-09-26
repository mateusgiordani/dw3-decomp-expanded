/*
 * CARDGAME:0x800a44f0 CARDGAME_F0x800a44f0
 * 264 bytes at CARDGAME.PRO offset 0x21840 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a44f0, jump table (.rodata) at 0x80083d2c
 *  Symbols     CARDGAME_F0x800a3d90=0x800a3d90 CARDGAME_F0x800a3e9c=0x800a3e9c
 *              CARDGAME_F0x800a41f8=0x800a41f8 CARDGAME_F0x800a4328=0x800a4328
 *  Compare     264 bytes from 0x800a44f0 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a44f0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x20 (s0-s1, ra).
 *
 * Per-event card-choice step keyed by `event` (3..59, jump table 0x80083d2c):
 * runs the matching pickers on side 0 (or 1 for event 40) and returns 1 when a
 * choice was made; unknown events return 1.
 *
 * Matching note: the slot offset `d` is a named local so p1 + d adds base-first
 * as in PAL.
 */

#include <stdint.h>

int32_t CARDGAME_F0x800a4328(int32_t p1);
void CARDGAME_F0x800a3d90(int32_t p1, int32_t side);
int32_t CARDGAME_F0x800a3e9c(int32_t p1, int32_t side);
int32_t CARDGAME_F0x800a41f8(int32_t p1, int32_t side, int32_t value, int32_t keep);

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S8(p, o)     (*(int8_t *)((p) + (o)))

int32_t CARDGAME_F0x800a44f0(int32_t p1, int32_t p2, int32_t event)
{
    int32_t ret;

    ret = 0;
    switch (event) {
    case 39:
        if (CARDGAME_F0x800a4328(p1) != 0)
            ret = 1;
        break;
    case 7:
    case 9:
    case 26:
    case 55:
    case 57:
        CARDGAME_F0x800a3d90(p1, 0);
        if (CARDGAME_F0x800a3e9c(p1, 0) != 0)
            ret = 1;
        break;
    case 21:
    case 46:
        CARDGAME_F0x800a41f8(p1, 0, 30, 0);
        CARDGAME_F0x800a3d90(p1, 0);
        if (CARDGAME_F0x800a3e9c(p1, 0) != 0)
            ret = 1;
        break;
    case 56:
        CARDGAME_F0x800a41f8(p1, 0, 10, 0);
        CARDGAME_F0x800a3d90(p1, 0);
        if (CARDGAME_F0x800a3e9c(p1, 0) != 0)
            ret = 1;
        break;
    case 40:
        if (CARDGAME_F0x800a3e9c(p1, 1) != 0)
            ret = 1;
        break;
    case 3:
    case 15:
    case 58:
    case 59:
        if (U8(p1, 0x79e) != 0) {
            int32_t d;

            ret = 1;
            d = S8(p1, 0x575) * 8;
            U8(p1 + d, 0x586) = U8(p1, 0x7ac);
        }
        break;
    default:
        ret = 1;
        break;
    }
    return ret;
}
