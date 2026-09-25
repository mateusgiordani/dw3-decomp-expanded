// CARDGAME:0x8008bc8c (size 376, 0x178)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x8fdc
// Framed function: prologue 27bdffd8 addiu sp,-0x28, saves s0/s1/s2/ra at
// 0x18/0x1c/0x20/0x24(sp); s0=a0 (state), s1=a1 (table base), s2=a2 (step).
// Epilogue lw ra/s2/s1/s0 + jr ra + addiu sp,+0x28 at 0x8008bdec-0x8008be00.
// Next framed CARDGAME:0x8008be04 at +0x178, size 0x178 contiguous, no overlap.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm 94 words
// matches PAL; decompile shows EXE vector + 2x table callbacks (slots 0xed8/0xf0c).
// Callers: 2 direct jal from CARDGAME_F0x8008be04 (0x8008bf14 a2=-1, 0x8008bfb0 a2=+1).
// Callees: 0 direct; 5 indirect jalr (EXE vector *0x80055c48 + slots 0xed8/0xf0c x2).
// Idiom: (*EXEvec)(0x4001b); v=slot_ed8(f434, f43c); slot_f0c(p2, f43c, 5, v+0x1800, 0x6100);
// clear old entry (idx*76: byte+0x150 &= ~1, half+0x12e = 0); f43c += step;
// v=slot_ed8(f434, f43c); slot_f0c(p2, f43c, 1, v+0x1800, 0x5c00);
// set new entry (byte+0x150 |= 1, half+0x12e = 1).
// Callee *0x80055c48 is the EXE-resident shared vector (same lui v0,0x8005 /
// lw v0,0x5c48(v0) idiom as the 0x8009bf5c/0x8009c0dc family, arg 0x4001b here).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base), exact_byte_match,
// no alternates needed. Distinct byte-pointer temps per entry computation keep the
// base-first addu order (8d938/9c9a0 idiom) and let each address coalesce into v0;
// a single reused temp spills the entry to v1, an inline int address swaps the
// addu operands. Tail advances p2 itself (addu s1,s1,v0) for the final sh.
// sha256 6e1a3643d0d1a0732dea52afacb5461643b336778cb6ac0f7b595beeaab1d428 (376B).
// Status: C_MATCHING (portable C, no asm, no explicit register variables).
#include <stdint.h>

typedef void (*cardgame_vec_t)(int32_t);
typedef int32_t (*cardgame_cb2_t)(int32_t, int32_t);
typedef void (*cardgame_cb5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);

#define CARDGAME_VEC (*(cardgame_vec_t *)0x80055c48)

void CARDGAME_F0x8008bc8c(int32_t p1, int32_t p2, int32_t p3)
{
    int32_t v;
    uint8_t *t1, *t2, *t3;

    CARDGAME_VEC(0x4001b);
    v = ((cardgame_cb2_t)*(uint32_t *)(p2 + 0xed8))(*(int32_t *)(p1 + 0x434), *(int32_t *)(p1 + 0x43c));
    ((cardgame_cb5_t)*(uint32_t *)(p2 + 0xf0c))(p2, *(int32_t *)(p1 + 0x43c), 5, v + 0x1800, 0x6100);
    t1 = (uint8_t *)p2 + *(int32_t *)(p1 + 0x43c) * 76;
    t1[0x150] &= 0xfe;
    t2 = (uint8_t *)p2 + *(int32_t *)(p1 + 0x43c) * 76;
    *(uint16_t *)(t2 + 0x12e) = 0;
    *(int32_t *)(p1 + 0x43c) += p3;
    v = ((cardgame_cb2_t)*(uint32_t *)(p2 + 0xed8))(*(int32_t *)(p1 + 0x434), *(int32_t *)(p1 + 0x43c));
    ((cardgame_cb5_t)*(uint32_t *)(p2 + 0xf0c))(p2, *(int32_t *)(p1 + 0x43c), 1, v + 0x1800, 0x5c00);
    t3 = (uint8_t *)p2 + *(int32_t *)(p1 + 0x43c) * 76;
    t3[0x150] |= 1;
    p2 += *(int32_t *)(p1 + 0x43c) * 76;
    *(uint16_t *)(p2 + 0x12e) = 1;
}
