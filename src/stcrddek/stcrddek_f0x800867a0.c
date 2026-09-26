/*
 * STCRDDEK:0x800867a0 STCRDDEK_F0x800867a0
 * 72 bytes at STCRDDEK.PRO offset 0x3af0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800867a0
 *  Symbols     EXE_F0x80014504=0x80014504 STCRDDEK_func_8008671c=0x8008671c
 *  Compare     72 bytes from 0x800867a0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800867a0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x18 (s0, ra).
 *
 * Creates the 0x6c-byte object handled by STCRDDEK_func_8008671c (EXE
 * 0x80014504, flags 0), sets +0x54 = 0x1000, +0x58 = 6 and stores `arg` at
 * +0x50, returning the object.
 */

#include <stdint.h>

int32_t EXE_F0x80014504(void *handler, int32_t size, int32_t flags);
int32_t STCRDDEK_func_8008671c();

#define S32(p, o)    (*(int32_t *)((p) + (o)))

int32_t STCRDDEK_F0x800867a0(int32_t arg)
{
    int32_t obj;

    obj = EXE_F0x80014504(STCRDDEK_func_8008671c, 0x6c, 0);
    S32(obj, 0x54) = 0x1000;
    S32(obj, 0x58) = 6;
    S32(obj, 0x50) = arg;
    return obj;
}
