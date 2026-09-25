/*
 * CARDGAME:0x80088f0c CARDGAME_F0x80088f0c
 * 108 bytes at CARDGAME.PRO offset 0x625c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80088f0c
 *  Symbols     CARDGAME_F0x80084320=0x80084320
 *  Compare     108 bytes from 0x80088f0c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80088f0c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Contiguity: prev CARDGAME:0x80087edc (0x1030) ends at 0x80088f0c; next
 * CARDGAME:0x80088f78 at +0x6c (27bdffd8).
 *
 * (**(code **)(param_2 + 0xec8))(param_2); bytes 0x49d/0x49c/0x499 = 1.
 *
 * Xref: 1 caller CARDGAME_F0x80084320 via 0x80084a24 UNCONDITIONAL_CALL; 0
 * direct callees (2 indirect jalr via s1+0xecc/0xec8).
 */

#include <stdint.h>

typedef void (*cardgame_cb_t)(int32_t);

void CARDGAME_F0x80088f0c(int32_t p1, int32_t p2)
{
    ((cardgame_cb_t)*(uint32_t *)(p2 + 0xecc))(p2);
    *(uint32_t *)(p1 + 0x440) = 0;
    *(uint8_t *)(p1 + 0x422) = 1;
    ((cardgame_cb_t)*(uint32_t *)(p2 + 0xec8))(p2);
    *(uint8_t *)(p1 + 0x49d) = 1;
    *(uint8_t *)(p1 + 0x49c) = 1;
    *(uint8_t *)(p1 + 0x499) = 1;
}
