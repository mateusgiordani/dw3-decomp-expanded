/*
 * CARDGAME:0x800a2180 CARDGAME_F0x800a2180
 * 1160 bytes at CARDGAME.PRO offset 0x1f4d0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a2180, jump table (.rodata) at 0x80083a68
 *  Symbols     CARDGAME_F0x80083e34=0x80083e34 CARDGAME_F0x800954f8=0x800954f8
 *              CARDGAME_F0x8009edcc=0x8009edcc CARDGAME_F0x800a09fc=0x800a09fc
 *              CARDGAME_F0x800a1db8=0x800a1db8 CARDGAME_F0x800a1e60=0x800a1e60
 *              D_8004B7D0=0x8004b7d0
 *  Compare     1160 bytes from 0x800a2180 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a2180
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x30 (s0-s4, ra).
 *
 * Card game state machine on the byte at p1+0x4dc (18-entry jump table at
 * 0x80083a68; state 0 shares the default body and falls through into state 3).
 *
 * p2+0x18 is the scene object, reloaded at every use. The signed byte at
 * p1+0x575 is a 1-based slot count indexing 8-byte records at p1+0x580.
 */

#include <stdint.h>

typedef int32_t (*pad0_t)(int32_t);
typedef int32_t (*pad1_t)(int32_t, int32_t);
typedef void (*m1_t)(int32_t);
typedef void (*m5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);

int32_t CARDGAME_F0x80083e34(int32_t id, int32_t kind, int32_t index);
int32_t CARDGAME_F0x800954f8(int32_t p1, int32_t scene, int32_t value);
void CARDGAME_F0x8009edcc(int32_t p1, int32_t p2);
int32_t CARDGAME_F0x800a09fc(int32_t p1);
int32_t CARDGAME_F0x800a1db8(int32_t p1, int32_t p2);
int32_t CARDGAME_F0x800a1e60(int32_t p1, int32_t p2);

extern uint32_t D_8004B7D0[];

#define P253()       ((pad0_t)D_8004B7D0[253])(0)
#define P258(n)      ((pad1_t)D_8004B7D0[258])(0, (n))
#define PRESSED(n)   ((P253() >> P258(n)) & 1)

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S8(p, o)     (*(int8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))
#define SCENE        S32(p2, 0x18)

int32_t CARDGAME_F0x800a2180(int32_t p1, int32_t p2)
{
    int32_t ret;
    int32_t id;
    int32_t v;
    int32_t i;

    ret = 0;
    switch (U8(p1, 0x4dc)) {
    default:
    case 0:
        if (CARDGAME_F0x800a09fc(p1) != 0) {
            U8(p1, 0x421) = 0x5d;
            U8(p1, 0x2f4) = 1;
            U8(p1, 0x4dc) = 1;
            break;
        }
        U8(p1, 0x4dc) = 3;
        /* fallthrough */
    case 3:
        if (S16(SCENE, 0x64) == 0) {
            U8(p1, 0x4dc) = 4;
        } else {
            U8(p1, 0x4dc) = 5;
            S32(p1, 0x4ec) = 0;
            S32(p1, 0x4e8) = 0;
            U8(p1, 0x4dd) = 0;
        }
        break;
    case 1:
        U8(p1, 0x421) = 0x4c;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x4dc) = 2;
        break;
    case 2:
        U8(p1, 0x421) = 0x5a;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x4dc) = 3;
        break;
    case 4:
        if (CARDGAME_F0x800a1db8(p1, p2) == 0)
            break;
        U8(p1, 0x4dc) = 5;
        S32(p1, 0x4ec) = 0;
        S32(p1, 0x4e8) = 0;
        U8(p1, 0x4dd) = 0;
        break;
    case 5:
        if (CARDGAME_F0x800a1e60(p1, p2) == 0)
            break;
        U8(p1, 0x4dc) = 6;
        S32(p1, 0x4ec) = 0;
        S32(p1, 0x4e8) = 0;
        break;
    case 6:
        {
            int32_t k = S8(p1, 0x575) - 1;
            int32_t o = k * 8;
            int32_t c = S16(p1 + o, 0x580) * 2;

            id = S16(p1 + c, 0x50);
        }
        if (CARDGAME_F0x800954f8(p1, SCENE, CARDGAME_F0x80083e34(id, 1, 0)) != 0) {
            ((m5_t)S32(SCENE, 0xee4))(SCENE, CARDGAME_F0x80083e34(id, 2, 0), 0, 1, 1);
            U8(p1, 0x4dc) = 8;
        } else {
            S16(p1, 0x4e0) = 0;
            U8(p1, 0x4dc) = 7;
        }
        break;
    case 7:
        {
            int32_t k = S8(p1, 0x575) - 1;
            int32_t o = k * 8;
            int32_t c = S16(p1 + o, 0x580) * 2;
            int32_t r = CARDGAME_F0x80083e34(S16(p1 + c, 0x50), 4, S16(p1, 0x4e0));

            if (r != 0) {
                U8(p1, 0x421) = r;
                U8(p1, 0x2f4) = 1;
                S16(p1, 0x4e0) = S16(p1, 0x4e0) + 1;
                break;
            }
        }
        if (S8(p1, 0x575) >= 2) {
            int32_t k = S8(p1, 0x575) - 2;
            int32_t o = k * 8;

            S16(p1 + o, 0x582) = 0;
        }
        for (i = 0; i < 12; i++) {
            int32_t j = i * 0x4c - 1;
            U8(SCENE + (S8(p1, 0x575) + j), 0x146) = 0;
        }
        U8(p1, 0x4dc) = 10;
        break;
    case 8:
        if (PRESSED(0xd) || PRESSED(0xe)) {
            ((m1_t)S32(SCENE, 0xee8))(SCENE);
            U8(p1, 0x4dc) = 9;
        }
        break;
    case 9:
        if (U8(SCENE, 0xdfa) != 0)
            break;
        U8(p1, 0x4dc) = 0xc;
        break;
    case 10:
        U8(p1, 0x421) = 0x5a;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x4dc) = 0xb;
        break;
    case 11:
        U8(p1, 0x4dc) = 0xc;
        break;
    case 12:
        {
            int32_t k = S8(p1, 0x575) - 1;
            int32_t o = k * 8;
            int32_t slot = p1 + o;
            int32_t card = S16(slot, 0x580);
            int32_t c = card * 2;

            int32_t n = U8(slot, 0x584);

            if (S16(p1 + c, 0x50) != 0xd) {
                int32_t off = n * 200;
                int32_t side = p1 + off;

                S16(p1 + (S16(side, 0x5a2) * 2 + off), 0x614) = card;
                S16(side, 0x5a2) = S16(side, 0x5a2) + 1;
            }
        }
        v = U8(p1, 0x575);
        U8(p1, 0x575) = v - 1;
        if (U8(p1, 0x4dd) != 0)
            U8(p1, 0x575) = v - 2;
        U8(p1, 0x4dc) = 0xd;
        S32(p1, 0x4ec) = 0;
        S32(p1, 0x4e8) = 0;
        break;
    case 13:
        CARDGAME_F0x8009edcc(p1, p2);
        U8(p1, 0x4dc) = 0xe;
        break;
    case 14:
        if (CARDGAME_F0x800a09fc(p1) != 0) {
            U8(p1, 0x421) = 0x5d;
            U8(p1, 0x2f4) = 1;
            U8(p1, 0x4dc) = 0xf;
        } else {
            ret = 1;
        }
        break;
    case 15:
        U8(p1, 0x421) = 0x4c;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x4dc) = 0x10;
        break;
    case 16:
        U8(p1, 0x421) = 0x5a;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x4dc) = 0x11;
        break;
    case 17:
        ret = 1;
        break;
    }
    return ret;
}
