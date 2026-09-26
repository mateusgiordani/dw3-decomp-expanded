/*
 * CARDGAME:0x8009c898 CARDGAME_F0x8009c898
 * 108 bytes at CARDGAME.PRO offset 0x19be8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009c898
 *  Symbols     (none)
 *  Compare     108 bytes from 0x8009c898 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009c898
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Starts a move for record `idx` (0x4c-byte records from p1+0x108): the target
 * (p1 + idx*0x4c + 0x124/0x126) becomes (x, y) and the start (+0x128/+0x12a)
 * the current position (+0x120/+0x122). Unless mode is 1 it also sets both
 * timers (+0x130/+0x134) to `time`, state +0x14a = 2, +0x12e = 1 and copies the
 * pair at +0x108 to +0x110. Returns 0.
 *
 * Matching note: the record is accessed through a struct, so its stores are
 * MEM_IN_STRUCT_P and do not alias the scalar stack argument `mode`; its load
 * then sinks below them as in PAL.
 */

#include <stdint.h>

typedef struct {
    uint8_t pad0[0x108];
    int32_t a0;
    int32_t a1;
    int32_t b0;
    int32_t b1;
    uint8_t pad10[8];
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
    uint8_t pad30[0x12];
    uint8_t state;
    uint8_t pad43[9];
} MoveRec;

int32_t CARDGAME_F0x8009c898(int32_t p1, int32_t idx, int32_t time, int32_t x, int32_t y, int32_t mode)
{
    MoveRec *r;

    r = (MoveRec *)(p1 + idx * 0x4c);
    r->dst_x = x;
    r->dst_y = y;
    r->src_x = r->cur_x;
    r->src_y = r->cur_y;
    if (mode != 1) {
        r->timer1 = time;
        r->timer0 = time;
        r->state = 2;
        r->active = 1;
        r->b0 = r->a0;
        r->b1 = r->a1;
    }
    return 0;
}
