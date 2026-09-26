/*
 * CARDGAME:0x800a4328 CARDGAME_F0x800a4328
 * 280 bytes at CARDGAME.PRO offset 0x21678 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a4328
 *  Symbols     EXE_F0x8001ebf8=0x8001ebf8
 *  Compare     280 bytes from 0x800a4328 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a4328
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x88 (s0-s6, ra).
 *
 * Among the flagged hand cards (flags p1+0x446, hand list p1+0x6c8, count
 * p1+0x66e) picks the one whose lookup record value at +8 is lowest (below 500)
 * and writes its hand entry to the byte at p1 + p1[0x575]*8 + 0x586.
 *
 * Returns 1 if any card was flagged.
 *
 * Matching note: found is cleared before the lookup setup call, which keeps it
 * in a saved register across the loop's callback calls, as in PAL.
 */

#include <stdint.h>

struct Lookup {
    uint8_t *rec;
    int32_t pad0[10];
    void (*fn)(int32_t);
    int32_t pad1[10];
};

void EXE_F0x8001ebf8(struct Lookup *out);

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S8(p, o)     (*(int8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

int32_t CARDGAME_F0x800a4328(int32_t p1)
{
    struct Lookup info;
    int8_t *flags;
    int32_t best;
    int32_t bi;
    int32_t found;
    int32_t i;

    best = 500;
    bi = 500;
    flags = (int8_t *)(p1 + 0x446);
    found = 0;
    EXE_F0x8001ebf8(&info);
    for (i = 0; i < S16(p1, 0x66e); i++) {
        if (flags[i] != 0) {
            int32_t c = S16(p1 + i * 2, 0x6c8) * 2;

            info.fn(S16(p1 + c, 0x50) + 1);
            found = 1;
            if (S16(info.rec, 8) < best) {
                best = S16(info.rec, 8);
                bi = i;
            }
        }
    }
    if (found == 1) {
        int32_t o = bi * 2;
        int32_t d = S8(p1, 0x575) * 8;

        U8(p1 + d, 0x586) = U8(p1 + o, 0x6c8);
    }
    return found;
}
