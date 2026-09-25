// STCRDDEK:0x800887d4 (size 160, 0xa0)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x5bc4 (RAW, no header)
// Boundary: prologue 27bde0ff addiu sp,sp,-0x20 + sw s1,0x14(sp) at 0x800887d4-0x800887d8;
// epilogue lw ra,0x18(sp) + lw s1,0x14(sp) + lw s0,0x10(sp) + jr ra + addiu sp,sp,+0x20
// at 0x80088860-0x80088870. Prev leaf STCRDDEK:0x800887c8 ends 0x800887d0 (delay slot);
// next STCRDDEK:0x80088874 starts +0xa0, size 0xa0 contiguous, no overlap.
// Ghidra project ddw3-pal-sles-03936, program STCRDDEK, read-only: disasm 40 words;
// decompile: v0 = func_0x80014504(STCRDDEK_func_800884e8, 0xf4, 0x34); slots v0+0xec/0xf0 =
// 0x800886c8/0x800887c8; fields v0+0x54 = 0x1000, v0+0x58 = 3, v0+0x50 = 2, v0+0x94 = 10,
// v0+0x64 = -1 (delay slot); STCRDDEK_func_80088634(v0, param); STCRDDEK_func_80088628
// (v0, 0x280, 0x100); return v0.
// Callers: 1 direct jal from STCRDDEK 0x80089dfc (UNCONDITIONAL_CALL, delay addu a0,v0,a0);
// caller stores v0 to 0x0(s3), then dispatches via slot 0x28(a0) with a1 = 2.
// Callees: cross-module jal to EXE 0x80014504 (a0 = 0x800884e8, a1 = 0xf4, a2 = 0x34);
// in-overlay jals to STCRDDEK 0x80088634 (framed, a0 = obj, a1 = param) and 0x80088628
// (leaf setter: sw a1,0x5c(a0) / sw a2,0x60(a0)).
// 0x800884e8/0x800886c8 are framed in-overlay functions; 0x800887c8 is a leaf setter
// (li v0,0xa; jr; sw v0,0x10(a0)) immediately before this prologue.
// stcrddek.s is GUIDE only; never copied as source. No Ghidra state change.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
// Status: C_MATCHING (portable C, no asm, no explicit register variables).
#include "common/types.h"

extern void *F0x80014504(int32_t a0, int32_t a1, int32_t a2);
extern void STCRDDEK_F0x800884e8(void);
extern void STCRDDEK_F0x800886c8(void);
extern void STCRDDEK_F0x800887c8(void);
extern void STCRDDEK_F0x80088634(void *a0, int32_t a1);
extern void STCRDDEK_F0x80088628(void *a0, int32_t a1, int32_t a2);

void *STCRDDEK_F0x800887d4(int32_t a0)
{
    void *v0;

    v0 = F0x80014504((int32_t)STCRDDEK_F0x800884e8, 0xf4, 0x34);
    *(void **)((uint8_t *)v0 + 0xec) = (void *)STCRDDEK_F0x800886c8;
    *(void **)((uint8_t *)v0 + 0xf0) = (void *)STCRDDEK_F0x800887c8;
    *(int32_t *)((uint8_t *)v0 + 0x54) = 0x1000;
    *(int32_t *)((uint8_t *)v0 + 0x58) = 3;
    *(int32_t *)((uint8_t *)v0 + 0x50) = 2;
    *(int32_t *)((uint8_t *)v0 + 0x94) = 10;
    *(int32_t *)((uint8_t *)v0 + 0x64) = -1;
    STCRDDEK_F0x80088634(v0, a0);
    STCRDDEK_F0x80088628(v0, 0x280, 0x100);
    return v0;
}
