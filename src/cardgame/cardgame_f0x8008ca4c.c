// CARDGAME:0x8008ca4c (size 272, 0x110)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x9d9c (RAW, no header)
// Boundary: prologue 27bdffd8 addiu sp,-0x28, saves s2/s0/s1/ra at 0x20/0x18/0x1c/0x24(sp);
// s2=a0, s0=a1, s1=a2; epilogue lw ra/s2/s1/s0 + jr ra + addiu sp,+0x28 at 0x8008cb44-0x8008cb58.
// Next framed CARDGAME:0x8008cb5c at +0x110 (27bdffc8 prologue), size 0x110 contiguous, no overlap.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm 68 words matches PAL;
// decompile: 6 indirect jalr via table in p2 (offsets 0xecc, 0xeb8, 0xf14, 0xf3c, 0xf24, 0xec8),
// field stores at p1+0x440/0x422/0x438, halfword select via p1[0x575]*8+0x580, halfword clear at
// p2+0x594, 5-arg call (5th arg 0x1000 spilled at sp+0x10), 15-byte descending clear at p1+0x46f..0x47d.
// cardgame.s is GUIDE only; never copied as source. No Ghidra state change.
// Callers: 19 direct jal from CARDGAME_F0x80084320 (0x80084bc4..0x80084d74 step 0x18, a2 varies:
// 0, 0x4000, ...); 0 direct callees, 6 indirect (table offsets above, loaded from s0=p2).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base), exact_byte_match, no alternates needed.
// Status: C_MATCHING (portable C, no asm, no explicit register variables).
// sha256 532f82e22ce7e7b11e1f47935b8501cc12cf659071149c9d2c19868ff7af0d1d (272B).
// Loop note: indexed for-loop recomputes the address each iteration (1 word short); the explicit
// index+pointer do-while (p = p1 + i) reproduces the hoisted addu v1,s2,a0 and top-store shape.
#include <stdint.h>

typedef void (*cardgame_8ca4c_cb1_t)(int32_t);
typedef void (*cardgame_8ca4c_cb2_t)(int32_t, int32_t);
typedef void (*cardgame_8ca4c_cb3_t)(int32_t, int32_t, int32_t);
typedef void (*cardgame_8ca4c_cb4_t)(int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_8ca4c_cb5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);

void CARDGAME_F0x8008ca4c(int32_t p1, int32_t p2, int32_t p3)
{
    int i;
    int p;

    ((cardgame_8ca4c_cb1_t)*(uint32_t *)(p2 + 0xecc))(p2);
    ((cardgame_8ca4c_cb2_t)*(uint32_t *)(p2 + 0xeb8))(p2, p3);
    *(uint32_t *)(p1 + 0x440) = 0;
    *(uint8_t *)(p1 + 0x422) = 1;
    *(uint32_t *)(p1 + 0x438) = p3;
    ((cardgame_8ca4c_cb4_t)*(uint32_t *)(p2 + 0xf14))(p2, 0xf, 0xe500, 0x6100);
    ((cardgame_8ca4c_cb3_t)*(uint32_t *)(p2 + 0xf3c))(p2, 0xf, *(int16_t *)(p1 + (((int8_t *)p1)[0x575] << 3) + 0x580));
    *(uint16_t *)(p2 + 0x594) = 0;
    ((cardgame_8ca4c_cb5_t)*(uint32_t *)(p2 + 0xf24))(p2, 0xf, 8, 0x1000, 0x1000);
    ((cardgame_8ca4c_cb1_t)*(uint32_t *)(p2 + 0xec8))(p2);
    *(uint8_t *)(p1 + 0x49d) = 2;
    *(uint8_t *)(p1 + 0x4ad) = 0;
    i = 0xe;
    p = p1 + i;
    do {
        *(uint8_t *)(p + 0x46f) = 0;
        p--;
        i--;
    } while (i >= 0);
    *(uint8_t *)(p1 + 0x499) = 1;
}
