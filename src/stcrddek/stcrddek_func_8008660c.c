#include "exe/exe.h"

/* STCRDDEK:0x8008660c (88B, file-off 0x395c, base 0x80082cb0): allocate a
 * 0x494-byte object through the EXE allocator F0x80014504 tagged with
 * STCRDDEK_func_80086574 (callback-style tag stored by the allocator),
 * then publish the constants 0x1000 / 6 at obj+0x50/xtag+0x54/0x58 and
 * the two caller args at obj+0x50 / obj+0x5c. Returns the fresh object
 * (still in $v0 at jr $ra).
 * PAL: reference/extracted/pro/stcrddek.bin; 22 words at off 0x395c match
 * Ghidra disasm word-for-word (prologue 27bdffe0, jal 0c005141 to EXE
 * 0x80014504 with delay-slot li a2,0xd4, four sw stores, framed epilogue).
 * Boundary: entry + 88 = 0x80086664, which Ghidra reports as data
 * ("No instruction at address"); reviewed inventory places an UNKNOWN gap
 * 14772..15160 (file off) before the next CODE region at 0x800867e8.
 * Caller: single direct jal from STCRDDEK 0x80089dd0 (file off 0x7120);
 * its delay-slot/jump sequence stores $v0 to 0(s3), proving the pointer
 * return. Callee 0x80014504 is cross-module (EXE); no intra-overlay def.
 * STCRDDEK_func_80086574 is a real framed in-overlay function (prologue
 * 27bdffe0 at 0x80086574) passed tag-style. Names conservative.
 * Portable C only, no asm, no explicit register variables. */
extern int32_t F0x80014504(int32_t a0, int32_t a1, int32_t a2);
extern void STCRDDEK_func_80086574(void);

int32_t STCRDDEK_func_8008660c(int32_t arg0, int32_t arg1)
{
    int32_t v0;

    v0 = F0x80014504((int32_t)STCRDDEK_func_80086574, 0x494, 0xd4);
    *(int32_t *)(v0 + 0x54) = 0x1000;
    *(int32_t *)(v0 + 0x58) = 6;
    *(int32_t *)(v0 + 0x50) = arg0;
    *(int32_t *)(v0 + 0x5c) = arg1;
    return v0;
}
