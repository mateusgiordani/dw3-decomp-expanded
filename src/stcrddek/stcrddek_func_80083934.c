/*
 * STCRDDEK:0x80083934 STCRDDEK_func_80083934
 * 72 bytes at STCRDDEK.PRO offset 0xc84 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80083934
 *  Symbols     F0x80014504=0x80014504 STCRDDEK_func_800838c4=0x800838c4
 *              STCRDDEK_func_8008397c=0x8008397c
 *  Compare     72 bytes from 0x80083934 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80083934
 */

#include "common/types.h"

/*
 * Semantics: v0 = F0x80014504(STCRDDEK_func_800838c4, 0x70, 0) (EXE allocator /
 * registrar shared by STCRD* entries; a2 = 0 via move); then constants 0x1000
 * at v0+0x54, 5 at v0+0x58, caller arg (saved via s0) at v0+0x50.
 *
 * v0 is preserved to jr ra, so the fresh object is returned. The arg word type
 * is unrecovered (stored opaquely); int32_t keeps width/sign explicit.
 *
 * Callback STCRDDEK_func_800838c4 dispatches on *(arg+0xC): 1 -> 0x800837b4
 * then 0x800835d8; 2 -> 0x800835d8; 3 -> return; else indirect call via slot at
 * arg+0x38.
 *
 * Portable C only, no asm, no explicit register variables. Operation order
 * (0x54, 0x58, 0x50) preserved for matching.
 */
extern int32_t F0x80014504(int32_t a0, int32_t a1, int32_t a2);
extern void STCRDDEK_func_800838c4(void);

int32_t STCRDDEK_func_80083934(int32_t arg0)
{
    int32_t obj;

    obj = F0x80014504((int32_t)STCRDDEK_func_800838c4, 0x70, 0);
    *(int32_t *)(obj + 0x54) = 0x1000;
    *(int32_t *)(obj + 0x58) = 5;
    *(int32_t *)(obj + 0x50) = arg0;
    return obj;
}
