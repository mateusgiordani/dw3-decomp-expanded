/* STCRDDEK:0x800847c0 (size 208 = 0xD0, file-off 0x1b10 = vaddr - 0x80082cb0).
 * PAL bytes (authority): reference/extracted/pro/stcrddek.bin base 0x80082cb0,
 * file range [0x1b10, 0x1be0); head 00002821 3c028005 24478d34 00803021 ...,
 * tail ... 1440fff4 00851021 03e00008 00000000 (jr ra + nop).
 * Boundary: prev STCRDDEK ends 0x800847bc (jr ra + addiu sp,+0x80 epilogue);
 * entry 0x800847c0 (clear a1); body ends jr ra/nop at 0x80084888/0x8008488c;
 * next STCRDDEK:0x80084890 framed (addiu sp,sp,-0x120). Contiguous, no overlap.
 * function_labels.csv: STCRDDEK,0x800847c0,function,STCRDDEK_func_800847c0,208.
 * Ghidra STCRDDEK (project ddw3-pal-sles-03936, base 0x80082cb0) read-only:
 * disasm 52 insns word-matches PAL head/tail; decompile agrees (3 loops:
 * 0x13b init/copy, 0x28 decrement, 0x13b compact); no Ghidra state change.
 * Callers: 3 direct jal sites inside STCRDDEK_func_80085210
 * (0x800852b8 a0=s2, 0x8008587c, 0x800861c0), each passing a struct pointer.
 * Tables are absolute EXE-data addresses (lui 0x8005, -G0, no small data):
 * D0x80048d34 base crib, element read at [i+0x3a2] (true table 0x800490d6);
 * D0x80049372 row table, 0x66-byte rows of int16, row picked by *(arg0+0x5c).
 * Layout used (conservative, offsets only): +0x5c word selector, +0x7c word
 * survivor count, +0x88 int16 list (315 entries), +0x2fe int8 counts (315).
 * stcrddek.s is GUIDE only; never copied as source.
 * Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
 * Status: C_MATCHING (fn_exact_pipeline exact_byte_match, difference_count 0,
 * candidate_sha256 == reference_sha256 e7dcdf8e; base variant, no alternates
 * beyond the two ranked source-shape tries documented in the handoff). */
#include <stdint.h>

extern uint8_t D0x80048d34[];
extern uint8_t D0x80049372[];

void STCRDDEK_func_800847c0(uint8_t *arg0)
{
    int16_t *dst;
    int i;
    uint8_t *tbl;
    int16_t *row;
    int j;
    int k;
    int16_t *out;

    i = 0;
    tbl = D0x80048d34;
    dst = (int16_t *)arg0;
    do {
        dst[0x44] = 0;
        *(arg0 + i + 0x2FE) = tbl[i + 0x3A2];
        dst++;
        i++;
    } while (i < 0x13B);

    row = (int16_t *)(D0x80049372 + (*(int32_t *)(arg0 + 0x5C)) * 0x66);
    j = 0;
    do {
        j++;
        k = *row++;
        (*(arg0 + k + 0x2FE))--;
    } while (j < 0x28);

    *(int32_t *)(arg0 + 0x7C) = 0;
    i = 0;
    out = (int16_t *)arg0;
    do {
        if (*(int8_t *)(arg0 + i + 0x2FE) > 0) {
            out[0x44] = (int16_t)i;
            out++;
            (*(int32_t *)(arg0 + 0x7C))++;
        }
        i++;
    } while (i < 0x13B);
}
