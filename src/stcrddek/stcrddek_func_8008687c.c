/*
 * STCRDDEK:0x8008687c STCRDDEK_func_8008687c
 * 108 bytes at STCRDDEK.PRO offset 0x3bcc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008687c
 *  Symbols     (none)
 *  Compare     108 bytes from 0x8008687c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x8008687c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * No Ghidra state change.
 *
 * Behavior (conservative): if word at +0xc == 0 return 1; else add word at +0x4
 * into word at +0x8, store the sum, then clamp: delta > 0 with sum > 0x1000, or
 * delta <= 0 with sum < 0, clears the accumulator (and the +0xc word on the
 * taken clamp path) and returns 1; otherwise returns 0. The delta > 0 case is
 * written first so the upper-bound check stays on the fall-through path, as in
 * the original branch layout (blez to the negative-side check). Struct layout
 * beyond these three words is unrecovered, so the object stays int32_t *.
 */

#include "common/types.h"

int32_t STCRDDEK_func_8008687c(int32_t *arg0) {
    int32_t sum;

    if (arg0[3] == 0) {
        return 1;
    }
    sum = arg0[2] + arg0[1];
    arg0[2] = sum;
    if (arg0[1] > 0) {
        if (sum > 0x1000) {
            arg0[2] = 0x1000;
            arg0[3] = 0;
            return 1;
        }
    } else if (sum < 0) {
        arg0[2] = 0;
        arg0[3] = 0;
        return 1;
    }
    return 0;
}
