// CARDGAME:0x8009edcc (304B) PAL @ 0x80082cb0 base
// Reference: reference/extracted/pro/cardgame.bin file-off = 0x8009edcc-0x80082cb0 = 0x1c11c
// Ghidra: project ddw3-pal-sles-03936 program CARDGAME disasm 0x8009edcc --instructions 76, decompile 0x8009edcc
// PAL bytes verified word-for-word (prologue d0ffbd27 .. epilogue j ra). Caller via CARDGAME_F0x8009e668 etc.
// Semantics: double loop (outer 2 x inner 5) + 5 tail calls per outer, all via *(a1+0x18)+0xea0 slot.
//   a0 = uint8_t *base (stride 0xc8), a1 = context with slot at +0x18.
//   Inner byte at base+off+inner+0x5a8 (lbu), outer halfwords at base+off+0x5a4/0x5a6/0x5a2/0x59c/0x59e (lh).
//   Outer offset off = outer*0xc8, cur = a0+off (both kept in s-regs per PAL: s2=cur, s3=off, s5=base).
// Codegen notes (psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, -O2 -G0):
//   Base variant GIV-folds cur into base+0x59e induction (lh 6($18) etc) = 29B diff.
//   o2-g0-no-strength-reduce (-fno-strength-reduce) keeps cur in s2 with large
//   immediates (lh 0x5a4(s2) etc); inner order forced via a0+(inner+off) spelling.
//   The idx5 tail block keeps off += 0xc8 after the callback; this placement produces
//   PAL's li a2,5 before the loads and addiu s3,0xc8 in the jalr delay slot. The
//   canonical o2-g0-no-strength-reduce build is byte-identical across all 304 bytes:
//   frame 0x30, spill order, regs (s5=base,s4=ctx,s1=outer,s0=inner,s2=cur,s3=off),
//   inner two-add, 4/5 tail blocks, epilogue.
// Status: C_MATCHING (304/304 bytes exact, variant o2-g0-no-strength-reduce)
#include <stdint.h>

typedef struct {
    uint8_t pad[0x59c];
    int16_t f59c; // 0x59c
    int16_t f59e; // 0x59e
    uint8_t gap[2]; // 0x5a0-0x5a1
    int16_t f5a2; // 0x5a2
    int16_t f5a4; // 0x5a4
    int16_t f5a6; // 0x5a6
    uint8_t f5a8; // 0x5a8
} S;

void CARDGAME_F0x8009edcc(uint8_t *a0, uint8_t *a1) {
    int32_t outer = 0;
    int32_t inner = 0;
    uint8_t *cur = a0;
    int32_t off = 0;
    do {
        inner = 0;
        do {
            int32_t ctx = *(int32_t *)(a1 + 0x18);
            int32_t fn = *(int32_t *)(ctx + 0xea0);
            ((void (*)(int32_t, int32_t, int32_t, int32_t))fn)(ctx, outer, inner, ((uint8_t *)(a0 + (inner + off)))[0x5a8]);
            inner++;
        } while (inner < 5);
        {
            int32_t ctx = *(int32_t *)(a1 + 0x18);
            int32_t fn = *(int32_t *)(ctx + 0xea0);
            ((void (*)(int32_t, int32_t, int32_t, int32_t))fn)(ctx, outer, 5, ((S *)cur)->f5a4);
            off += 0xc8;
        }
        {
            int32_t ctx = *(int32_t *)(a1 + 0x18);
            int32_t fn = *(int32_t *)(ctx + 0xea0);
            ((void(*)(int32_t,int32_t,int32_t,int32_t))fn)(ctx, outer, 6, ((S*)cur)->f5a6);
        }
        {
            int32_t ctx = *(int32_t *)(a1+0x18);
            int32_t fn = *(int32_t*)(ctx+0xea0);
            ((void(*)(int32_t,int32_t,int32_t,int32_t))fn)(ctx, outer, 7, ((S*)cur)->f5a2);
        }
        {
            int32_t ctx = *(int32_t*)(a1+0x18);
            int32_t fn = *(int32_t*)(ctx+0xea0);
            ((void(*)(int32_t,int32_t,int32_t,int32_t))fn)(ctx, outer, 8, ((S*)cur)->f59c);
        }
        {
            int32_t ctx = *(int32_t*)(a1+0x18);
            int32_t fn = *(int32_t*)(ctx+0xea0);
            ((void(*)(int32_t,int32_t,int32_t,int32_t))fn)(ctx, outer, 9, ((S*)cur)->f59e);
        }
        cur += 0xc8;
        outer++;
    } while (outer < 2);
}
