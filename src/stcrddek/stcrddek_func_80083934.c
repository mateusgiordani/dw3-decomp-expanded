#include "common/types.h"

/* STCRDDEK:0x80083934 (72B, 18 words, file-off 0xC84 = vaddr - 0x80082cb0).
 * PAL bytes (authority): reference/extracted/pro/stcrddek.bin at off 0xC84,
 * 18 LE words 27bdffe8..27bd0018, all equal to Ghidra STCRDDEK disasm words.
 * Boundary: Ghidra function list entry STCRDDEK_func_80083934 size 72;
 * prologue addiu sp,sp,-0x18 + sw s0,0x10(sp) at 0x80083934, epilogue
 * lw ra,0x14(sp) + lw s0,0x10(sp) + jr ra + addiu sp,sp,0x18 at 0x8008396c;
 * next framed function STCRDDEK_func_8008397c starts immediately after
 * (addiu sp,sp,-0x30), no gap, no overlap. Prev function 0x800838c4+112
 * ends exactly at entry.
 * Semantics: v0 = F0x80014504(STCRDDEK_func_800838c4, 0x70, 0) (EXE allocator
 * / registrar shared by STCRD* entries; a2 = 0 via move); then constants
 * 0x1000 at v0+0x54, 5 at v0+0x58, caller arg (saved via s0) at v0+0x50.
 * v0 is preserved to jr ra, so the fresh object is returned. The arg word
 * type is unrecovered (stored opaquely); int32_t keeps width/sign explicit.
 * Callback STCRDDEK_func_800838c4 dispatches on *(arg+0xC): 1 -> 0x800837b4
 * then 0x800835d8; 2 -> 0x800835d8; 3 -> return; else indirect call via
 * slot at arg+0x38. Caller: single direct jal from STCRDDEK 0x800852c0
 * (a0 = s2, v0 saved to 0xCC(s3)); upstream stcrddek.s .L0x00002610 jal
 * 0x80083934 agrees (GUIDE ONLY, never copied as source).
 * Portable C only, no asm, no explicit register variables. Operation order
 * (0x54, 0x58, 0x50) preserved for matching.
 * C_MATCHING: fn_exact_pipeline.py exact_byte_match, base variant
 * (psyq-gcc-2.8.1-sn32-4.0.0010 -O2 -G0, aspsx-2.79), 72/72 bytes,
 * sha 5cc68ebeddd0ef3d72c8e1a7d89af35c1e043e19055f6d9c3545493432fcc8df.
 * Report: reports/handoffs/stcrddek-80083934-c-recovery.md. */
extern int32_t F0x80014504(int32_t a0, int32_t a1, int32_t a2);
extern void STCRDDEK_func_800838c4(void);

int32_t STCRDDEK_func_80083934(int32_t arg0)
{
    int32_t obj;

    obj = F0x80014504((int32_t)STCRDDEK_func_800838c4, 0x70, 0);
    *(int32_t *)(obj + 0x54) = 0x1000;
    *(int32_t *)(obj + 0x58) = 5;
    *(int32_t *)(obj + 0x50) = arg0;
    return obj;
}
