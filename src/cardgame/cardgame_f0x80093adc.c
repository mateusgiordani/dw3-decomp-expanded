/*
 * CARDGAME:0x80093adc CARDGAME_F0x80093adc
 * 52 bytes at CARDGAME.PRO offset 0x10e2c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80093adc
 *  Symbols     (none)
 *  Compare     52 bytes from 0x80093adc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80093adc
 */

#include <stdint.h>

void CARDGAME_F0x80093adc(uint8_t *state, uint32_t context, int32_t side)
{
    int32_t offset;
    int32_t value;

    offset = side * 0x72;
    *(int32_t *)(state + 0x424) = 0;
    value = *(uint8_t *)(state + offset + 0x72c);
    *(uint8_t *)(state + 0x422) = 1;
    *(int32_t *)(state + 0x428) = value - 1;
}
