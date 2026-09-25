// CARDGAME:0x80088f0c (size 108, 0x6c)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x0625c
// Boundary: sweep reports/handoffs/cardgame-boundary-sweep.md #25; prologue 27bdffe0 (-0x20), epilogue jr ra + 27bd0020 (+0x20) at 0x80088f70/0x80088f74.
// Contiguity: prev CARDGAME:0x80087edc (0x1030) ends at 0x80088f0c; next CARDGAME:0x80088f78 at +0x6c (27bdffd8).
// Ghidra CARDGAME (ddw3-pal-sles-03936, read-only): disasm matches PAL word-for-word; decompile:
//   (**(code **)(param_2 + 0xecc))(param_2); *(u32 *)(param_1 + 0x440) = 0; *(u8 *)(param_1 + 0x422) = 1;
//   (**(code **)(param_2 + 0xec8))(param_2); bytes 0x49d/0x49c/0x499 = 1.
// Xref: 1 caller CARDGAME_F0x80084320 via 0x80084a24 UNCONDITIONAL_CALL; 0 direct callees (2 indirect jalr via s1+0xecc/0xec8).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base), exact_byte_match, no alternates needed.
// Status: C_MATCHING (portable C, no asm).
#include <stdint.h>

typedef void (*cardgame_cb_t)(int32_t);

void CARDGAME_F0x80088f0c(int32_t p1, int32_t p2)
{
    ((cardgame_cb_t)*(uint32_t *)(p2 + 0xecc))(p2);
    *(uint32_t *)(p1 + 0x440) = 0;
    *(uint8_t *)(p1 + 0x422) = 1;
    ((cardgame_cb_t)*(uint32_t *)(p2 + 0xec8))(p2);
    *(uint8_t *)(p1 + 0x49d) = 1;
    *(uint8_t *)(p1 + 0x49c) = 1;
    *(uint8_t *)(p1 + 0x499) = 1;
}
