/*
 * STCRDABM:0x80084bc8 STCRDABM_func_80084bc8
 * 112 bytes at STCRDABM.PRO offset 0x1f18 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80084bc8
 *  Symbols     D0x80048d34=0x80048d34
 *  Compare     112 bytes from 0x80084bc8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80084bc8
 */

#include "common/types.h"

extern uint8_t D0x80048d34[];

/* STCRDABM:0x80084bc8 (112 bytes, 28 instructions)
 * PAL-SLES-03936; reviewed body-80084bc8, base 0x80082cb0.
 * Single loop-carried table offset incremented at loop top keeps the PAL
 * addiu/slti/addu/lbu-0x4df shape; entry stays a byte offset from object so
 * the O2 induction-variable fold does not merge +0x80 into the pointer init
 * (which would emit addiu $5,$4,128 with 0($5) stores instead of PAL move
 * with 0x80(a1) stores and fold +1 into lbu 0x4e0).
 */
void STCRDABM_func_80084bc8(int32_t object) {
    int32_t index;
    int32_t offset;
    int32_t entry;
    index = 0;
    *(int32_t *)(object + 0x7c) = 0;
    offset = *(int32_t *)(object + 0x60) * 0xc;
    entry = 0;
    do {
        offset += 1;
        if (offset < 0x13b && D0x80048d34[offset + 0x4df] != 0) {
            *(int32_t *)(object + entry + 0x80) = 1;
            *(int32_t *)(object + 0x7c) = 1;
        } else {
            *(int32_t *)(object + entry + 0x80) = 0;
        }
        index++;
        entry += 4;
    } while (index < 0xc);
}
