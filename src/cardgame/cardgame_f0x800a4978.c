/*
 * CARDGAME:0x800a4978 CARDGAME_F0x800a4978
 * 1148 bytes at CARDGAME.PRO offset 0x21cc8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a4978
 *  Symbols     CARDGAME_F0x80083e34=0x80083e34 CARDGAME_F0x80085f38=0x80085f38
 *              CARDGAME_F0x800a36e4=0x800a36e4 CARDGAME_F0x800a3934=0x800a3934
 *              CARDGAME_F0x800a44f0=0x800a44f0 CARDGAME_F0x800a45f8=0x800a45f8
 *              CARDGAME_F0x800a4924=0x800a4924 EXE_F0x8001ebf8=0x8001ebf8
 *  Compare     1148 bytes from 0x800a4978 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a4978
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x98 (s0-s8, ra).
 *
 * Picks a hand card to play. Clears the per-slot flags at p1+0x46f, then scans
 * the hand list (p1+0x6c8, count p1+0x66e) from the index returned by
 * CARDGAME_F0x800a4924; the chosen slot gets flag 1 and the function returns 1.
 *
 * The 0x58-byte local is filled by EXE:0x8001ebf8; its word 0 points to a
 * record whose byte 3 is a kind and its word 11 is a lookup callback.
 *
 * Matching notes: the card id is a block-local set once per loop (sched1 birth
 * boost puts the zero argument before the id load, as in PAL), and loop 2 has
 * its own index so c outranks i in global allocation (s0/s1).
 */

#include <stdint.h>

struct Lookup {
    uint8_t *rec;
    int32_t pad0[10];
    void (*fn)(int32_t);
    int32_t pad1[10];
};

int32_t CARDGAME_F0x80083e34(int32_t id, int32_t kind, int32_t index);
int32_t CARDGAME_F0x80085f38(int32_t p1, int32_t card);
int32_t CARDGAME_F0x800a36e4(int32_t p1, int32_t p2, int32_t value);
int32_t CARDGAME_F0x800a3934(int32_t p1, int32_t side, int32_t arg);
int32_t CARDGAME_F0x800a44f0(int32_t p1, int32_t p2, int32_t id);
int32_t CARDGAME_F0x800a45f8(int32_t p1, int32_t card, int32_t other, int32_t mode);
int32_t CARDGAME_F0x800a4924(int32_t p1, int32_t kind);
void EXE_F0x8001ebf8(struct Lookup *out);

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S8(p, o)     (*(int8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define COUNT        S16(p1, 0x66e)
#define HAND(i)      S16(p1 + (i) * 2, 0x6c8)
#define CARD_ID(c)   ({ int32_t o_ = (c) * 2; S16(p1 + o_, 0x50); })
#define SLOT_CARD    ({ int32_t k_ = S8(p1, 0x575) - 1; int32_t o_ = k_ * 8; S16(p1 + o_, 0x580); })

int32_t CARDGAME_F0x800a4978(int32_t p1, int32_t p2)
{
    struct Lookup info;
    int32_t ret;
    int32_t i;
    int32_t j;
    int32_t c;
    int32_t kind;
    int32_t t;

    ret = 0;
    for (i = 0; i < COUNT; i++)
        U8(p1 + i, 0x46f) = 0;
    if (S8(p1, 0x575) == 0) {
        int32_t a;
        int32_t b;

        if (U8(p1, 0x2f8) == 5) {
            c = CARDGAME_F0x800a4924(p1, 1);
            kind = 1;
        } else {
            c = CARDGAME_F0x800a4924(p1, 3);
            a = CARDGAME_F0x800a3934(p1, 0, 0);
            b = CARDGAME_F0x800a3934(p1, 1, 0);
            kind = 3;
            if ((U8(p1, 0x305) & 1) || a < b)
                return 0;
        }
        for (i = c; i < COUNT; i++) {
            int32_t k;
            int32_t o;

            c = HAND(i);
            k = c - 0x28;
            o = k * 4;
            if (U8(p1 + o, 0x35c) == kind && CARDGAME_F0x80085f38(p1, c) != 0) {
                int32_t id = CARD_ID(c);

                if (CARDGAME_F0x800a36e4(p1, p2, CARDGAME_F0x80083e34(id, 0, 0)) != 0
                    && CARDGAME_F0x800a44f0(p1, p2, id) != 0) {
                    U8(p1 + i, 0x46f) = 1;
                    ret = 1;
                    break;
                }
            }
        }
    } else {
        int32_t found = 0;

        for (i = 0; U8(p1 + i, 0x400) != 0xff; i++) {
            if (U8(p1 + i, 0x400) == CARD_ID(SLOT_CARD)) {
                found = 1;
                break;
            }
        }
        if (found) {
            for (j = CARDGAME_F0x800a4924(p1, 4); j < COUNT; j++) {
                c = HAND(j);
                if (CARDGAME_F0x80085f38(p1, c) != 0) {
                    int32_t id = CARD_ID(c);

                    if (CARDGAME_F0x800a36e4(p1, p2, CARDGAME_F0x80083e34(id, 0, 0)) != 0
                        && CARDGAME_F0x800a44f0(p1, p2, id) != 0) {
                        U8(p1 + j, 0x46f) = 1;
                        ret = 1;
                        break;
                    }
                }
            }
        }
        if (ret == 0) {
            uint8_t *rec;
            uint8_t *other;
            int32_t card;
            int32_t mode;

            EXE_F0x8001ebf8(&info);
            card = SLOT_CARD;
            info.fn(CARD_ID(card) + 1);
            rec = info.rec;
            if (rec[3] == 3 || rec[3] == 9) {
                i = CARDGAME_F0x800a4924(p1, 3);
                if (i < COUNT) {
                    t = kind = 3;
                    do {
                        int32_t k;
                        int32_t q;

                        {
                            int32_t o = i * 2;

                            c = S16(p1 + o, 0x6c8);
                        }
                        k = c - 0x28;
                        q = p1 + k * 4;
                        if ((U8(q, 0x35c) == kind || U8(q, 0x35c) == 4) && U8(q, 0x35d) != 0) {
                            info.fn(CARD_ID(c) + 1);
                            other = info.rec;
                            if (rec[3] == t) {
                                if (other[3] != 9)
                                    continue;
                                mode = 0;
                            } else {
                                if (other[3] != t)
                                    continue;
                                mode = 1;
                            }
                            if (CARDGAME_F0x80085f38(p1, c) != 0
                                && CARDGAME_F0x800a36e4(p1, p2, CARDGAME_F0x80083e34(CARD_ID(c), 0, 0)) != 0
                                && CARDGAME_F0x800a45f8(p1, card, c, mode) != 0) {
                                U8(p1 + i, 0x46f) = 1;
                                ret = 1;
                                break;
                            }
                        }
                    } while (++i < COUNT);
                }
            }
        }
    }
    return ret;
}
