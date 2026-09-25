/*
 * CARDGAME:0x80087458 CARDGAME_F0x80087458
 * 340 bytes at CARDGAME.PRO offset 0x47a8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80087458
 *  Symbols     CARDGAME_F0x8008735c=0x8008735c
 *  Compare     340 bytes from 0x80087458 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80087458
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Original delivery remains preserved in worker commit
 * bcb142fb002ea2a72a9952f4d5f06adec1aae4b5.
 *
 * Distinct branch-local row variables reproduce the PAL row/value registers
 * with GCC 2.8.1 / ASPSX 2.79, O2 G0 (full 340-byte match; revision 8).
 */

#include "common/types.h"























































#include <stdint.h>

extern void CARDGAME_F0x8008735c(int32_t a0, uint8_t *a1, int32_t a2, int32_t a3, int32_t a4);

int32_t CARDGAME_F0x80087458(int32_t p1, uint8_t *p2, int32_t p3) {
    int32_t idx;
    int32_t r;
    int32_t h;

    idx = *(int32_t *)(p1 + 0x43c);
    r = 0;
    if (*(int8_t *)(p1 + idx + 0x446) != 0) {
        uint8_t *row;
        if (*(uint8_t *)(p1 + 0x444) >= 6)
            goto out;
                h = *(int16_t *)(p3 + (idx << 1) + 100);
        CARDGAME_F0x8008735c(p1, p2, p3, 0, h);
        *(int8_t *)(p1 + *(int32_t *)(p1 + 0x43c) + 0x46f) = 1;
        ((void (*)(uint8_t *, int32_t))*(void **)(p2 + 0xf1c))(p2, *(int32_t *)(p1 + 0x43c));
        row = p2 + *(int32_t *)(p1 + 0x43c) * 76;
        row[0x150] |= 2;
        r = 1;
        *(uint8_t *)(p1 + 0x444) = *(uint8_t *)(p1 + 0x444) + r;
    } else {
        uint8_t *row;
        uint8_t *q;
        if (*(int8_t *)(p1 + idx + 0x46f) == 0)
            goto out;
        q = p2;
                h = *(int16_t *)(p3 + (idx << 1) + 100);
        CARDGAME_F0x8008735c(p1, q, p3, 1, h);
        *(int8_t *)(p1 + *(int32_t *)(p1 + 0x43c) + 0x46f) = 0;
        row = q + *(int32_t *)(p1 + 0x43c) * 76;
        row[0x150] &= 0xfd;
        r = 2;
        *(uint8_t *)(p1 + 0x444) = *(uint8_t *)(p1 + 0x444) - 1;
    }
out:
    return r;
}