// CARDGAME:0x8009403c (size 672, 0x2a0)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1138c
// Boundary: prologue 27bdffe0 addiu sp,-0x20, saves s0/s1/s2/ra at 0x10/0x14/0x18/0x1c(sp);
// s0=a0, s2=a1, s1=a2; epilogue lw ra/s2/s1/s0 + jr ra + addiu sp,+0x20 at 0x800942c4-0x800942d8.
// Next CARDGAME:0x800942dc at +0x2a0 (27bdffc8 prologue), size 0x2a0 contiguous, no overlap.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only, no mutation:
//   disasm 0x8009403c (168 words) matches PAL LE words byte-for-byte (prologue/epilogue pair);
//   decompile 0x8009403c -> void (int,int,int): first loop over count at p1+p3*0x72+0x72c
//   with 0xe stride accumulating shorts at +0x734/+0x736 into p1+0x42c/0x430 (clamp 99),
//   bonus +0x14 when 3 < n, two indirect jalr via *(p2+0xf14)/(p2+0xf3c), u8 search of
//   *(p1+0x244) against halfwords at p1+0xb2+0x50, byte/halfword stores at p2+0x64c/0x657/0x658/0x62c
//   and second flag loop setting bit 2 at slot stride 0x4c offset 0x150 (hypothesis only;
//   C below written from disassembly).
//   x-ref to 0x8009403c: 2 UNCONDITIONAL_CALL from CARDGAME_F0x80084320 @ 0x8008446c (a2=0)
//   and 0x80084484 (a2=1); graph callers depth1 FUN_800a2df8; direct callees none (2 indirect jalr).
// Upstream cardgame.s is GUIDE only; never copied as source. No Ghidra state change.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
// r8 (o55/s0923m): exact_byte_match 672/672 on psyq-gcc-2.8.1 base (receipt argv). Clamped sums
// are compound "+=" with the stored value compared; the per-record offset is i * 0xe so strength
// reduction creates the j cursor (PAL copies after the entry test); the count limit reads through a
// value local t = p3 * 0x72 (PAL addu p1,t) in both loops; the search walks its own pointer w and
// the last loop has its own counter m. Details: strategy-r8-o55/attempts-r8.
#include <stdint.h>

typedef void (*cardgame_9403c_cb4_t)(int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_9403c_cb3_t)(int32_t, int32_t, int32_t);

void CARDGAME_F0x8009403c(int32_t p1, int32_t p2, int32_t p3)
{
    int i;
    int n;
    int v;
    int addr;
    int limit;
    int target;
    int cand;
    int found;
    int off;
    int slot;
    int m;
    int t;
    int w;

    n = 0;
    *(int32_t *)(p1 + 0x42c) = 0;
    *(int32_t *)(p1 + 0x430) = 0;
    i = 0;
    for (; i < *(uint8_t *)(p1 + (t = p3 * 0x72) + 0x72c); i++) {
        if (*(int8_t *)(p1 + i + 0x446) != 0) {
            addr = p1 + (i * 0xe + p3 * 0x72);
            if ((*(int32_t *)(p1 + 0x42c) += *(int16_t *)(addr + 0x734)) > 99)
                *(int32_t *)(p1 + 0x42c) = 99;
            if ((*(int32_t *)(p1 + 0x430) += *(int16_t *)(addr + 0x736)) > 99)
                *(int32_t *)(p1 + 0x430) = 99;
            n++;
        }
    }
    if (n > 3) {
        if ((*(int32_t *)(p1 + 0x42c) += 0x14) > 99)
            *(int32_t *)(p1 + 0x42c) = 99;
        if ((*(int32_t *)(p1 + 0x430) += 0x14) > 99)
            *(int32_t *)(p1 + 0x430) = 99;
    }
    ((cardgame_9403c_cb4_t)*(uint32_t *)(p2 + 0xf14))(p2, 0x11, 0x8300, 0x6100);
    found = 0;
    v = *(uint8_t *)(p1 + 0x244);
    cand = 0x59;
    if (cand < v) {
        limit = v;
        target = *(int32_t *)(p1 + 0x438) - 1;
        for (w = p1 + 0xb2; cand < limit; cand++, w += 2) {
            if (*(int16_t *)(w + 0x50) == target) {
                found = cand;
                break;
            }
        }
    }
    if (found == 0) {
        found = 0x57;
        *(int32_t *)(p1 + 0x438) = 0x13b;
    }
    ((cardgame_9403c_cb3_t)*(uint32_t *)(p2 + 0xf3c))(p2, 0x11, found);
    *(int16_t *)(p2 + 0x64c) = 5;
    *(uint8_t *)(p2 + 0x657) = *(uint8_t *)(p1 + 0x42c);
    *(uint8_t *)(p2 + 0x658) = *(uint8_t *)(p1 + 0x430);
    off = 0;
    if (p3 != 0)
        off = 6;
    t = p3 * 0x72;
    for (m = 0; m < *(uint8_t *)(p1 + t + 0x72c); m++) {
        if (*(int8_t *)(p1 + m + 0x446) != 0) {
            slot = p2 + (off + m) * 0x4c;
            *(uint8_t *)(slot + 0x150) |= 4;
        }
    }
    *(int16_t *)(p2 + 0x62c) = 0;
    *(int32_t *)(p1 + 0x424) = 0;
    *(uint8_t *)(p1 + 0x422) = 1;
}