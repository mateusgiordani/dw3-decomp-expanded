/*
 * CARDGAME:0x800a0708 CARDGAME_F0x800a0708
 * 76 bytes at CARDGAME.PRO offset 0x1da58 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a0708
 *  Symbols     CARDGAME_F0x800a0628=0x800a0628
 *  Compare     76 bytes from 0x800a0708 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a0708
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Body: addr = a1*0x72 + 0x72c; p = (uint8_t *)(a0 + addr); old = *p; *p = old
 * + 1; CARDGAME_F0x800a0628(a0, a1, a2, old);
 *
 * Order matters for matching: constant 0x72c is added to a1*0x72 BEFORE a0
 * (addiu v0,0x72c; addu v0,a0,v0). Folding 0x72c into lbu/sb offset (v1 trial)
 * yields 72 B; adding a0 first (v2 trial) swaps two words (diff 8 at off 24).
 *
 * The sb executes in the jal delay slot; lbu/andi/addiu sequence preserved.
 *
 * Caller: CARDGAME:0x800a0754 @0x800a07f8 (jal, a0=s3/a1=s4/a2=lh 0x64(s1));
 * plus DATA xref at 0x800a3298. Callee CARDGAME:0x800a0628 takes
 * (a0,a1,a2,a3=old); a2 passes through (caller-supplied lh).
 */

#include <stdint.h>

extern void CARDGAME_F0x800a0628(int32_t a0, int32_t a1, int32_t a2, uint32_t a3);

void CARDGAME_F0x800a0708(int32_t a0, int32_t a1, int32_t a2)
{
    int32_t addr;
    uint8_t *p;
    uint8_t old;

    addr = a1 * 0x72 + 0x72c;
    p = (uint8_t *)(a0 + addr);
    old = *p;
    *p = (uint8_t)(old + 1);
    CARDGAME_F0x800a0628(a0, a1, a2, old);
}
