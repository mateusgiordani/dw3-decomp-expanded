// CARDGAME:0x80097238 (size 216, 0xD8)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x14588
// Boundary: prologue 27bdffd0 addiu sp,-0x30 at 0x80097238, epilogue 03e00008 jr ra + 27bd0030 addiu sp,+0x30 at 0x80097308/0x8009730c.
// Next function CARDGAME:0x80097310 (addiu sp,-0x20) confirms size 0xD8 contiguous.
// Ghidra program CARDGAME read-only (project ddw3-pal-sles-03936, base 0x80082cb0): disasm 54 words, decompile + xrefs read-only, no cache mutation.
// Callers: CARDGAME_F0x80097508 -> jal 0x80097238 at 0x80097748 (UNCONDITIONAL_CALL). Callees: 3 indirect jalr per iteration (vtable at +0x134/+0x118/+0x148 from object at *(cursor+4)).
// Reference words compared with coordinator reference/extracted/pro/cardgame.bin @ off 0x14588 (54 words, Ghidra bytes match reference dump word-for-word).
// Exact toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 -fno-strength-reduce.
// The late local cursor initialization reproduces PAL prologue order while the
// pinned no-strength-reduce variant preserves the base+4 loads. Full 216-byte match.
#include <stdint.h>

typedef void (*cardgame_cb3_t)(int32_t, int32_t, int32_t);
typedef void (*cardgame_cb2_t)(int32_t, int32_t);

void CARDGAME_F0x80097238(int32_t arg0, int32_t *input, uint8_t *rec)
{
    uint32_t slots[2];
    int32_t i;
    int32_t k;
    int32_t *cursor;
    (void)arg0;
    i = 0;
    slots[0] = rec[0x14];
    k = 4;
    slots[1] = rec[0x15];
    cursor = input;
    do {
        int32_t off;
        int32_t a1v;
        int32_t a2v;
        int32_t o0;
        int32_t o1;
        int32_t o2;
        off = i << 2;
        i += 1;
        o0 = cursor[1];
        a1v = (int32_t)(int16_t)(*(uint16_t *)(rec + 0) + 0x67);
        a2v = (int32_t)(int16_t)(*(uint16_t *)(rec + 2) + k);
        ((cardgame_cb3_t)*(int32_t *)(o0 + 0x134))(o0, a1v, a2v);
        k += 13;
        o1 = cursor[1];
        ((cardgame_cb3_t)*(int32_t *)(o1 + 0x118))(o1, 0, *(int32_t *)((uint8_t *)slots + off));
        o2 = cursor[1];
        ((cardgame_cb2_t)*(int32_t *)(o2 + 0x148))(o2, 1);
        cursor += 1;
    } while (i < 2);
}
