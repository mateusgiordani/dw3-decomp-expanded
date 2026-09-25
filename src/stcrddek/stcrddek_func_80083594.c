// STCRDDEK:0x80083594 (size 68, 0x44)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x8e4 (RAW, no header).
// Boundary: reviewed inventory body-80083594 (start 2276, end 2344, 68 B); PAL jr ra
// plus delay slot end; next framed STCRDDEK:0x800835d8 follows contiguously.
// Ghidra program STCRDDEK (project ddw3-pal-sles-03936) read-only: disasm 17 words
// matches PAL word-for-word; decompile: v0 = func_0x80014504(STCRDDEK_func_800834e0,
// 0x68, 0); *(v0 + 0x64) = STCRDDEK_func_80083314; *(v0 + 0x50) = 0x1000;
// *(v0 + 0x54) = 0; void return. No Ghidra state change.
// Callers: 1 direct jal from STCRDDEK 0x80089e2c (UNCONDITIONAL_CALL, x-ref read-only).
// Callees: 1 direct cross-module jal to EXE 0x80014504 (a0=0x800834e0 callback-arg,
// a1=0x68, a2=0). Stored words 0x80083314/0x800834e0 confirmed as in-overlay code
// pointers; owning struct layout and allocator arg roles left unrecovered.
// Pattern matches sibling allocator-wrappers (STCRDDEK 0x80088ac8, STSTATUS 0x80084f10).
// stcrddek.s is GUIDE only; never copied as source.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
// Status: C_MATCHING pending exact_byte_match (portable C, no asm, no register variables).
#include "common/types.h"

extern void *F0x80014504(int32_t a0, int32_t a1, int32_t a2);
extern void STCRDDEK_func_800834e0(void);
extern void STCRDDEK_func_80083314(void);

void STCRDDEK_func_80083594(void)
{
    void *v0;

    v0 = F0x80014504((int32_t)STCRDDEK_func_800834e0, 0x68, 0);
    *(void **)((uint8_t *)v0 + 0x64) = (void *)STCRDDEK_func_80083314;
    *(int32_t *)((uint8_t *)v0 + 0x50) = 0x1000;
    *(int32_t *)((uint8_t *)v0 + 0x54) = 0;
}
