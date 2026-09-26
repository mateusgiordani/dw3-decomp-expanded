/*
 * CARDGAME:0x800a1db8 CARDGAME_F0x800a1db8
 * 168 bytes at CARDGAME.PRO offset 0x1f108 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a1db8
 *  Symbols     (none)
 *  Compare     168 bytes from 0x800a1db8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a1db8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x20 (s0-s2, ra).
 *
 * On the first call (counter p1[0x4e8] == 0) sets the flags p1[0x49d] and
 * p1[0x499], calls the method at +0xec8 of the object p2[0x18] and bumps the
 * counter. Returns 1 when that object's state (+0x64) is 2 and p1[0x498] is
 * clear.
 */

#include <stdint.h>

typedef void (*method_t)(int32_t);

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

int32_t CARDGAME_F0x800a1db8(int32_t p1, int32_t p2)
{
    int32_t ret;

    ret = 0;
    if (S32(p1, 0x4e8) == 0) {
        U8(p1, 0x49d) = 1;
        U8(p1, 0x499) = 1;
        ((method_t)S32(S32(p2, 0x18), 0xec8))(S32(p2, 0x18));
        S32(p1, 0x4e8)++;
    }
    if (S16(S32(p2, 0x18), 0x64) == 2 && U8(p1, 0x498) == 0)
        ret = 1;
    return ret;
}
