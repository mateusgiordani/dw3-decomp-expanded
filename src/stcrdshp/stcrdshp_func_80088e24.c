/*
 * STCRDSHP:0x80088e24 STCRDSHP_func_80088e24
 * 208 bytes at STCRDSHP.PRO offset 0x6174 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80088e24
 *  Symbols     D_80044B38=0x80044b38 D_8005CCA8=0x8005cca8
 *              F0x8001ffa8=0x8001ffa8 STCRDDEK_func_8008a100=0x8008a100
 *  Compare     208 bytes from 0x80088e24 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80088e24
 */

#include "common/types.h"

/*
 * Next function STCRDSHP:0x80088ef4 starts immediately after (no gap/overlap).
 *
 * STCRDSHP bytes authoritative: Ghidra words match the reference bin
 * word-for-word, so the campaign program stands.
 */
extern void F0x8001ffa8(void *buf);
extern struct {
    char pad[0x40c];
    void (*fn40c)(int);
    char pad2[0x14];
    void (*fn424)(int);
} D_80044B38;
extern int D_8005CCA8;

void STCRDSHP_func_80088e24(void) {
    char buf[56];
    int ret;

    F0x8001ffa8(buf);
    ((void (*)(int, int))*(void **)(buf + 0x24))(640, 0);
    ret = ((int (*)(int))D_80044B38.fn424)(0x06420000);
    ((void (*)(int))*(void **)(buf + 0x2c))(ret);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x32);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x16);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x1D);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x94);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x6A);
}
