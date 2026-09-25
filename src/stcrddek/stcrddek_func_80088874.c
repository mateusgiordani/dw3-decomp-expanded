/*
 * STCRDDEK:0x80088874 STCRDDEK_func_80088874
 * 12 bytes at STCRDDEK.PRO offset 0x5bc4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80088874
 *  Symbols     STCRDDEK_func_80088874=0x80088874
 *  Compare     12 bytes from 0x80088874 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80088874
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * No Ghidra state change.
 *
 * Same setter family as neighbours 0x80088880/0x80088894 (store a1/a2 pair).
 *
 * sha256 4139fc3c4238b5db0df0657a202b4cc5569d4908e6fc5689f111ca65687870a3
 * (12B).
 */

#include <stdint.h>

void STCRDDEK_func_80088874(int32_t p1, int32_t p2, int32_t p3)
{
    *(uint32_t *)(p1 + 0x58) = p2;
    *(uint32_t *)(p1 + 0x60) = p3;
}
