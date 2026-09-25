/*
 * STCRDABM:0x800856a8 STCRDABM_func_800856a8
 * 248 bytes at STCRDABM.PRO offset 0x29f8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800856a8
 *  Symbols     D0x80044b38=0x80044b38 D0x8005cca8=0x8005cca8
 *              F0x8001ffa8=0x8001ffa8
 *  Compare     248 bytes from 0x800856a8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x800856a8
 */

#include "common/types.h"

/* STCRDABM:0x800856a8 (248 bytes, 62 instructions)
 * PAL-SLES-03936; base 0x80082cb0, off 0x29f8.
 * Boundary 0x800856a8..0x800857a0 confirmed by Ghidra disasm:
 * next prologue addiu sp,sp,-0x20 at 0x800857a0.
 * Body: init stack record via EXE F0x8001ffa8, dispatch through its
 * slots +0x24/+0x2c, then fan out through the RAM vector at 0x80044f44
 * with fixed ids 0x7f6..0x7fa and RAM word 0x8005cca8 offsets.
 * The 0x80044b38 table base is held in a local so codegen keeps the
 * PAL lui/addiu pair and s0-relative loads.
 */
extern void F0x8001ffa8(void *buf);
extern uint8_t D0x80044b38[];
extern int32_t D0x8005cca8;

void STCRDABM_func_800856a8(void) {
    uint32_t buf[14];
    uint8_t *base;
    int32_t v;

    F0x8001ffa8(buf);
    ((void (*)(int32_t, int32_t))buf[9])(0x280, 0);
    base = D0x80044b38;
    v = ((int32_t (*)(int32_t))*(void **)(base + 0x424))(0x06060000);
    ((void (*)(int32_t))buf[11])(v);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(0x7f6);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(0x7f7);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(0x7f8);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(0x7f9);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(0x7fa);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(D0x8005cca8 + 0x16);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(D0x8005cca8 + 0x1d);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(D0x8005cca8 + 0x24);
}
