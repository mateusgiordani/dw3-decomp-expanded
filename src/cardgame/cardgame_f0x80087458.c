/* CARDGAME:0x80087458; parent recovery from PAL disassembly and read-only Ghidra.
 * Original delivery remains preserved in worker commit bcb142fb002ea2a72a9952f4d5f06adec1aae4b5.
 * Distinct branch-local row variables reproduce the PAL row/value registers
 * with GCC 2.8.1 / ASPSX 2.79, O2 G0 (full 340-byte match; revision 8). */
#include "common/types.h"























































#include <stdint.h>

extern void CARDGAME_F0x8008735c(int32_t a0, uint8_t *a1, int32_t a2, int32_t a3, int32_t a4);

int32_t CARDGAME_F0x80087458(int32_t p1, uint8_t *p2, int32_t p3) {
    int32_t idx;
    int32_t r;
    int32_t h;

    idx = *(int32_t *)(p1 + 0x43c);
    r = 0;
    if (*(int8_t *)(p1 + idx + 0x446) != 0) {
        uint8_t *row;
        if (*(uint8_t *)(p1 + 0x444) >= 6)
            goto out;
                h = *(int16_t *)(p3 + (idx << 1) + 100);
        CARDGAME_F0x8008735c(p1, p2, p3, 0, h);
        *(int8_t *)(p1 + *(int32_t *)(p1 + 0x43c) + 0x46f) = 1;
        ((void (*)(uint8_t *, int32_t))*(void **)(p2 + 0xf1c))(p2, *(int32_t *)(p1 + 0x43c));
        row = p2 + *(int32_t *)(p1 + 0x43c) * 76;
        row[0x150] |= 2;
        r = 1;
        *(uint8_t *)(p1 + 0x444) = *(uint8_t *)(p1 + 0x444) + r;
    } else {
        uint8_t *row;
        uint8_t *q;
        if (*(int8_t *)(p1 + idx + 0x46f) == 0)
            goto out;
        q = p2;
                h = *(int16_t *)(p3 + (idx << 1) + 100);
        CARDGAME_F0x8008735c(p1, q, p3, 1, h);
        *(int8_t *)(p1 + *(int32_t *)(p1 + 0x43c) + 0x46f) = 0;
        row = q + *(int32_t *)(p1 + 0x43c) * 76;
        row[0x150] &= 0xfd;
        r = 2;
        *(uint8_t *)(p1 + 0x444) = *(uint8_t *)(p1 + 0x444) - 1;
    }
out:
    return r;
}