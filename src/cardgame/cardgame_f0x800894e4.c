/*
 * CARDGAME:0x800894e4 CARDGAME_F0x800894e4
 * 156 bytes at CARDGAME.PRO offset 0x6834 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800894e4
 *  Symbols     D_800A58C4=0x800a58c4
 *  Compare     156 bytes from 0x800894e4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800894e4
 */

#include "common/types.h"

/*
 * Framed -0x30/+0x30; next framed function CARDGAME:0x80089580 at +0x9c.
 *
 * 7 direct jal callers at 0x80084a70..0x80084b00 (a2 = 0..6, dispatch fan-in);
 * 0 direct callees (2 indirect jalr via slots 0xEE4/0xEAC on second arg).
 *
 * D_800A58C4 is a 2-byte-entry table indexed by idx (callers pass 0..6).
 */

typedef void (*CardFn5)(void *a0, uint32_t a1, int32_t a2, int32_t a3, int32_t a4);
typedef void (*CardFn6)(void *a0, int32_t a1, int32_t a2, uint32_t a3, int32_t a4, int32_t a5);

extern uint8_t D_800A58C4[];

void CARDGAME_F0x800894e4(uint8_t *ctx, uint8_t *s, int32_t idx) {
    uint8_t *p = &D_800A58C4[idx * 2];
    int32_t one = 1;

    ((CardFn5)*(uint32_t *)(s + 0xEE4))(s, p[1], 0, 0, one);
    ((CardFn6)*(uint32_t *)(s + 0xEAC))(s, 5, 5, p[0], 0, 66);
    ctx[0x422] = (uint8_t)one;
}
