/*
 * CARDGAME:0x8009403c CARDGAME_F0x8009403c
 * 672 bytes at CARDGAME.PRO offset 0x1138c (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float
 *  Variant     base
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x8009403c
 *  Symbols     (none)
 *  Compare     672 bytes from 0x8009403c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009403c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Next CARDGAME:0x800942dc at +0x2a0 (27bdffc8 prologue), size 0x2a0
 * contiguous, no overlap.
 *
 * C below written from disassembly). x-ref to 0x8009403c: 2 UNCONDITIONAL_CALL
 * from CARDGAME_F0x80084320 @ 0x8008446c (a2=0) and 0x80084484 (a2=1); graph
 * callers depth1 FUN_800a2df8; direct callees none (2 indirect jalr).
 *
 * No Ghidra state change.
 *
 * Clamped sums are compound "+=" with the stored value compared; the per-record
 * offset is i * 0xe so strength reduction creates the j cursor (PAL copies
 * after the entry test); the count limit reads through a value local t = p3 *
 * 0x72 (PAL addu p1,t) in both loops; the search walks its own pointer w and
 * the last loop has its own counter m. Details: strategy-r8-o55/attempts-r8.
 */

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