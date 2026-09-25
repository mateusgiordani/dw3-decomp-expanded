/*
 * STCRDDEK:0x8008660c STCRDDEK_func_8008660c
 * 88 bytes at STCRDDEK.PRO offset 0x395c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008660c
 *  Symbols     F0x80014504=0x80014504 STCRDDEK_func_80086574=0x80086574
 *  Compare     88 bytes from 0x8008660c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x8008660c
 */

#include "exe/exe.h"

/*
 * allocate a 0x494-byte object through the EXE allocator F0x80014504 tagged
 * with STCRDDEK_func_80086574 (callback-style tag stored by the allocator),
 * then publish the constants 0x1000 / 6 at obj+0x50/xtag+0x54/0x58 and the two
 * caller args at obj+0x50 / obj+0x5c. Returns the fresh object (still in $v0 at
 * jr $ra).
 *
 * Caller: single direct jal from STCRDDEK 0x80089dd0 (file off 0x7120); its
 * delay-slot/jump sequence stores $v0 to 0(s3), proving the pointer return.
 * Callee 0x80014504 is cross-module (EXE); no intra-overlay def.
 *
 * STCRDDEK_func_80086574 is a real framed in-overlay function (prologue
 * 27bdffe0 at 0x80086574) passed tag-style. Names conservative.
 *
 * Portable C only, no asm, no explicit register variables.
 */
extern int32_t F0x80014504(int32_t a0, int32_t a1, int32_t a2);
extern void STCRDDEK_func_80086574(void);

int32_t STCRDDEK_func_8008660c(int32_t arg0, int32_t arg1)
{
    int32_t v0;

    v0 = F0x80014504((int32_t)STCRDDEK_func_80086574, 0x494, 0xd4);
    *(int32_t *)(v0 + 0x54) = 0x1000;
    *(int32_t *)(v0 + 0x58) = 6;
    *(int32_t *)(v0 + 0x50) = arg0;
    *(int32_t *)(v0 + 0x5c) = arg1;
    return v0;
}
