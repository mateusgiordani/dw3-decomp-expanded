/*
 * CARDGAME:0x800a0cd4 CARDGAME_F0x800a0cd4
 * 632 bytes at CARDGAME.PRO offset 0x1e024 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a0cd4
 *  Symbols     EXE_F0x8001ebf8=0x8001ebf8
 *  Compare     632 bytes from 0x800a0cd4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a0cd4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0xb0 (s0-s8, ra).
 *
 * Looks for a run of equal cards on one side: collects (slot, card id) pairs of
 * the side's records (count at p1+side*0x72+0x72c, 14-byte records at +0x72e),
 * sorts them by id, then from index `start` asks the lookup callback about each
 * id and counts consecutive equal ids whose record has a non-zero value at
 * +0xa. A run longer than two marks its slots in p1+0x446, stores the value in
 * p1+0x438 and returns the index after the run; -1 else.
 *
 * Matching notes: j is both the sort's inner index and the run length (one
 * saved register, as in PAL); the clear loop has its own index k.
 */

#include <stdint.h>

struct Lookup {
    uint8_t *rec;
    int32_t pad0[10];
    void (*fn)(int32_t);
    int32_t pad1[10];
};

struct Pair {
    int16_t slot;
    int16_t id;
};

void EXE_F0x8001ebf8(struct Lookup *out);

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

int32_t CARDGAME_F0x800a0cd4(int32_t p1, int32_t side, int32_t start)
{
    struct Pair pairs[6];
    struct Pair t;
    struct Lookup info;
    int32_t ret;
    int32_t best;
    int32_t base;
    int32_t n;
    int32_t i;
    int32_t k;
    int32_t j;

    ret = -1;
    best = 0x51;
    base = side * 0x72;
    n = U8(p1 + base, 0x72c);
    EXE_F0x8001ebf8(&info);
    for (i = 0; i < n; i++) {
        int32_t o = base + i * 0xe;
        int32_t c = S16(p1 + o, 0x72e) * 2;

        pairs[i].id = S16(p1 + c, 0x50);
        pairs[i].slot = i;
    }
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (pairs[i].id > pairs[j].id) {
                t = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = t;
            }
        }
    }
    j = 0;
    for (i = start; i < n - 1; i++) {
        int32_t v;

        info.fn(pairs[i].id + 1);
        v = S16(info.rec, 0xa);
        if (v != 0 && pairs[i].id == pairs[i + 1].id) {
            best = v;
            j++;
        } else {
            if (j >= 2)
                goto found;
            j = 0;
        }
    }
    if (j >= 2) {
    found:
        for (k = 0; k < n; k++)
            U8(p1 + k, 0x446) = 0;
        for (; j >= 0; j--)
            U8(p1 + pairs[i - j].slot, 0x446) = 1;
        S32(p1, 0x438) = best;
        ret = i + 1;
    }
    return ret;
}
