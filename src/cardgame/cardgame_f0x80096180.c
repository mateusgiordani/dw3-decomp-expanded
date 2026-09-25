// CARDGAME:0x80096180 (size 112, 0x70)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x134d0
// Framed function: prologue addiu sp,-0x20, epilogue jr ra / addiu sp,+0x20.
// Next CARDGAME:0x800961f0 at +0x70; prev CARDGAME_F0x80095fb4 ends at 0x80096144
// with a 0x3C gap holding leaf handlers at 0x80096144/0x80096150/0x80096160.
// Ghidra CARDGAME (read-only): decompile shows alloc via EXE 0x80014504 with
// tpl 0x80095fb4, size 0x74, flag 0, then pointer/field init; caller is
// CARDGAME_F0x8009f110+0x214 (xref 0x8009f324, UNCONDITIONAL_CALL).
// Gap leaves (read-only disasm): 0x80096144 stores halves at +0x54/+0x56,
// 0x80096150 sets byte +0x63=1 and word +0x50=0, 0x80096160 sets halves/bytes
// around +0x5a/+0x62/+0x64/+0x66. Stored here as handler function pointers.
// Portable C recovery (campaign cardgame-human-v1 rev 2): replaces the rev-1
// __asm__ noreorder wrapper, which is not portable C. Store order below
// follows PAL bytes: +0x68, +0x6c, +0x70, +0x54, +0x56, +0x63.
// Arg widths: PAL uses sh for both args, so only the low half is stored;
// signedness is unproven, int16_t is the conservative halfword reading.
#include <stdint.h>

extern void CARDGAME_F0x80095fb4(void);
extern void CARDGAME_F0x80096144(void);
extern void CARDGAME_F0x80096150(void);
extern void CARDGAME_F0x80096160(void);
extern void *EXE_F0x80014504(void *tpl, int32_t size, int32_t flag);

void CARDGAME_F0x80096180(int16_t a0, int16_t a1) {
    uint8_t *obj;

    obj = (uint8_t *)EXE_F0x80014504((void *)CARDGAME_F0x80095fb4, 0x74, 0);
    *(void **)(obj + 0x68) = (void *)CARDGAME_F0x80096144;
    *(void **)(obj + 0x6c) = (void *)CARDGAME_F0x80096160;
    *(void **)(obj + 0x70) = (void *)CARDGAME_F0x80096150;
    *(uint16_t *)(obj + 0x54) = (uint16_t)a0;
    *(uint16_t *)(obj + 0x56) = (uint16_t)a1;
    *(obj + 0x63) = 0;
}
