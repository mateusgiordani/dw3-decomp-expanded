/*
 * STCRDDEK:0x8008a0b8 STCRDDEK_func_8008a0b8
 * 72 bytes at STCRDDEK.PRO offset 0x7408 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008a0b8
 *  Symbols     F0x80014504=0x80014504 FIELDSTG_F0x800872e4=0x800872e4
 *              FIELDSTG_F0x800873d8=0x800873d8
 *              STCRDDEK_func_800831f0=0x800831f0
 *              STCRDDEK_func_8008924c=0x8008924c
 *              STCRDDEK_func_80089e98=0x80089e98
 *  Compare     72 bytes from 0x8008a0b8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x8008a0b8
 */

#include "exe/exe.h"

/*
 * allocate a 0x120-byte object through the EXE allocator F0x80014504 tagged
 * with STCRDDEK_func_80089e98 (the allocator stores the tag at obj+0x48, slot
 * 18 -- the same tag role as FIELDSTG_F0x800872e4 in FIELDSTG_F0x800873d8),
 * then publish STCRDDEK_func_8008924c at obj+0x11c and the constants 0x1000 / 7
 * at obj+0x50 / obj+0x54. Returns the fresh object (still in $v0 at jr $ra).
 *
 * STCRDDEK_func_80089e98 and STCRDDEK_func_8008924c are real code (MIPS
 * prologues 27bdffe0 / 27bdff70 at their first words); 0x8008924c is also
 * called directly from 0x800893a8, while 0x80089e98 has no direct jal and is
 * passed tag-style. Callee semantics unconfirmed; names conservative.
 */
extern int32_t F0x80014504(int32_t a0, int32_t a1, int32_t a2);
extern void STCRDDEK_func_80089e98(void);
extern void STCRDDEK_func_8008924c(void);

int32_t STCRDDEK_func_8008a0b8(void)
{
    int32_t obj;

    obj = F0x80014504((int32_t)STCRDDEK_func_80089e98, 0x120, 0x6c);
    *(int32_t *)(obj + 0x11c) = (int32_t)STCRDDEK_func_8008924c;
    *(int32_t *)(obj + 0x50) = 0x1000;
    *(int32_t *)(obj + 0x54) = 7;
    return obj;
}
