/*
 * STCRDDEK:0x80088880 STCRDDEK_func_80088880
 * 20 bytes at STCRDDEK.PRO offset 0x5bd0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80088880
 *  Symbols     (none)
 *  Compare     20 bytes from 0x80088880 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80088880
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Next function STCRDDEK:0x80088894 at +0x14 (li v0,1 prologue), contiguous, no
 * overlap.
 *
 * No Ghidra state change.
 *
 * No Ghidra state change.
 *
 * Xrefs: no direct xrefs/callers in Ghidra; no callees (leaf). Sibling setters
 * at 0x80088874 (12 B, stores a1/a2 only) and 0x80088894 (same shape, offsets
 * 0x68/0x6c/0x70).
 *
 * sha256 dad0f2e3aac60fcaab0769d9a75ec15ceed3cc433e4051affbc8ea251d3afacd
 * (20B).
 */

#include <stdint.h>

void STCRDDEK_func_80088880(int32_t a0, int32_t a1, int32_t a2)
{
    *(int32_t *)(a0 + 0x7c) = a1;
    *(int32_t *)(a0 + 0x80) = a2;
    *(int32_t *)(a0 + 0x78) = 1;
}
