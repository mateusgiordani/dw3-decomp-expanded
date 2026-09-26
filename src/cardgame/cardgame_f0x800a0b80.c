/*
 * CARDGAME:0x800a0b80 CARDGAME_F0x800a0b80
 * 340 bytes at CARDGAME.PRO offset 0x1ded0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a0b80, jump table (.rodata) at 0x80083978
 *  Symbols     CARDGAME_F0x8009d8fc=0x8009d8fc CARDGAME_F0x800a09fc=0x800a09fc
 *              CARDGAME_F0x800a0acc=0x800a0acc
 *  Compare     340 bytes from 0x800a0b80 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a0b80
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x20 (s0-s2, ra).
 */

#include <stdint.h>

typedef void (*m2_t)(int32_t, int32_t);
typedef void (*m4_t)(int32_t, int32_t, int32_t, int32_t);

void CARDGAME_F0x8009d8fc(int32_t p1);
int32_t CARDGAME_F0x800a09fc(int32_t p1);
void CARDGAME_F0x800a0acc(int32_t p1, int32_t side);

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))
#define SCENE        S32(p2, 0x18)

int32_t CARDGAME_F0x800a0b80(int32_t p1, int32_t p2)
{
    int32_t ret;

    ret = 0;
    switch (U8(p1, 0x2f9)) {
    case 0:
        U8(p1, 0x421) = 0x9a;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x2f9) = 1;
        break;
    case 1:
        ((m2_t)S32(p1, 0x818))(p1, 0);
        U8(p1, 0x2f9) = 2;
        ((m4_t)S32(SCENE, 0xea0))(SCENE, 0, 6, S16(p1, 0x5a6));
        break;
    case 2:
        CARDGAME_F0x8009d8fc(p1);
        U8(p1, 0x421) = 0x9d;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x2f9) = 3;
        break;
    case 3:
        ((m2_t)S32(p1, 0x818))(p1, 1);
        U8(p1, 0x2f9) = 4;
        CARDGAME_F0x800a09fc(p1);
        CARDGAME_F0x800a0acc(p1, 1);
        CARDGAME_F0x800a0acc(p1, 0);
        ((m4_t)S32(SCENE, 0xea0))(SCENE, 1, 6, S16(p1, 0x66e));
        break;
    case 4:
        U8(p1, 0x421) = 0x9e;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x2f9) = 5;
        break;
    case 5:
        ret = 1;
        break;
    }
    return ret;
}
