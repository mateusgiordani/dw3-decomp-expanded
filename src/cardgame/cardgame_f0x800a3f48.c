/*
 * CARDGAME:0x800a3f48 CARDGAME_F0x800a3f48
 * 304 bytes at CARDGAME.PRO offset 0x21298 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a3f48
 *  Symbols     CARDGAME_F0x800a3e9c=0x800a3e9c EXE_F0x8001ebf8=0x8001ebf8
 *  Compare     304 bytes from 0x800a3f48 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a3f48
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x88 (s0-s5, ra).
 *
 * For each card of one side (count p1+side*0x72+0x72c, 14-byte records at
 * +0x72e) whose flag is set (p1+0x446, or p1+0x44c for side 1), asks the
 * EXE:0x8001ebf8 lookup callback about the card and clears the flag unless the
 * record's kind byte is 3; then calls CARDGAME_F0x800a3e9c.
 *
 * Matching note: the side offset is assigned to `o` in the loop condition, so
 * the count load adds p1 first without loop.c hoisting the multiply.
 */

#include <stdint.h>

struct Lookup {
    uint8_t *rec;
    int32_t pad0[10];
    void (*fn)(int32_t);
    int32_t pad1[10];
};

void EXE_F0x8001ebf8(struct Lookup *out);
int32_t CARDGAME_F0x800a3e9c(int32_t p1, int32_t side);

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))

void CARDGAME_F0x800a3f48(int32_t p1, int32_t side)
{
    struct Lookup info;
    int8_t *flags;
    int32_t i;
    int32_t o;

    EXE_F0x8001ebf8(&info);
    flags = (int8_t *)(p1 + 0x446);
    if (side == 1)
        flags = (int8_t *)(p1 + 0x44c);
    for (i = 0; o = side * 0x72, i < U8(p1 + o, 0x72c); i++) {
        if (flags[i] != 0) {
            int32_t c = S16(p1 + (side * 0x72 + i * 0xe), 0x72e) * 2;

            info.fn(S16(p1 + c, 0x50) + 1);
            if (info.rec[0] != 3)
                flags[i] = 0;
        }
    }
    CARDGAME_F0x800a3e9c(p1, side);
}
