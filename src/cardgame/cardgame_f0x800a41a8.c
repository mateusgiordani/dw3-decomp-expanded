/*
 * CARDGAME:0x800a41a8 CARDGAME_F0x800a41a8
 * 80 bytes at CARDGAME.PRO offset 0x214f8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a41a8
 *  Symbols     (none)
 *  Compare     80 bytes from 0x800a41a8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a41a8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Returns 1 when card `i` is flagged (p1+0x446) and its value at +0x736 of the
 * side's 14-byte entry (p1 + side * 0x72 + i * 0xe) is at most `value`.
 *
 * Matching note: the flag address is written i + p1 and the test as value >=
 * entry, which give PAL's operand order and registers.
 */

#include <stdint.h>

#define S8(p, o)     (*(int8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

int32_t CARDGAME_F0x800a41a8(int32_t p1, int32_t side, int32_t i, int32_t value)
{
    int32_t ret;

    ret = 0;
    if (S8(i + p1, 0x446) != 0)
        ret = value >= S16(p1 + (side * 0x72 + i * 0xe), 0x736);
    return ret;
}
