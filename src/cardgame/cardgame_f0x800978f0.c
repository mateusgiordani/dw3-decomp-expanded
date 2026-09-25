/* CARDGAME:0x800978f0 (size 104, 0x68) */
/* PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x14c40 (RAW, no header) */
/* Boundary: sweep reports/handoffs/cardgame-boundary-sweep.md #121; prev 0x80097508 ends 0x800978ec (gap 0), next 0x80097958 (gap 0) */
/* Disasm (PAL words): prologue addiu sp,-0x28, saves s2/s3/s0/s1/ra; s2=a0, s3=a1, s0=0, s1=0xe0c; */
/* loop @0x80097918: a0=s2, a1=s3, a2=s2+s1, jal 0x80097508 with a3=s0 in delay slot; s0++, slti v0,s0,6, */
/* bne loop with addiu s1,+0x18 in delay slot; epilogue restores ra/s3/s2/s1/s0, jr ra. 6 iterations, */
/* s1 steps 0xe0c,0xe24,0xe3c,0xe54,0xe6c,0xe84. Xrefs: 1 caller (jal at 0x8009bdf8), 1 callee 0x80097508. */
/* Ghidra program CARDGAME read-only (never import); cardgame.s GUIDE only. */
/* Match: portable C on pinned base psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 -mips1 -msoft-float. */
/* The offset induction variable keeps s1 as a byte offset (addu a2,s2,s1 / addiu s1,0x18); aspsx */
/* canonicalizes cc1 idioms (subu sp frame -> addiu, move -> addu, slt imm -> slti, j r31 -> jr r31). */
/* Pointer-shaped source (s1 as pointer) mismatches 13 words; gcc-2.7.2 overshoots to 108 bytes. */

extern void CARDGAME_F0x80097508(void *a0, void *a1, void *a2, int a3);

void CARDGAME_F0x800978f0(void *a0, void *a1)
{
    int i;
    int off;

    for (i = 0, off = 0xe0c; i < 6; i++, off += 0x18)
        CARDGAME_F0x80097508(a0, a1, (unsigned char *)a0 + off, i);
}
