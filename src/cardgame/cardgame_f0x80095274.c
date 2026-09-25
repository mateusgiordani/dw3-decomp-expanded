// CARDGAME:0x80095274 (size 128, 0x80) -- portable C, C_MATCHING (exact_byte_match)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x125c4
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #100:
// prologue 27bdffe8 addiu sp,-0x18 ; sw s0,0x10(sp) ; move s0,a0 ; sw ra,0x14(sp),
// epilogue lw ra,0x14(sp) ; lw s0,0x10(sp) ; jr ra ; addiu sp,+0x18 (frame -0x18/+0x18).
// Next framed CARDGAME:0x8009535c at +0xe8 (gap 0x68 leaf/data), prev CARDGAME:0x8009518c
// size 0x7c ends 0x80095208 (gap 0x6c to this start); size 0x80 contiguous, no overlap.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm confirms
// lb/addiu/sll/addu/lbu table check; decompile hypothesis matches this C shape.
// cardgame.s is GUIDE only (2x jal 0x80095274 at L0x18cc/0x18e4, file-off 0x18cc/0x18e4,
// vaddr 0x8008457c/0x80084594); never copied as source. No Ghidra state change.
// Callers: 2 direct jal (PAL bin scan for 0x0c02549d at file-off 0x18cc/0x18e4).
// Callee: 1 indirect jalr via lw v0,0xec8(a0) after move a0,a1 (a0=q at call).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0, exact_byte_match,
// sha256 daee2b81db151ae1b6d0058144888d67eb39212376687f438c0c50de9ab57ee0.
// Signedness duality at +0x575 is compiler-observed and required for matching:
// first read is lb (signed index math), post-call decrement reload is lbu
// (unsigned) followed by addiu -1 and sb. Both views kept; no struct invented.
// Delay slots (move a0,a1 / li v0,2 / sb v1,0x575(s0)) are compiler-scheduled
// from this exact C shape; passing q to the indirect callee forces the early move.

// Indirect callee stored at q+0xec8; called with q as first argument (a0).
typedef void (*cardgame_80095274_cb_t)(void *arg);

void CARDGAME_F0x80095274(unsigned char *p, unsigned char *q, int r) {
    signed char idx = ((signed char *)p)[0x575];
    unsigned char t = *(unsigned char *)(p + ((idx - 1) << 3) + 0x584);

    if (t == 0 || r != 0) {
        ((cardgame_80095274_cb_t *)(q + 0xec8))[0](q);
        p[0x49d] = 1;
        p[0x499] = 1;
        p[0x422] = 1;
        p[0x575]--;
    } else {
        p[0x422] = 2;
    }
}
