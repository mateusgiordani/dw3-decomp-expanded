/*
 * CARDGAME:0x80091eb0 CARDGAME_F0x80091eb0
 * 744 bytes at CARDGAME.PRO offset 0xf200 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80091eb0
 *  Symbols     CARDGAME_F0x8009170c=0x8009170c CARDGAME_F0x80091eb0=0x80091eb0
 *  Compare     744 bytes from 0x80091eb0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80091eb0
 */

#include "common/types.h"

/*
 * Scope is exactly one framed function: prologue 27bdffc8 addiu sp,-0x38 at
 * 0x80091eb0; epilogue 03e00008 jr ra + 27bd0038 addiu sp,+0x38 at
 * 0x80092190/0x80092194; next prologue c8ffbd27 addiu sp,-0x38 at 0x80092198 =
 * +0x2E8. Size 744 contiguous (186 words), no overlap.
 *
 * Signature is conservative: p1/p2 are byte bases with raw offsets (no invented
 * struct); sel/slot/found are word-sized ints; halfword at dst+2 is signed
 * (lh). Callers pass (s1,s0,s2) so all three params are word handles.
 */

extern void CARDGAME_F0x8009170c(uint8_t *p1, uint8_t *p2, int32_t sel, int32_t slot);

typedef void (*cardgame_cb4_t)(uint8_t *p2, int32_t slot, int32_t kind, int32_t arg);
typedef void (*cardgame_cb3_t)(uint8_t *p2, int32_t slot, int32_t arg);
typedef void (*cardgame_cb5_t)(uint8_t *p2, int32_t slot, int32_t a2, int32_t a3, int32_t a4);

typedef struct { uint32_t w0, w1, w2; int16_t h; } __attribute__((packed, aligned(2))) CardRec14;

typedef struct { uint32_t w0, w1, w2, w3; } CardQW16;

typedef struct { uint32_t w0, w1, w2; } CardW12;

typedef struct { uint8_t prefix[0x146]; uint8_t flags[0]; } CardFlagView;

void CARDGAME_F0x80091eb0(uint8_t *p1, uint8_t *p2, int32_t sel) {
    uint8_t *t1;
    uint32_t orig;
    int32_t slot;
    int32_t found;
    int32_t i;
    int32_t off1;
    int32_t off2;
    uint8_t *dst;
    uint8_t *src;
    int16_t card;
    cardgame_cb4_t cb14;
    cardgame_cb3_t cb3c;
    uint16_t *slotbase;
    uint8_t *q;

    t1 = p1 + (sel * 114 + 0x72C);
    orig = t1[0];
    slot = (int32_t)orig;
    if (sel != 0) {
        slot = (int32_t)(orig + 6);
    }

    found = 0;
    src = t1 + (orig * 14 + 2);
    for (i = 0, off2 = 0x74c, off1 = 0x72e; i < 12; off2 += 14, i++, off1 += 14) {
        if (i < 6) {
            if (i >= (int32_t)p1[0x72C]) continue;
            src = p1 + off1;
        } else {
            if ((i - 6) >= (int32_t)p1[0x79E]) continue;
            src = p1 + off2;
        }
        if (((int8_t *)(p1 + i))[0x46F] != 0) {
            found = i;
            break;
        }
    }

    dst = t1 + orig * 14;
    *(CardRec14 *)(dst + 2) = *(const CardRec14 *)src;

    cb14 = *(cardgame_cb4_t *)(p2 + 0xF14);
    cb14(p2, slot, 0xE500, 0x6100);
    card = *(int16_t *)(dst + 2);
    cb3c = *(cardgame_cb3_t *)(p2 + 0xF3C);
    cb3c(p2, slot, (int32_t)card);

    {
        uint32_t *d = (uint32_t *)(p2 + slot * 76 + 0x108);
        uint32_t *s = (uint32_t *)(p2 + found * 76 + 0x108);
        uint32_t *e = s + 16;
        do {
            *(CardQW16 *)d = *(const CardQW16 *)s;
            d += 4;
            s += 4;
        } while (s != e);
        *(CardW12 *)d = *(const CardW12 *)s;
    }
    slotbase = (uint16_t *)(p2 + slot * 76 + 0x120);
    *slotbase = 0;

    CARDGAME_F0x8009170c(p1, p2, sel, slot);

    i = 0;
    if ((int32_t)((int8_t)p1[0x575]) - 1 > 0) {
        do {
            q = p1 + i * 8;
            if (q[0x585] == 0) {
                if (((CardFlagView *)p2)->flags[i + found * 76] != 0) {
                    ((CardFlagView *)p2)->flags[i + slot * 76] = 1;
                }
            }
            i++;
        } while (i < (int32_t)((int8_t)p1[0x575]) - 1);
    }

    p1[0x422] = 1;
    (*(cardgame_cb5_t *)(p2 + 0xF24))(p2, found, 5, 0, 0x1000);
    *(uint32_t *)(p1 + 0x424) = 7;
}
