// CARDGAME:0x8009f04c (size 196, 0xC4; file off 0x1c39c; base 0x80082cb0)
// PAL: reference/extracted/pro/cardgame.bin; Ghidra ddw3-pal-sles-03936/CARDGAME
// Framed function from reports/handoffs/cardgame-boundary-sweep.md
// (0x8009eefc/336 precedes, 0x8009f110 follows; jr ra + addiu sp,0x30).
// Loop 40x: two indirect RNG calls through EXE table slot D_8004dc04[2]
// (lw 8(s4) per call), indices (rng() % a2) + a1, then an int16 swap at
// a0+0x59c+0x14. A card-deck shuffle.
// The table is referenced through its symbol, with no local base pointer:
// ASPSX then emits the lui 0x8005 / addiu s4,v0,-0x23fc split, and the loop
// optimizer places it after the counter reset as in PAL.
// PAL has no zero-divisor guards: build with --strip-div-guard.
#include "common/types.h"

typedef int32_t (*cardgame_rng_fn)(void);

/* Observed layout only: int16 elements behind a +0x59c base with a +0x14
   member offset (lh/lhu/sh +0x14(s0)/+0x14(v1)). Length unknown. */
typedef struct {
    uint8_t pad[0x14];
    int16_t e[1];
} cardgame_deck_t;

extern int32_t D_8004dc04[];

void CARDGAME_F0x8009f04c(uint8_t *a0, int32_t a1, int32_t a2)
{
    uint8_t *p;
    int32_t i;
    int32_t j;
    int32_t k;
    int32_t t1;
    uint32_t t2;

    p = a0 + 0x59c;
    if (a2 < 2) {
        return;
    }
    for (i = 0; i < 40; i++) {
        j = ((cardgame_rng_fn)D_8004dc04[2])() % a2 + a1;
        k = ((cardgame_rng_fn)D_8004dc04[2])() % a2 + a1;
        t1 = ((cardgame_deck_t *)p)->e[j];
        t2 = (uint16_t)((cardgame_deck_t *)p)->e[k];
        ((cardgame_deck_t *)p)->e[j] = (int16_t)t2;
        ((cardgame_deck_t *)p)->e[k] = (int16_t)t1;
    }
}
