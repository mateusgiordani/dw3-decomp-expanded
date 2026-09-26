/*
 * CARDGAME:0x80093624 CARDGAME_F0x80093624
 * 236 bytes at CARDGAME.PRO offset 0x10974 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80093624
 *  Symbols     (none)
 *  Compare     236 bytes from 0x80093624 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80093624
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Starts a transfer from `side` to the other side's 0xc8-byte record: clears
 * p1[0x424]/p1[0x428], stores the amount (side record +0x59c) and the other
 * side's old value << 8, subtracts the amount from the other side's +0x59e
 * (clamped at 0), and sets the per-step size p1[0x434] from the difference over
 * (count * 28 - 16) frames, at least 1. Sets p1[0x422].
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

void CARDGAME_F0x80093624(int32_t p1, int32_t p2, int32_t side)
{
    int32_t me;
    int32_t other;
    int32_t cnt;

    me = p1 + side * 0xc8;
    S32(p1, 0x424) = 0;
    S32(p1, 0x428) = 0;
    S32(p1, 0x42c) = S16(me, 0x59c);
    other = p1 + (side ^ 1) * 0xc8;
    S32(p1, 0x430) = S16(other, 0x59e) << 8;
    S16(other, 0x59e) -= S16(me, 0x59c);
    if (S16(other, 0x59e) < 0)
        S16(other, 0x59e) = 0;
    cnt = p1 + side * 0x72;
    if (U8(cnt, 0x72c) != 0) {
        S32(p1, 0x434) = (S32(p1, 0x430) - (S16(other, 0x59e) << 8)) / (U8(cnt, 0x72c) * 28 - 16);
        if (S32(p1, 0x434) == 0)
            S32(p1, 0x434) = 1;
    } else {
        S32(p1, 0x434) = 1;
    }
    U8(p1, 0x422) = 1;
}
