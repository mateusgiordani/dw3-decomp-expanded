/*
 * STCRDDEK:0x800838c4 STCRDDEK_func_800838c4
 * 112 bytes at STCRDDEK.PRO offset 0xc14 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800838c4
 *  Symbols     STCRDDEK_F0x800835d8=0x800835d8 STCRDDEK_F0x800837b4=0x800837b4
 *              STCRDDEK_F0x800838c4=0x800838c4
 *  Compare     112 bytes from 0x800838c4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800838c4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Previous framed STCRDDEK:0x800837b4 (272B) ends exactly at 0x800838c4; next
 * framed STCRDDEK:0x80083934 starts exactly at +0x70: size 0x70 contiguous, no
 * overlap.
 *
 * Branch chain is a linear decision tree (beq ==1, slti <2 -> default, beq ==2,
 * beq ==3, fallthrough to default); a switch lowers to a balanced tree (checks
 * ==2 first, 136B), a structured if/else chain inverts branches (bne) and
 * threads kind<2 into kind<1.
 *
 * Independent forward-goto conditions reproduce the exact tree with no
 * threading.
 *
 * Callers: none direct (bin-wide jal scan for 0x0c200e31 empty; Ghidra x-ref to
 * 0x800838c4 empty) - reachable via table or another module; no caller
 * promoted.
 *
 * Callees: 2 direct intra-overlay jals to STCRDDEK 0x800837b4 and STCRDDEK
 * 0x800835d8; 1 indirect jalr via pointer at arg + 0x38.
 *
 * No Ghidra state change.
 *
 * Semantics unconfirmed: deck-menu role is name-based only; offsets kept raw,
 * no struct promoted.
 */

#include "common/types.h"

extern void STCRDDEK_F0x800837b4(uint8_t *arg);
extern void STCRDDEK_F0x800835d8(uint8_t *arg);

void STCRDDEK_F0x800838c4(uint8_t *arg)
{
    int32_t kind;
    void (*fn)(uint8_t *);

    kind = *(int32_t *)(arg + 0x0c);
    if (kind == 1)
        goto L1;
    if (kind < 2)
        goto Ldef;
    if (kind == 2)
        goto L2;
    if (kind == 3)
        goto Lepi;
Ldef:
    fn = *(void (**)(uint8_t *))(arg + 0x38);
    fn(arg);
    *(int32_t *)(arg + 0x5c) = 0;
    goto Lepi;
L1:
    STCRDDEK_F0x800837b4(arg);
L2:
    STCRDDEK_F0x800835d8(arg);
Lepi: ;
}
