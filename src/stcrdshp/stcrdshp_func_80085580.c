/*
 * STCRDSHP:0x80085580 STCRDSHP_func_80085580
 * 180 bytes at STCRDSHP.PRO offset 0x28d0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80085580
 *  Symbols     STCRDSHP_func_8008543c=0x8008543c
 *  Compare     180 bytes from 0x80085580 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80085580
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * 180-byte PAL body, base 0x80082cb0.
 */

#include "common/types.h"

extern void STCRDSHP_func_8008543c(int32_t *arg0);

void STCRDSHP_func_80085580(int32_t *arg0) {
    int32_t state;
    int32_t sum;
    int32_t next;

    state = arg0[3];
    if (state == 1) {
        goto state1;
    }
    if (state < 2) {
        goto indirect;
    }
    if (state == 2) {
        goto tail;
    }
    if (state == 3) {
        return;
    }
indirect:
    ((void (*)(int32_t *))arg0[14])(arg0);
    return;
state1:
    if (arg0[4] == 0) {
        return;
    }
    sum = arg0[23] + arg0[24];
    arg0[23] = sum;
    if (arg0[22] != 0) {
        goto negdir;
    }
    if (sum <= 0xFF00) {
        goto tail;
    }
    next = 2;
    arg0[23] = 0xFF00;
    goto set2;
negdir:
    if (sum >= 0) {
        goto tail;
    }
    next = 2;
    arg0[23] = 0;
set2:
    arg0[3] = next;
tail:
    STCRDSHP_func_8008543c(arg0);
}
