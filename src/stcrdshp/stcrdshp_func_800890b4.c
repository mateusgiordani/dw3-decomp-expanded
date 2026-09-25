/*
 * STCRDSHP:0x800890b4 STCRDSHP_func_800890b4
 * 64 bytes at STCRDSHP.PRO offset 0x6404 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800890b4
 *  Symbols     STCRDSHP_func_800890b4=0x800890b4
 *  Compare     64 bytes from 0x800890b4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x800890b4
 */

#include "common/types.h"

typedef struct {
    int32_t steps;   /* 0x00 */
    int32_t unk04;   /* 0x04 */
    int32_t start;   /* 0x08 */
    int32_t fixed;   /* 0x0c */
    int32_t end;     /* 0x10 */
    int32_t step;    /* 0x14 */
    int32_t active;  /* 0x18 */
} STCRDSHP_Interp;

void STCRDSHP_func_800890b4(STCRDSHP_Interp *state, int32_t start, int32_t end,
                            int32_t steps) {
    if (start != end) {
        state->steps = steps;
        state->fixed = start << 8;
        state->start = start;
        state->end = end;
        state->active = 1;
        state->step = ((end - start) << 8) / state->steps;
    }
}
