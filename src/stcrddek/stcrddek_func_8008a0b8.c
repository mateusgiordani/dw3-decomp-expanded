#include "exe/exe.h"

/* STCRDDEK:0x8008a0b8 (72B, file-off 0x7408, base 0x80082cb0): allocate a
 * 0x120-byte object through the EXE allocator F0x80014504 tagged with
 * STCRDDEK_func_80089e98 (the allocator stores the tag at obj+0x48, slot
 * 18 -- the same tag role as FIELDSTG_F0x800872e4 in FIELDSTG_F0x800873d8),
 * then publish STCRDDEK_func_8008924c at obj+0x11c and the constants
 * 0x1000 / 7 at obj+0x50 / obj+0x54. Returns the fresh object (still in
 * $v0 at jr $ra).
 * PAL: reference/extracted/pro/stcrddek.bin; evidence body-8008a0b8 bytes
 * match exactly. Straight-line 18 words, single jal 0x80014504, jr $ra end.
 * Direct caller 0x800832b4 (inside STCRDDEK_func_800831f0; upstream ddw3
 * stcrddek.s line 392 confirms the jal word 0x0c02282e).
 * STCRDDEK_func_80089e98 and STCRDDEK_func_8008924c are real code (MIPS
 * prologues 27bdffe0 / 27bdff70 at their first words); 0x8008924c is also
 * called directly from 0x800893a8, while 0x80089e98 has no direct jal and
 * is passed tag-style. Callee semantics unconfirmed; names conservative.
 * C_MATCHING: fn_exact_pipeline.py exact_byte_match, base variant
 * (psyq-gcc-2.8.1-sn32-4.0.0010 -O2 -G0, aspsx-2.79), 72/72 bytes,
 * sha 0cc11d7d8128fdcf787cd2c8714fad592d6805b810489c3e54b63dcf897a7956.
 * Report: reports/handoffs/stcrddek-8008a0b8-c-recovery.md. */
extern int32_t F0x80014504(int32_t a0, int32_t a1, int32_t a2);
extern void STCRDDEK_func_80089e98(void);
extern void STCRDDEK_func_8008924c(void);

int32_t STCRDDEK_func_8008a0b8(void)
{
    int32_t obj;

    obj = F0x80014504((int32_t)STCRDDEK_func_80089e98, 0x120, 0x6c);
    *(int32_t *)(obj + 0x11c) = (int32_t)STCRDDEK_func_8008924c;
    *(int32_t *)(obj + 0x50) = 0x1000;
    *(int32_t *)(obj + 0x54) = 7;
    return obj;
}
