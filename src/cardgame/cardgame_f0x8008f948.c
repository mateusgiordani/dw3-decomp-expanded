/*
 * CARDGAME:0x8008f948 CARDGAME_F0x8008f948
 * 48 bytes at CARDGAME.PRO offset 0xcc98 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008f948
 *  Symbols     (none)
 *  Compare     48 bytes from 0x8008f948 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008f948
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Counts a step in p1[0x305] and sets p1[0x304] to one more than the previous
 * slot's value (int16 at +0x580 of p1 + (p1[0x575] - 1) * 8). Returns 1.
 *
 * Matching note: the slot value is read into an int local first; loading it
 * straight into the byte store lets GCC narrow the lh to lbu.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S8(p, o)     (*(int8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

int32_t CARDGAME_F0x8008f948(int32_t p1)
{
    int32_t d;
    int32_t v;

    d = (S8(p1, 0x575) - 1) * 8;
    v = S16(p1 + d, 0x580);
    U8(p1, 0x304) = v + 1;
    U8(p1, 0x305)++;
    return 1;
}
