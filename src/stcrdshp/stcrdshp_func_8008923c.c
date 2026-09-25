/*
 * STCRDSHP:0x8008923c STCRDSHP_func_8008923c
 * 76 bytes at STCRDSHP.PRO offset 0x658c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008923c
 *  Symbols     D_8008CD20=0x8008cd20 STCRDSHP_func_8008923c=0x8008923c
 *  Compare     76 bytes from 0x8008923c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x8008923c
 */

#include "common/types.h"
typedef struct { int16_t key; int16_t value; } CardTableEntry;
extern CardTableEntry D_8008CD20[];
int32_t STCRDSHP_func_8008923c(int32_t value) {
    int32_t index;
    for (index = 0; D_8008CD20[index].key != 0; index++) {
        if (D_8008CD20[index].key == value) return D_8008CD20[index].value;
    }
    return 1;
}
