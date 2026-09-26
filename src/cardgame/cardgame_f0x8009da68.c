/*
 * CARDGAME:0x8009da68 CARDGAME_F0x8009da68
 * 384 bytes at CARDGAME.PRO offset 0x1adb8 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x8009da68
 *  Symbols     (none)
 *  Compare     384 bytes from 0x8009da68 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009da68
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Returns the hand to the deck and reorders it. The deck block at p1+0x664
 * holds the draw position (+4), a counter (+8), the hand size (+0xa), 40 card
 * ids (+0x14) and the hand ids (+0x64); per-position state bytes live in the
 * (index, state) pairs at p1+0x30a. Cards put back get state 2 * p1[0x300] + 1;
 * cards whose state is above 2 * p1[0x300] + 2 at the draw position are rotated
 * to the end of the first p1[0x41c] cards with state 7; finally the pair
 * indices are renumbered 39..0.
 *
 * Built with -fno-strength-reduce (variant o2-g0-no-strength-reduce): the
 * rotate loop recomputes i * 2 each pass, and the fill and renumber loops are
 * explicit pointer loops.
 *
 * Returns the hand to the deck and reorders it. The deck block at p1+0x664
 * holds the draw position (+4), a counter (+8), the hand size (+0xa), 40 card
 * ids (+0x14) and the hand ids (+0x64); per-position state bytes live in the
 * (index, state) pairs at p1+0x30a. Cards put back get state 2 * p1[0x300] + 1;
 * cards whose state is above 2 * p1[0x300] + 2 at the draw position are rotated
 * to the end of the first p1[0x41c] cards with state 7; finally the pair
 * indices are renumbered 0..39.
 */

#include <stdint.h>

struct Deck {
    int16_t pad0[2];
    int16_t top;
    int16_t pad6;
    int16_t moved;
    int16_t count;
    int16_t padc[4];
    int16_t cards[40];
    int16_t hand[40];
};

#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define U8(p, o)     (*(uint8_t *)((p) + (o)))

void CARDGAME_F0x8009da68(int32_t p1)
{
    struct Deck *d;
    int32_t old;
    int32_t i;
    int32_t k;
    int32_t n;
    int32_t m;
    uint8_t *p;
    uint8_t *q;
    int16_t t;

    d = (struct Deck *)(p1 + 0x664);
    old = d->top;
    while (d->count > 0) {
        d->cards[--d->top] = d->hand[--d->count];
        d->moved++;
    }
    if (old != d->top) {
        k = d->top;
        if (k < old) {
            p = (uint8_t *)(k * 2 + p1);
            do {
                k++;
                p[0x30b] = U8(p1, 0x300) * 2 + 1;
                p += 2;
            } while (k < old);
        }
    }
    for (k = 0; k < 40; k++) {
        i = d->top;
        {
            int32_t o = i * 2;

            if (U8(p1 + o, 0x30b) > U8(p1, 0x300) * 2 + 2)
                break;
            t = *(int16_t *)((uint8_t *)d + o + 0x14);
        }
        while (i < U8(p1, 0x41c) - 1) {
            n = i * 2;
            i++;
            m = i * 2;
            *(int16_t *)((uint8_t *)d + n + 0x14) = d->cards[i];
            U8(p1 + n, 0x30b) = U8(p1 + m, 0x30b);
        }
        d->cards[U8(p1, 0x41c) - 1] = t;
        {
            int32_t k2 = U8(p1, 0x41c) - 1;
            int32_t o = k2 * 2;

            U8(p1 + o, 0x30b) = 7;
        }
    }
    k = 39;
    q = (uint8_t *)(p1 + 0x4e);
    do {
        q[0x30a] = k;
        k--;
        q -= 2;
    } while (k >= 0);
}
