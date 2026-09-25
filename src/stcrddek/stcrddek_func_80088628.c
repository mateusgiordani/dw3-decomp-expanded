/*
 * STCRDDEK:0x80088628 STCRDDEK_func_80088628
 * 12 bytes at STCRDDEK.PRO offset 0x5978 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80088628
 *  Symbols     L0x80088628=0x80088628 STCRDDEK_func_800887d4=0x800887d4
 *  Compare     12 bytes from 0x80088628 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80088628
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Xref: one direct UNCONDITIONAL_CALL from STCRDDEK_func_800887d4 at 0x80088854
 * (jal 0x80088628; delay-slot li a2,0x100); callers graph depth-2 agrees.
 * Semantics of +0x5c/+0x60 fields and of the 0x100 constant are unconfirmed;
 * deck-menu role is a name-based hypothesis only.
 *
 * FIGHTSTG .L0x80088628 (sw $v0,($s3)) is a different module at the same
 * runtime address; never merged (overlay identity is MODULE:address).
 */

#include <stdint.h>

void STCRDDEK_func_80088628(int32_t *p, int32_t arg1, int32_t arg2)
{
    p[23] = arg1;
    p[24] = arg2;
}
