/*
 * CARDGAME:0x80086d28 CARDGAME_F0x80086d28
 * 80 bytes at CARDGAME.PRO offset 0x4078 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80086d28
 *  Symbols     CARDGAME_F0x80086a30=0x80086a30
 *  Compare     80 bytes from 0x80086d28 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80086d28
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Disasm: addiu sp,-24 ; sw ra,16(sp) ; move v1,a2 ; li v0,1 ; beq v1,v0,L1
 * (delay: move a2,zero) ; li v0,2 ; beq v1,v0,L2 (delay: nop) ; j Ljoin (delay:
 * nop) ; L1: j Ljoin (delay: li a2,6) ; L2: li a2,12 ; Ljoin: jal 0x80086a30
 * (delay: nop) ; lw ra,16(sp) ; nop ; jr ra (delay: addiu sp,+24)
 *
 * Next framed CARDGAME:0x80086d78 at +0x50, prev CARDGAME:0x80086a30 size 0x2f8
 * ends exactly at 0x80086d28: size 0x50 self-contained (prologue/epilogue
 * pair).
 *
 * Frame -0x18/+0x18, 4 Ghidra xrefs (jal sites in 0x8008a068/0x8008ad98), 1
 * callee CARDGAME:0x80086a30 (extern + --symbol).
 *
 * The sel=0 initializer lets the compiler fill the first beq delay slot with
 * move a2,zero, and the explicit goto-join shape emits the two j-to-join with
 * nop delays plus jal-delay nop, matching the reference's unscheduled layout.
 *
 * The subu/addu/j $31 emitted by cc1 are assembled by ASPSX to the reference
 * addiu/jr ra words via pseudo-instruction expansion.
 */

#include "common/types.h"

extern void CARDGAME_F0x80086a30(void *a0, void *a1, int32_t a2);

void CARDGAME_F0x80086d28(void *a0, void *a1, int32_t a2)
{
    int32_t sel = 0;

    if (a2 == 1)
        goto L6;
    if (a2 == 2)
        goto L12;
    goto Ljoin;
L6:
    sel = 6;
    goto Ljoin;
L12:
    sel = 12;
Ljoin:
    CARDGAME_F0x80086a30(a0, a1, sel);
}
