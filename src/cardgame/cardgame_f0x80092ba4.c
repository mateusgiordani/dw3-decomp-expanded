/*
 * CARDGAME:0x80092ba4 CARDGAME_F0x80092ba4
 * 176 bytes at CARDGAME.PRO offset 0xfef4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80092ba4
 *  Symbols     CARDGAME_F0x80092ba4=0x80092ba4
 *  Compare     176 bytes from 0x80092ba4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80092ba4
 */

#include "common/types.h"

/*
 * Prologue 27bdffe0 addiu sp,-0x20 ; sw s1/s0/ra ; move s1,a0 ; move s0,a1
 *
 * Body: *(p1+0x5a6)=0, *(p1+0x66e)=0, then 4x indirect jalr via *(p2+0xea0):
 * (p2,0,6,lh(p1+0x5a6)), (p2,1,6,lh(p1+0x66e)), (p2,0,5,lh(p1+0x5a4)),
 * (p2,1,5,lh(p1+0x66c)); then *(p1+0x440)=0 (word), *(p1+0x423)=1 (byte),
 * return 1.
 *
 * Epilogue lw s0/s1/ra + jr ra + addiu sp,+0x20 at 0x80092c40/0x80092c50; next
 * CARDGAME:0x80092c54 prologue confirms size 0xb0 contiguous.
 */

typedef void (*CardEa0Fn)(int32_t, int32_t, int32_t, int32_t);

int32_t CARDGAME_F0x80092ba4(int32_t p1, int32_t p2)
{
    int32_t result;

    *(int16_t *)(p1 + 0x5a6) = 0;
    *(int16_t *)(p1 + 0x66e) = 0;
    ((CardEa0Fn)*(void **)(p2 + 0xea0))(p2, 0, 6, *(int16_t *)(p1 + 0x5a6));
    ((CardEa0Fn)*(void **)(p2 + 0xea0))(p2, 1, 6, *(int16_t *)(p1 + 0x66e));
    ((CardEa0Fn)*(void **)(p2 + 0xea0))(p2, 0, 5, *(int16_t *)(p1 + 0x5a4));
    ((CardEa0Fn)*(void **)(p2 + 0xea0))(p2, 1, 5, *(int16_t *)(p1 + 0x66c));
    result = 1;
    *(int32_t *)(p1 + 0x440) = 0;
    *(uint8_t *)(p1 + 0x423) = (uint8_t)result;
    return result;
}
