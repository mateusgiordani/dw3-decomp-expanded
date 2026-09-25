/*
 * STCRDDEK:0x800832e8 STCRDDEK_func_800832e8
 * 44 bytes at STCRDDEK.PRO offset 0x638 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800832e8
 *  Symbols     EXE_F0x80014504=0x80014504 STCRDDEK_func_800831f0=0x800831f0
 *  Compare     44 bytes from 0x800832e8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800832e8
 */

#include "common/types.h"

extern void STCRDDEK_func_800831f0(void);
extern void EXE_F0x80014504(void *, int32_t, int32_t);

void STCRDDEK_func_800832e8(void) {
    EXE_F0x80014504(&STCRDDEK_func_800831f0, 0x50, 4);
}
