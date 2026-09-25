// STCRDDEK:0x80088ac8 (size 108, 0x6c)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x5e18 (RAW, no header)
// Boundary: prologue 27bde8ff addiu sp,sp,-0x18, sw ra,0x10(sp) before jal;
// epilogue lw ra,0x10(sp) + nop + jr ra + addiu sp,sp,+0x18 at 0x80088b24-0x80088b30.
// Next framed STCRDDEK:0x80088b34 at +0x6c (27bdffc8 addiu sp,sp,-0x38), size 0x6c contiguous, no overlap.
// Ghidra program STCRDDEK (project ddw3-pal-sles-03936) read-only: disasm 27 words matches PAL
// word-for-word (sha256 f8405099aa86aeaa3fd8aad6c3ac59c2c067881adec6392b39ebc7115d137418);
// decompile: v0 = func_0x80014504(STCRDDEK_func_800888b0, 0x9c, 0);
// stores to v0+0x8c/0x90/0x94/0x98, then 0x1000 at v0+0x50, 3 at v0+0x54; returns v0.
// Callers: 1 direct jal from STCRDDEK 0x800859ec (UNCONDITIONAL_CALL); caller saves v0 to
// 0xd0(s3) then dispatches via slots v0+0x8c/0x90 with (a0, 0x125)/(a0, 0x2a) args.
// Callees: 1 direct cross-module jal to EXE 0x80014504 (a0=0x800888b0, a1=0x9c, a2=0).
// Slot targets are tiny in-overlay setters at 0x80088874/0x80088880/0x80088894/0x800888a8
// (sw a1/a2 to small offsets, return 1); 0x800888b0 is a framed in-overlay function.
// stcrddek.s is GUIDE only; never copied as source. No Ghidra state change.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base), exact_byte_match.
// Status: C_MATCHING (portable C, no asm, no explicit register variables).
#include "common/types.h"

extern void *F0x80014504(int32_t a0, int32_t a1, int32_t a2);
extern void STCRDDEK_F0x800888b0(void);
extern void STCRDDEK_F0x80088874(void);
extern void STCRDDEK_F0x80088880(void);
extern void STCRDDEK_F0x80088894(void);
extern void STCRDDEK_F0x800888a8(void);

void *STCRDDEK_F0x80088ac8(void)
{
    void *v0;

    v0 = F0x80014504((int32_t)STCRDDEK_F0x800888b0, 0x9c, 0);
    *(void **)((uint8_t *)v0 + 0x8c) = (void *)STCRDDEK_F0x80088874;
    *(void **)((uint8_t *)v0 + 0x90) = (void *)STCRDDEK_F0x80088880;
    *(void **)((uint8_t *)v0 + 0x94) = (void *)STCRDDEK_F0x80088894;
    *(void **)((uint8_t *)v0 + 0x98) = (void *)STCRDDEK_F0x800888a8;
    *(int32_t *)((uint8_t *)v0 + 0x50) = 0x1000;
    *(int32_t *)((uint8_t *)v0 + 0x54) = 3;
    return v0;
}
