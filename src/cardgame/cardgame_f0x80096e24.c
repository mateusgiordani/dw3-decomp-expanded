// CARDGAME:0x80096e24 (size 104, 0x68)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x14174
// Framed function: prologue 27bdffd8 addiu sp,-0x28, epilogue jr ra / addiu sp,+0x28.
// Loop wrapper: 3 iterations calling CARDGAME_F0x80096cf0(a0, a1, index, a0 + off),
// off steps 0xdc0, 0xdcc, 0xdd8 (stride 12, 12-byte entries).
// Ghidra program CARDGAME read-only (project ddw3-pal-sles-03936, base 0x80082cb0):
// disasm matches PAL word-for-word (26 words); decompile is do { callee; i++; off += 12; }
// while (i < 3); x-ref from 0x80096e58 jal 0x80096cf0; caller 0x8009be10
// (CARDGAME_F0x8009bba8) jal 0x80096e24.
// Portable C recovery (rev 2): replaces the rev 1 top-level __asm__ wrapper,
// which is ASM_MATCHING evidence, not portable C (RULES R2/R3).
extern void CARDGAME_F0x80096cf0(int a0, int a1, int index, int arg);

void CARDGAME_F0x80096e24(int a0, int a1)
{
    int index;
    int off;

    index = 0;
    off = 0xdc0;
    do {
        CARDGAME_F0x80096cf0(a0, a1, index, a0 + off);
        index += 1;
        off += 12;
    } while (index < 3);
}
