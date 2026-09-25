/*
 * STCRDDEK:0x800887c8 STCRDDEK_func_800887c8
 * 12 bytes at STCRDDEK.PRO offset 0x5b18 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800887c8
 *  Symbols     (none)
 *  Compare     12 bytes from 0x800887c8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800887c8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Decompile: *(undefined4 *)(param_1 + 0x10) = 10; return;
 *
 * Xref: no direct callers in STCRDDEK (empty x-ref to 0x800887c8; table/EXE
 * callers not excluded).
 */

#include <stdint.h>

void STCRDDEK_func_800887c8(int32_t p1)
{
    *(uint32_t *)(p1 + 0x10) = 10;
}
