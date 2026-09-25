/*
 * CARDGAME:0x8008bc8c CARDGAME_F0x8008bc8c
 * 376 bytes at CARDGAME.PRO offset 0x8fdc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008bc8c
 *  Symbols     CARDGAME_F0x8008be04=0x8008be04
 *  Compare     376 bytes from 0x8008bc8c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008bc8c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue 27bdffd8 addiu sp,-0x28, saves s0/s1/s2/ra at
 * 0x18/0x1c/0x20/0x24(sp); s0=a0 (state), s1=a1 (table base), s2=a2 (step).
 *
 * Epilogue lw ra/s2/s1/s0 + jr ra + addiu sp,+0x28 at 0x8008bdec-0x8008be00.
 *
 * Next framed CARDGAME:0x8008be04 at +0x178, size 0x178 contiguous, no overlap.
 *
 * Callers: 2 direct jal from CARDGAME_F0x8008be04 (0x8008bf14 a2=-1, 0x8008bfb0
 * a2=+1).
 *
 * Callees: 0 direct; 5 indirect jalr (EXE vector *0x80055c48 + slots
 * 0xed8/0xf0c x2).
 *
 * Idiom: (*EXEvec)(0x4001b); v=slot_ed8(f434, f43c); slot_f0c(p2, f43c, 5,
 * v+0x1800, 0x6100); clear old entry (idx*76: byte+0x150 &= ~1, half+0x12e =
 * 0); f43c += step; v=slot_ed8(f434, f43c); slot_f0c(p2, f43c, 1, v+0x1800,
 * 0x5c00); set new entry (byte+0x150 |= 1, half+0x12e = 1).
 *
 * Callee *0x80055c48 is the EXE-resident shared vector (same lui v0,0x8005 / lw
 * v0,0x5c48(v0) idiom as the 0x8009bf5c/0x8009c0dc family, arg 0x4001b here).
 *
 * sha256 6e1a3643d0d1a0732dea52afacb5461643b336778cb6ac0f7b595beeaab1d428
 * (376B).
 */

#include <stdint.h>

typedef void (*cardgame_vec_t)(int32_t);
typedef int32_t (*cardgame_cb2_t)(int32_t, int32_t);
typedef void (*cardgame_cb5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);

#define CARDGAME_VEC (*(cardgame_vec_t *)0x80055c48)

void CARDGAME_F0x8008bc8c(int32_t p1, int32_t p2, int32_t p3)
{
    int32_t v;
    uint8_t *t1, *t2, *t3;

    CARDGAME_VEC(0x4001b);
    v = ((cardgame_cb2_t)*(uint32_t *)(p2 + 0xed8))(*(int32_t *)(p1 + 0x434), *(int32_t *)(p1 + 0x43c));
    ((cardgame_cb5_t)*(uint32_t *)(p2 + 0xf0c))(p2, *(int32_t *)(p1 + 0x43c), 5, v + 0x1800, 0x6100);
    t1 = (uint8_t *)p2 + *(int32_t *)(p1 + 0x43c) * 76;
    t1[0x150] &= 0xfe;
    t2 = (uint8_t *)p2 + *(int32_t *)(p1 + 0x43c) * 76;
    *(uint16_t *)(t2 + 0x12e) = 0;
    *(int32_t *)(p1 + 0x43c) += p3;
    v = ((cardgame_cb2_t)*(uint32_t *)(p2 + 0xed8))(*(int32_t *)(p1 + 0x434), *(int32_t *)(p1 + 0x43c));
    ((cardgame_cb5_t)*(uint32_t *)(p2 + 0xf0c))(p2, *(int32_t *)(p1 + 0x43c), 1, v + 0x1800, 0x5c00);
    t3 = (uint8_t *)p2 + *(int32_t *)(p1 + 0x43c) * 76;
    t3[0x150] |= 1;
    p2 += *(int32_t *)(p1 + 0x43c) * 76;
    *(uint16_t *)(p2 + 0x12e) = 1;
}
