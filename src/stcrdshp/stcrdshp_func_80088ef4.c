/*
 * STCRDSHP:0x80088ef4 STCRDSHP_func_80088ef4
 * 192 bytes at STCRDSHP.PRO offset 0x6244 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80088ef4
 *  Symbols     D_80044B38=0x80044b38 D_8005CCA8=0x8005cca8
 *  Compare     192 bytes from 0x80088ef4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80088ef4
 */

#include "common/types.h"

/* STCRDSHP:0x80088ef4, 192B. PAL base 0x80082cb0, file off 0x6244. */
/* EXE helper table base 0x80044B38 (+0x404 slot); arg base *0x8005CCA8. */
extern int32_t D_8005CCA8;
extern uint8_t D_80044B38[];

int32_t STCRDSHP_func_80088ef4(void) {
    uint8_t *base = D_80044B38;
    if ((*(int32_t (**)(int32_t))(base + 0x404))(D_8005CCA8 + 0x32) != 0) {
        return 1;
    }
    if ((*(int32_t (**)(int32_t))(base + 0x404))(D_8005CCA8 + 0x16) != 0) {
        return 1;
    }
    if ((*(int32_t (**)(int32_t))(base + 0x404))(D_8005CCA8 + 0x1d) != 0) {
        return 1;
    }
    if ((*(int32_t (**)(int32_t))(base + 0x404))(D_8005CCA8 + 0x94) != 0) {
        return 1;
    }
    return (*(int32_t (**)(int32_t))(base + 0x404))(D_8005CCA8 + 0x6a) != 0;
}
