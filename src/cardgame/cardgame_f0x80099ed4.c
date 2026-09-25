/*
 * CARDGAME:0x80099ed4 CARDGAME_F0x80099ed4
 * 140 bytes at CARDGAME.PRO offset 0x17224 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80099ed4
 *  Symbols     CARDGAME_F0x80099d18=0x80099d18 CARDGAME_F0x80099de8=0x80099de8
 *  Compare     140 bytes from 0x80099ed4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80099ed4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL-SLES-03936.
 *
 * Exact with PsyQ GCC 2.8.1 SN32 4.0.0010 / ASPSX 2.79, O2 base.
 *
 * flagUnits counts nine-byte units: two steps advance one 18-byte slot.
 *
 * Keep the counter between those steps: loop discovers the IVs in PAL
 * initialization order, then combine folds the two pointer advances.
 */

#include "common/types.h"

extern void CARDGAME_F0x80099d18(void *a0);
extern void CARDGAME_F0x80099de8(void *a0);

void CARDGAME_F0x80099ed4(uint8_t *a0)
{
    unsigned int i;
    unsigned int flagUnits = 0;
    uint8_t v;

    for (i = 0; i < 12; flagUnits++, i++, flagUnits++) {
        v = (a0 + flagUnits * 9)[0xcf8];
        if (v != 0) {
            if (v == 2)
                CARDGAME_F0x80099d18(a0 + (i * 0x12 + 0xce8));
            CARDGAME_F0x80099de8(a0 + (i * 0x12 + 0xce8));
        }
    }
}
