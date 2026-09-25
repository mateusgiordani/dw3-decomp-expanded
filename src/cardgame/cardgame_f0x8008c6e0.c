// CARDGAME:0x8008c6e0 (size 504, 0x1f8)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x9a30
// Boundary: prologue 27bdff70 addiu sp,-0x90, saves s5/s8/ra/s7/s6/s4/s3/s2/s1/s0
// (s5=a0 struct, s8=a2 mode); epilogue lw ra/s8/s7/s6/s5/s4/s3/s2/s1/s0 + jr ra +
// addiu sp,+0x90 at 0x8008c8a8-0x8008c8d4. Next CARDGAME:0x8008c8d8 at +0x1f8,
// size 0x1f8 contiguous, no overlap. sha256 of range:
// 550785ef3386a8714c15e0c87d19219949913730d60971e7c213f234fcfe97cb.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: first 16 words
// match PAL; two range-checked switches (kind-5 in 0..10) via jump tables at
// 0x8008349c (outer, count select) and 0x800834cc (inner, card select); each table
// is 11 words followed by a zero word (.align 3 pad); cardgame.s is GUIDE only
// (no 0x8008c6e0 label there).
// Callers: 3 direct jal from CARDGAME_F0x80084320 (0x80085c3c/0x80085c74/0x80085cac;
// a1=s0 pointer, a2=0/1 mode in delay slot). Callees: jal EXE:0x8001ebf8
// (stack-buffer fill, same helper as CARDGAME:0x80085fd0; writes record pointer at
// buf+0x0 and callback EXE:0x8001e7ec at buf+0x2c) + 1 indirect jalr via buf[11]
// with arg (record halfword at p1+off+0x50)+1. a1 is passed but never read in
// 0x8008c6e0-0x8008c8d4.
// Semantics: loop over count entries; skip entries with flag byte 0 at
// p1[i+0x446] (signed char, lb); per entry pick card id from the kind-selected
// halfword table (cursor p1+2*i), resolve its record, keep the index with max
// (mode 0, last-greatest wins ties) or min (mode != 0, first-smallest wins)
// record halfword at +8; first valid entry seeds best (best == -1 path).
// Result stored at *(p1+0x440).
// Case arms are ordered 5,7,11/13,9,15 in both switches (11/13 before 9),
// matching PAL arm code positions 0x8008c74c/58/64/70/7c and 0x8008c7e8/f4/00/0c/18.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
#include <stdint.h>

typedef void (*cardgame_8c6e0_cb_t)(int32_t);

extern void F0x8001ebf8(void *);

void CARDGAME_F0x8008c6e0(char *p1, int32_t p2, int32_t p3)
{
    int32_t buf[22];
    int32_t bestrec;
    int32_t card;
    int32_t best;
    int32_t i;
    int32_t off;
    int16_t h;
    cardgame_8c6e0_cb_t cb;
    int32_t rec;
    int32_t count = 0;

    (void)p2;
    switch (*(int32_t *)(p1 + 0x438)) {
    case 5:
        count = *(int16_t *)(p1 + 0x5a6);
        break;
    case 7:
        count = *(int16_t *)(p1 + 0x5a4);
        break;
    case 11:
    case 13:
        count = *(int16_t *)(p1 + 0x66e);
        break;
    case 9:
        count = *(int16_t *)(p1 + 0x5a2);
        break;
    case 15:
        count = *(int16_t *)(p1 + 0x66a);
    default:
        break;
    }
    bestrec = 0;
    card = bestrec;
    F0x8001ebf8(buf);
    best = -1;
    i = bestrec;
    if (count > 0) {
        do {
            if (*(signed char *)(p1 + i + 0x446) != 0) {
                switch (*(int32_t *)(p1 + 0x438)) {
                case 5:
                    card = *(int16_t *)(p1 + i * 2 + 0x600);
                    break;
                case 7:
                    card = *(int16_t *)(p1 + i * 2 + 0x5b0);
                    break;
                case 11:
                case 13:
                    card = *(int16_t *)(p1 + i * 2 + 0x6c8);
                    break;
                case 9:
                    card = *(int16_t *)(p1 + i * 2 + 0x614);
                    break;
                case 15:
                    card = *(int16_t *)(p1 + i * 2 + 0x6dc);
                default:
                    break;
                }
                off = card << 1;
                h = *(int16_t *)((char *)p1 + off + 0x50);
                cb = (cardgame_8c6e0_cb_t)buf[11];
                cb(h + 1);
                rec = buf[0];
                if (best == -1) {
                    best = i;
                    bestrec = rec;
                } else if (p3 == 0) {
                    if (*(int16_t *)((char *)rec + 8) >= *(int16_t *)((char *)bestrec + 8)) {
                        best = i;
                        bestrec = rec;
                    }
                } else if (*(int16_t *)((char *)rec + 8) < *(int16_t *)((char *)bestrec + 8)) {
                    best = i;
                    bestrec = rec;
                }
            }
            i++;
        } while (i < count);
        *(int32_t *)(p1 + 0x440) = best;
    } else {
        *(int32_t *)(p1 + 0x440) = best;
    }
}
