/*
 * CARDGAME:0x8008ca4c CARDGAME_F0x8008ca4c
 * 272 bytes at CARDGAME.PRO offset 0x9d9c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008ca4c
 *  Symbols     CARDGAME_F0x80084320=0x80084320
 *  Compare     272 bytes from 0x8008ca4c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008ca4c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Next framed CARDGAME:0x8008cb5c at +0x110 (27bdffc8 prologue), size 0x110
 * contiguous, no overlap.
 *
 * No Ghidra state change.
 *
 * Callers: 19 direct jal from CARDGAME_F0x80084320 (0x80084bc4..0x80084d74 step
 * 0x18, a2 varies: 0, 0x4000, ...); 0 direct callees, 6 indirect (table offsets
 * above, loaded from s0=p2).
 *
 * sha256 532f82e22ce7e7b11e1f47935b8501cc12cf659071149c9d2c19868ff7af0d1d
 * (272B).
 *
 * Loop note: indexed for-loop recomputes the address each iteration (1 word
 * short); the explicit index+pointer do-while (p = p1 + i) reproduces the
 * hoisted addu v1,s2,a0 and top-store shape.
 */

#include <stdint.h>

typedef void (*cardgame_8ca4c_cb1_t)(int32_t);
typedef void (*cardgame_8ca4c_cb2_t)(int32_t, int32_t);
typedef void (*cardgame_8ca4c_cb3_t)(int32_t, int32_t, int32_t);
typedef void (*cardgame_8ca4c_cb4_t)(int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_8ca4c_cb5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);

void CARDGAME_F0x8008ca4c(int32_t p1, int32_t p2, int32_t p3)
{
    int i;
    int p;

    ((cardgame_8ca4c_cb1_t)*(uint32_t *)(p2 + 0xecc))(p2);
    ((cardgame_8ca4c_cb2_t)*(uint32_t *)(p2 + 0xeb8))(p2, p3);
    *(uint32_t *)(p1 + 0x440) = 0;
    *(uint8_t *)(p1 + 0x422) = 1;
    *(uint32_t *)(p1 + 0x438) = p3;
    ((cardgame_8ca4c_cb4_t)*(uint32_t *)(p2 + 0xf14))(p2, 0xf, 0xe500, 0x6100);
    ((cardgame_8ca4c_cb3_t)*(uint32_t *)(p2 + 0xf3c))(p2, 0xf, *(int16_t *)(p1 + (((int8_t *)p1)[0x575] << 3) + 0x580));
    *(uint16_t *)(p2 + 0x594) = 0;
    ((cardgame_8ca4c_cb5_t)*(uint32_t *)(p2 + 0xf24))(p2, 0xf, 8, 0x1000, 0x1000);
    ((cardgame_8ca4c_cb1_t)*(uint32_t *)(p2 + 0xec8))(p2);
    *(uint8_t *)(p1 + 0x49d) = 2;
    *(uint8_t *)(p1 + 0x4ad) = 0;
    i = 0xe;
    p = p1 + i;
    do {
        *(uint8_t *)(p + 0x46f) = 0;
        p--;
        i--;
    } while (i >= 0);
    *(uint8_t *)(p1 + 0x499) = 1;
}
