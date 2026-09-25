#include "common/types.h"

/* CARDGAME:0x80092ba4 (size 176, 0xb0)
 * PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xfef4
 * Prologue 27bdffe0 addiu sp,-0x20 ; sw s1/s0/ra ; move s1,a0 ; move s0,a1
 * Body: *(p1+0x5a6)=0, *(p1+0x66e)=0, then 4x indirect jalr via *(p2+0xea0):
 *   (p2,0,6,lh(p1+0x5a6)), (p2,1,6,lh(p1+0x66e)),
 *   (p2,0,5,lh(p1+0x5a4)), (p2,1,5,lh(p1+0x66c));
 *   then *(p1+0x440)=0 (word), *(p1+0x423)=1 (byte), return 1.
 * Epilogue lw s0/s1/ra + jr ra + addiu sp,+0x20 at 0x80092c40/0x80092c50;
 * next CARDGAME:0x80092c54 prologue confirms size 0xb0 contiguous.
 * Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm 44
 * insns verified word-equal against PAL bytes; decompile CARDGAME_F0x80092ba4;
 * x-ref to from CARDGAME_F0x80084320 at 0x800843e0 (unconditional call).
 * Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base),
 * portable C, no register variables.
 * Status: C_MATCHING (exact_byte_match, 176/176 bytes, 0 differences; v2: early
 * result init removes the duplicated trailing li v0,1).
 */

typedef void (*CardEa0Fn)(int32_t, int32_t, int32_t, int32_t);

int32_t CARDGAME_F0x80092ba4(int32_t p1, int32_t p2)
{
    int32_t result;

    *(int16_t *)(p1 + 0x5a6) = 0;
    *(int16_t *)(p1 + 0x66e) = 0;
    ((CardEa0Fn)*(void **)(p2 + 0xea0))(p2, 0, 6, *(int16_t *)(p1 + 0x5a6));
    ((CardEa0Fn)*(void **)(p2 + 0xea0))(p2, 1, 6, *(int16_t *)(p1 + 0x66e));
    ((CardEa0Fn)*(void **)(p2 + 0xea0))(p2, 0, 5, *(int16_t *)(p1 + 0x5a4));
    ((CardEa0Fn)*(void **)(p2 + 0xea0))(p2, 1, 5, *(int16_t *)(p1 + 0x66c));
    result = 1;
    *(int32_t *)(p1 + 0x440) = 0;
    *(uint8_t *)(p1 + 0x423) = (uint8_t)result;
    return result;
}
