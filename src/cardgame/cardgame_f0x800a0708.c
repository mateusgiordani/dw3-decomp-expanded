// CARDGAME:0x800a0708 (size 76, 0x4c) -- C_MATCHING (portable C, rev2)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1da58
// Rev1 was ASM_MATCHING noreorder stub (exact bytes via embedded asm); rev2
// recovers portable C per cardgame-human-v1 campaign (RULES R2/R3: asm stubs
// do not count as C). Provenance preserved: boundary sweep row #196, PAL word
// decode, Ghidra CARDGAME disasm/decompile/xrefs (see handoff).
// Body: addr = a1*0x72 + 0x72c; p = (uint8_t *)(a0 + addr); old = *p;
//   *p = old + 1; CARDGAME_F0x800a0628(a0, a1, a2, old);
// Order matters for matching: constant 0x72c is added to a1*0x72 BEFORE a0
// (addiu v0,0x72c; addu v0,a0,v0). Folding 0x72c into lbu/sb offset (v1 trial)
// yields 72 B; adding a0 first (v2 trial) swaps two words (diff 8 at off 24).
// The sb executes in the jal delay slot; lbu/andi/addiu sequence preserved.
// Caller: CARDGAME:0x800a0754 @0x800a07f8 (jal, a0=s3/a1=s4/a2=lh 0x64(s1));
//   plus DATA xref at 0x800a3298. Callee CARDGAME:0x800a0628 takes
//   (a0,a1,a2,a3=old); a2 passes through (caller-supplied lh).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base),
//   exact_byte_match, no alternates needed.
#include <stdint.h>

extern void CARDGAME_F0x800a0628(int32_t a0, int32_t a1, int32_t a2, uint32_t a3);

void CARDGAME_F0x800a0708(int32_t a0, int32_t a1, int32_t a2)
{
    int32_t addr;
    uint8_t *p;
    uint8_t old;

    addr = a1 * 0x72 + 0x72c;
    p = (uint8_t *)(a0 + addr);
    old = *p;
    *p = (uint8_t)(old + 1);
    CARDGAME_F0x800a0628(a0, a1, a2, old);
}
