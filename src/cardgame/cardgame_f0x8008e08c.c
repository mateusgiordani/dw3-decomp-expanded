/*
 * CARDGAME:0x8008e08c CARDGAME_F0x8008e08c
 * 780 bytes at CARDGAME.PRO offset 0xb3dc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008e08c
 *  Symbols     (none)
 *  Compare     780 bytes from 0x8008e08c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008e08c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Steps two per-card counters for the flagged cards (flags p1+0x46f, 12 slots:
 * 6 records of 14 bytes at p1+0x72e.., count p1+0x72c, and 6 at p1+0x7a0..,
 * count p1+0x79e). The first counter (+0x734/+0x7a6) moves while p1+0x424 <
 * p1+0x430, in the direction of p1+0x428, mirrored in p2+0x14b per slot (stride
 * 0x4c); the second (+0x736/+0x7a8) likewise with p1+0x434, p1+0x42c and
 * p2+0x14c. When neither runs, re-flags the slots whose second counter is
 * exhausted and returns 1.
 *
 * Matching note: the second-half offsets (i - 6) * 0xe are block locals so they
 * stay a separate induction value, as in PAL.
 */

#include <stdint.h>

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S8(p, o)     (*(int8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

int32_t CARDGAME_F0x8008e08c(int32_t p1, int32_t p2)
{
    int32_t ret;
    int32_t i;

    ret = 1;
    if (S32(p1, 0x424) < S32(p1, 0x430)) {
        for (i = 0; i < 12; i++) {
            if (S8(p1 + i, 0x46f) == 0)
                continue;
            if (i < 6) {
                if (i < U8(p1, 0x72c)) {
                    if (S32(p1, 0x428) > 0) {
                        if (S16(p1 + i * 0xe, 0x734) < 99) {
                            S16(p1 + i * 0xe, 0x734)++;
                            U8(p2 + i * 0x4c, 0x14b)++;
                        }
                    } else if (S16(p1 + i * 0xe, 0x734) > 0) {
                        S16(p1 + i * 0xe, 0x734)--;
                        U8(p2 + i * 0x4c, 0x14b)--;
                    }
                }
            } else if (i - 6 < U8(p1, 0x79e)) {
                if (S32(p1, 0x428) > 0) {
                    int32_t o = (i - 6) * 0xe;

                    if (S16(p1 + o, 0x7a6) < 99) {
                        S16(p1 + o, 0x7a6)++;
                        U8(p2 + i * 0x4c, 0x14b)++;
                    }
                } else {
                    int32_t o = (i - 6) * 0xe;

                    if (S16(p1 + o, 0x7a6) > 0) {
                        S16(p1 + o, 0x7a6)--;
                        U8(p2 + i * 0x4c, 0x14b)--;
                    }
                }
            }
        }
        ret = 0;
    }
    if (S32(p1, 0x424) < S32(p1, 0x434)) {
        for (i = 0; i < 12; i++) {
            if (S8(p1 + i, 0x46f) == 0)
                continue;
            if (i < 6) {
                if (i < U8(p1, 0x72c)) {
                    if (S32(p1, 0x42c) > 0) {
                        if (S16(p1 + i * 0xe, 0x736) < 99) {
                            S16(p1 + i * 0xe, 0x736)++;
                            U8(p2 + i * 0x4c, 0x14c)++;
                        }
                    } else if (S16(p1 + i * 0xe, 0x736) > 0) {
                        S16(p1 + i * 0xe, 0x736)--;
                        U8(p2 + i * 0x4c, 0x14c)--;
                    }
                }
            } else if (i - 6 < U8(p1, 0x79e)) {
                if (S32(p1, 0x42c) > 0) {
                    int32_t o = (i - 6) * 0xe;

                    if (S16(p1 + o, 0x7a8) < 99) {
                        S16(p1 + o, 0x7a8)++;
                        U8(p2 + i * 0x4c, 0x14c)++;
                    }
                } else {
                    int32_t o = (i - 6) * 0xe;

                    if (S16(p1 + o, 0x7a8) > 0) {
                        S16(p1 + o, 0x7a8)--;
                        U8(p2 + i * 0x4c, 0x14c)--;
                    }
                }
            }
        }
        ret = 0;
    }
    S32(p1, 0x424)++;
    if (ret != 0) {
        for (i = 0; i < 12; i++) {
            U8(p1 + i, 0x46f) = 0;
            if (i < 6) {
                if (i < U8(p1, 0x72c) && S16(p1 + i * 0xe, 0x736) <= 0)
                    U8(p1 + i, 0x46f) = 1;
            } else if (i - 6 < U8(p1, 0x79e)) {
                int32_t o = (i - 6) * 0xe;

                if (S16(p1 + o, 0x7a8) <= 0)
                    U8(p1 + i, 0x46f) = 1;
            }
        }
    }
    return ret;
}
