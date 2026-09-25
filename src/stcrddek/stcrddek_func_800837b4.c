// STCRDDEK:0x800837b4 (size 272, 0x110)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0xb04 (RAW, no header)
// Boundary: prologue 27bdff90 addiu sp,sp,-0x70 + sw s0/ra at 0x800837b4-0x800837c0;
// epilogue lw ra,0x6c(sp) + lw s0,0x68(sp) + jr ra + addiu sp,sp,+0x70 at
// 0x800838b4-0x800838c0. Next framed STCRDDEK:0x800838c4 at +0x110
// (27bdffe8 addiu sp,sp,-0x18), contiguous, no overlap.
// Ghidra program STCRDDEK (project ddw3-pal-sles-03936) read-only: 68-insn disasm
// matches PAL word-for-word (sha256 a3060d9f5443b571fb36d42c9ed97609ffbe9bbc26763110d1699196cb1980d4).
// Decompile hypothesis + disasm: counter at +0x5c pre-incremented; if >= 0x29,
// state at +0xc = 2 and counter reset to 0x28, else one deck-grid step: EXE helper
// fills a stack callback block, then 5 indirect calls driven by an int16 card id
// from an EXE table, two (0x140,0x100)/(0x300,0x100) pairs, (counter-1)%9, flush.
// Caller: single direct jal from dispatcher STCRDDEK:0x800838c4 (state 1 path,
// pairs with STCRDDEK:0x800835d8, which reads the same EXE table base).
// Callee: 1 direct jal to EXE 0x8001ebf8.
// Table: EXE halfword entries at 0x80048d34+0x63e (int16 card ids); the lui/addiu
// pair is a relocated extern address (linker high-adjusted form), not an immediate.
// upstream stcrddek.s is GUIDE only, never copied. No Ghidra state change.
// Toolchain base: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0.
// Status: C_NONMATCHING (portable C,word-exact PAL boundary; see report for diff).
#include <stdint.h>

extern void F0x8001ebf8(void *);
extern int16_t D0x80048D34[];

void STCRDDEK_func_800837b4(int32_t *p)
{
    uint32_t cb[22];
    int16_t *tbase;
    int32_t *sub;
    int32_t idx;
    int32_t stride;
    int32_t col;
    int16_t card;
    void (*cb0)(int32_t);
    void (*cb1)(void);
    void (*cb2)(int32_t, int32_t);
    void (*cb3)(int32_t, int32_t);
    void (*cb4)(int32_t, int32_t);

    if (++p[23] >= 0x29) {
        p[3] = 2;
        p[23] = 0x28;
    } else {
        F0x8001ebf8(cb);
        tbase = D0x80048D34;
        sub = (int32_t *)p[20];
        idx = p[23] - 1;
        stride = sub[23];
        card = *(int16_t *)((uint8_t *)tbase + (stride * 51 + idx) * 2 + 0x63e);
        cb0 = (void (*)(int32_t))cb[11];
        cb0(card);
        cb2 = (void (*)(int32_t, int32_t))cb[15];
        cb2(0x140, 0x100);
        cb3 = (void (*)(int32_t, int32_t))cb[16];
        cb3(0x300, 0x100);
        col = p[23] - 1;
        cb4 = (void (*)(int32_t, int32_t))cb[17];
        cb4(col % 9, col / 9);
        cb1 = (void (*)(void))cb[12];
        cb1();
    }
}
