/*
 * CARDGAME:0x8009c960 CARDGAME_F0x8009c960
 * 64 bytes at CARDGAME.PRO offset 0x19cb0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009c960
 *  Symbols     (none)
 *  Compare     64 bytes from 0x8009c960 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009c960
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Sets the move endpoints of record `idx` (p1 + idx * 0x4c): the pair at
 * +0x110/+0x114 becomes (x, y), the pair at +0x108 is saved to +0x118, and both
 * timers (+0x130/+0x134) are set to `time`.
 */

#include <stdint.h>

typedef struct {
    uint8_t pad0[0x108];
    int32_t a0;
    int32_t a1;
    int32_t b0;
    int32_t b1;
    int32_t c0;
    int32_t c1;
    int16_t cur_x;
    int16_t cur_y;
    int16_t dst_x;
    int16_t dst_y;
    int16_t src_x;
    int16_t src_y;
    int16_t pad24;
    int16_t active;
    int32_t timer0;
    int32_t timer1;
} MoveRec;

void CARDGAME_F0x8009c960(int32_t p1, int32_t idx, int32_t time, int32_t x, int32_t y)
{
    MoveRec *r;

    r = (MoveRec *)(p1 + idx * 0x4c);
    r->b0 = x;
    r->timer1 = time;
    r->timer0 = time;
    r->b1 = y;
    r->c0 = r->a0;
    r->c1 = r->a1;
}
