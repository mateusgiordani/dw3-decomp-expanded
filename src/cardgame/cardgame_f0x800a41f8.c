/*
 * CARDGAME:0x800a41f8 CARDGAME_F0x800a41f8
 * 304 bytes at CARDGAME.PRO offset 0x21548 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a41f8
 *  Symbols     (none)
 *  Compare     304 bytes from 0x800a41f8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a41f8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * For one side (count p1+side*0x72+0x72c, 14-byte records), clears the flag
 * (p1+0x446, or p1+0x44c for side 1) of every flagged card whose value at
 * +0x736 is above `value`, remembering the last flagged index. If no flag is
 * left, returns 1 and, unless `keep` is set, flags that last card again.
 *
 * Matching notes: the first loop assigns the side offset `o` in its condition;
 * the second sets it in its init so the count load is hoisted base-first; ret =
 * 1 follows the re-flag so dbr moves it into the keep test's slot.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

int32_t CARDGAME_F0x800a41f8(int32_t p1, int32_t side, int32_t value, int32_t keep)
{
    int8_t *flags;
    int32_t ret;
    int32_t last;
    int32_t found;
    int32_t i;
    int32_t o;

    ret = 0;
    last = 0;
    if (side == 1)
        flags = (int8_t *)(p1 + 0x44c);
    else
        flags = (int8_t *)(p1 + 0x446);
    for (i = 0; o = side * 0x72, i < U8(p1 + o, 0x72c); i++) {
        if (flags[i] != 0) {
            last = i;
            if (value < S16(p1 + (side * 0x72 + i * 0xe), 0x736))
                flags[i] = 0;
        }
    }
    found = 0;
    for (i = 0, o = side * 0x72; i < U8(p1 + o, 0x72c); i++) {
        if (flags[i] != 0) {
            found = 1;
            break;
        }
    }
    if (!found) {
        if (keep == 0)
            flags[last] = 1;
        ret = 1;
    }
    return ret;
}
