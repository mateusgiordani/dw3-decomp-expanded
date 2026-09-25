/*
 * CARDGAME:0x800978f0 CARDGAME_F0x800978f0
 * 104 bytes at CARDGAME.PRO offset 0x14c40 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800978f0
 *  Symbols     CARDGAME_F0x80097508=0x80097508
 *  Compare     104 bytes from 0x800978f0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800978f0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Disasm (PAL words): prologue addiu sp,-0x28, saves s2/s3/s0/s1/ra; s2=a0,
 * s3=a1, s0=0, s1=0xe0c; loop @0x80097918: a0=s2, a1=s3, a2=s2+s1, jal
 * 0x80097508 with a3=s0 in delay slot; s0++, slti v0,s0,6, bne loop with addiu
 * s1,+0x18 in delay slot; epilogue restores ra/s3/s2/s1/s0, jr ra. 6
 * iterations, s1 steps 0xe0c,0xe24,0xe3c,0xe54,0xe6c,0xe84. Xrefs: 1 caller
 * (jal at 0x8009bdf8), 1 callee 0x80097508.
 *
 * The offset induction variable keeps s1 as a byte offset (addu a2,s2,s1 /
 * addiu s1,0x18); aspsx canonicalizes cc1 idioms (subu sp frame -> addiu, move
 * -> addu, slt imm -> slti, j r31 -> jr r31).
 *
 * Pointer-shaped source (s1 as pointer) mismatches 13 words; gcc-2.7.2
 * overshoots to 108 bytes.
 */

extern void CARDGAME_F0x80097508(void *a0, void *a1, void *a2, int a3);

void CARDGAME_F0x800978f0(void *a0, void *a1)
{
    int i;
    int off;

    for (i = 0, off = 0xe0c; i < 6; i++, off += 0x18)
        CARDGAME_F0x80097508(a0, a1, (unsigned char *)a0 + off, i);
}
