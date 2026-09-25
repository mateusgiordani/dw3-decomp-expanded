/*
 * CARDGAME:0x80099d18 CARDGAME_F0x80099d18
 * 208 bytes at CARDGAME.PRO offset 0x17068 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80099d18
 *  Symbols     DAT_8004df9c=0x8004df9c
 *  Compare     208 bytes from 0x80099d18 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80099d18
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL-SLES-03936.
 *
 * The callback load at 0x80099d18/1c precedes the prologue at 0x80099d20.
 *
 * The predecessor returns at 0x80099d10 (delay 0x80099d14); the direct caller
 * at 0x80099f1c targets 0x80099d18. The former 200-byte window is internal.
 *
 * Source filename retained for revision provenance; definition uses true entry.
 *
 * This is a measured source shape, not a general CSE rule.
 */

#include "common/types.h"

typedef uint8_t (*cardgame_tick_fn)(void);
extern cardgame_tick_fn DAT_8004df9c;

void CARDGAME_F0x80099d18(uint8_t *work)
{
    /* PAL control flow branches (beq) to the snap path and falls through
       into the interpolation, i.e. the source tests nonzero first. */
    if ((work[0xe] -= DAT_8004df9c()) != 0) {
        int qx;
        int qy;

        qx = work[0xf];
        qx = (*(int16_t *)(work + 8) - *(int16_t *)(work + 4)) * work[0xe] / qx;
        qy = work[0xf];
        qy = (*(int16_t *)(work + 10) - *(int16_t *)(work + 6)) * work[0xe] / qy;
        *(int16_t *)(work + 0) = *(uint16_t *)(work + 8) - qx;
        *(int16_t *)(work + 2) = *(uint16_t *)(work + 10) - qy;
    } else {
        work[0x10] = 1;
        work[0xf] = 0;
        work[0xe] = 0;
        *(int16_t *)(work + 0) = *(uint16_t *)(work + 8);
        *(int16_t *)(work + 2) = *(uint16_t *)(work + 10);
    }
}
