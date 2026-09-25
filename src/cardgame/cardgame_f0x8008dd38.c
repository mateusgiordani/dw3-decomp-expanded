#include "common/types.h"

/* CARDGAME:0x8008dd38 (136B, 0x88) -- portable C recovery (rev 2).
 * PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xb088.
 * Boundary sweep reports/handoffs/cardgame-boundary-sweep.md #57: prologue
 * 27bdffd8 addiu sp,-0x28, epilogue 03e00008 jr ra + 27bd0028 addiu sp,+0x28,
 * frame -0x28/+0x28; prev CARDGAME:0x8008dc70 size 0xc8 ends exactly at
 * 0x8008dd38, next CARDGAME:0x8008ddc0 at +0x88 gap 0 (contiguous).
 * Ghidra ddw3-pal-sles-03936/CARDGAME read-only: disasm 34 insns matches PAL
 * words; decompile bool (param_1, param_2, param_3, param_4) with
 * t = param_4 (+6 if param_3 != 0), gate on CARDGAME_F0x8008db7c, then
 * CARDGAME_F0x8008dc70, return != 0. x-ref to: 1 caller
 * CARDGAME_F0x800915d4 @ 0x800916cc (jal 0x8008dd38, delay move a0,s0, return
 * checked beqz v0). x-ref from: 2 callees 0x8008db7c, 0x8008dc70.
 * Caller CARDGAME_F0x800915d4 passes (param_1, param_2, *(+0x42c), *(+0x430))
 * when byte@+0x422 == 2 and branch arg != 0; twin sibling CARDGAME:0x8008daf4
 * shares this shape with callees 0x8008d938/0x8008da2c.
 * Supersedes rev-1 __asm__ noreorder wrapper (ASM_MATCHING evidence preserved
 * in handoff); this portable C is the matching source.
 * Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 base,
 * exact_byte_match, 2 relocations (jals), body 136 padding 0. */
extern int32_t CARDGAME_F0x8008db7c(int32_t a0, int32_t a1, int32_t a2);
extern void CARDGAME_F0x8008dc70(int32_t a0, int32_t a1, int32_t a2, int32_t a3);

int32_t CARDGAME_F0x8008dd38(int32_t a0, int32_t a1, int32_t a2, int32_t a3)
{
    int32_t ret = 0;
    int32_t t = a3;

    if (a2 != 0)
        t = a3 + 6;
    if (CARDGAME_F0x8008db7c(a0, a1, t) != 0) {
        CARDGAME_F0x8008dc70(a0, a1, a2, a3);
        ret = 1;
    }
    return ret;
}
