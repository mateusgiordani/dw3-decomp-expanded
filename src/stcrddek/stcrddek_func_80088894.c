/*
 * STCRDDEK:0x80088894 STCRDDEK_func_80088894
 * 20 bytes at STCRDDEK.PRO offset 0x5be4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80088894
 *  Symbols     (none)
 *  Compare     20 bytes from 0x80088894 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80088894
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Body words: 01000224 (li v0,1) ; 6c0085ac (sw a1,0x6c(a0)) ; 700086ac (sw
 * a2,0x70(a0)) ; 0800e003 (jr ra) ; 680082ac (sw v0,0x68(a0) in delay slot).
 * All 20 bytes byte-match PAL.
 *
 * No Ghidra state change.
 *
 * Family: sibling STCRDDEK:0x80088880 has the identical shape with offsets
 * +0x78/+0x7c/+0x80;
 *
 * STCRDDEK:0x800888a8 is a single-store (a1 -> +0x74) tail of the same cluster.
 */

#include <stdint.h>

int32_t STCRDDEK_func_80088894(int32_t p1, int32_t p2, int32_t p3)
{
    *(int32_t *)(p1 + 0x6c) = p2;
    *(int32_t *)(p1 + 0x70) = p3;
    *(int32_t *)(p1 + 0x68) = 1;
    return 1;
}
