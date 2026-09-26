/*
 * CARDGAME:0x800a1af8 CARDGAME_F0x800a1af8
 * 52 bytes at CARDGAME.PRO offset 0x1ee48 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a1af8
 *  Symbols     D_800A5D5C=0x800a5d5c
 *  Compare     52 bytes from 0x800a1af8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a1af8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Sets state p1[0x2f4] = 1 and loads the step p1[0x2f6]'s entry of the 4-byte
 * table at 0x800a5d5c: byte 0 into p1[0x421] and byte 2 into p1[0x2f7].
 *
 * Matching note: the table base is held in a local pointer and the entry in
 * another; indexing the extern directly gives PAL's code with base and index
 * registers swapped.
 */

#include <stdint.h>

typedef struct {
    uint8_t b0;
    uint8_t b1;
    uint8_t b2;
    uint8_t b3;
} StepEntry;

extern StepEntry D_800A5D5C[];

#define U8(p, o)     (*(uint8_t *)((p) + (o)))

void CARDGAME_F0x800a1af8(int32_t p1)
{
    StepEntry *e;
    StepEntry *t;

    t = D_800A5D5C;
    U8(p1, 0x2f4) = 1;
    e = &t[U8(p1, 0x2f6)];
    U8(p1, 0x421) = e->b0;
    U8(p1, 0x2f7) = e->b2;
}
