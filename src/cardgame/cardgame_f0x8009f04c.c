/*
 * CARDGAME:0x8009f04c CARDGAME_F0x8009f04c
 * 196 bytes at CARDGAME.PRO offset 0x1c39c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009f04c
 *  Symbols     D_8004dc04=0x8004dc04
 *  Compare     196 bytes from 0x8009f04c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009f04c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Loop 40x: two indirect RNG calls through EXE table slot D_8004dc04[2] (lw
 * 8(s4) per call), indices (rng() % a2) + a1, then an int16 swap at
 * a0+0x59c+0x14. A card-deck shuffle.
 *
 * The table is referenced through its symbol, with no local base pointer:
 *
 * ASPSX then emits the lui 0x8005 / addiu s4,v0,-0x23fc split, and the loop
 * optimizer places it after the counter reset as in PAL.
 */

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
