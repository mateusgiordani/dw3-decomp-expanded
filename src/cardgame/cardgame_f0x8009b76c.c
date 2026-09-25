// CARDGAME:0x8009b76c (size 292, 0x124)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x18abc
// Framed function: prologue 27bdff48 addiu sp,-0xb8 ; sw s0,0xb0(sp) ;
// move s0,a1 ; sw ra,0xb4(sp) ; epilogue lw ra,0xb4(sp) ; lw s0,0xb0(sp) ;
// jr ra ; addiu sp,+0xb8. Next function CARDGAME:0x8009b890 at +0x124
// confirms size 0x124 contiguous (prologue/prologue pair).
// Ghidra program CARDGAME read-only (project ddw3-pal-sles-03936, base 0x80082cb0):
// disasm 73 words match PAL word-for-word; decompile is a guarded 6-callback
// sequence through a stack work area (0xa0 bytes at sp+0x10) filled by EXE
// helper 0x8001f648; x-ref to 0x8009b76c: caller 0x8009bac8
// (CARDGAME_F0x8009bac8) UNCONDITIONAL_CALL, 40-record loop with stride 0x4c.
// Upstream cardgame.s GUIDE only, never copied.
// Source idiom follows matching sibling CARDGAME:0x8009af28 (same frame,
// same EXE-helper + DAT_80044f5c callback family, exact_byte_match on base):
// uint8_t buf[0xa0] with cast indirect calls, inline call arguments. The
// guard stays combined && (reproduces beq/li-delay/lbu/bne). There is no
// w6==0x10001000 interior guard here, so all six callbacks run.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
#include <stdint.h>

extern int32_t (*D_80044f5c)(int32_t a0);
extern void F0x8001f648(void *a0);

void CARDGAME_F0x8009b76c(int32_t p1, int32_t p2)
{
    uint8_t buf[0xa0];
    int32_t r;

    (void)p1;
    if (*(uint8_t *)(p2 + 0x45) != 0 && *(uint8_t *)(p2 + 0x42) == 11) {
        F0x8001f648(buf);
        ((void (*)(int32_t))*(void **)(buf + 0x88))((*(int32_t *)(p2 + 0x2c) / 2) % 5);
        ((void (*)(int32_t, int32_t))*(void **)(buf + 0x94))((*(int32_t *)p2 >> 8) + 0x14, (*(int32_t *)(p2 + 4) >> 8) + 0x17);
        ((void (*)(int32_t, int32_t, int32_t))*(void **)(buf + 0x8c))(*(int16_t *)(p2 + 0x18), *(int16_t *)(p2 + 0x1a), 0x1000);
        ((void (*)(int32_t, int32_t))*(void **)(buf + 0x7c))(0x100, 1);
        ((void (*)(int32_t, int32_t))*(void **)(buf + 0x74))(0x340, 0);
        r = D_80044f5c(0x25d0003);
        ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(buf + 0x84))(r, 9, *(int32_t *)p2 >> 8, *(int32_t *)(p2 + 4) >> 8);
    }
}
