/*
 * STCRDSHP:0x800843c0 STCRDSHP_func_800843c0
 * 248 bytes at STCRDSHP.PRO offset 0x1710 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800843c0
 *  Symbols     D_80042B98=0x80042b98 STCRDSHP_func_800843c0=0x800843c0
 *  Compare     248 bytes from 0x800843c0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x800843c0
 */

#include "common/types.h"

extern uint32_t D_80042B98[];

void STCRDSHP_func_800843c0(void *object) {
    int32_t count;
    int32_t index;
    int32_t match_count;
    int32_t total;

    match_count = ((int32_t (*)(int32_t, void *))D_80042B98[3])(1, (char *)object + 0x3c4);
    index = 0;
    *(int32_t *)((char *)object + 0x3c0) = 0;
    if (match_count > 0) {
        do {
            count = ((int32_t (*)(int32_t))D_80042B98[1])
                (*(int16_t *)((char *)object + index * 2 + 0x3c4));
            if (count == 0x62) {
                *(int16_t *)((char *)object + ((*(int32_t *)((char *)object + 0x3c0))++ << 1) + 0x98) =
                    *(uint16_t *)((char *)object + index * 2 + 0x3c4);
            }
            index++;
        } while (index < match_count);
    }
    total = *(int32_t *)((char *)object + 0x3c0);
    if (total != 0) {
        *(int32_t *)((char *)object + 0x64) = total / 8 + ((total & 7) != 0);
    }
}
