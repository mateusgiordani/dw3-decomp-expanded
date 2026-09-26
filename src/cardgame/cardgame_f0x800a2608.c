/*
 * CARDGAME:0x800a2608 CARDGAME_F0x800a2608
 * 2032 bytes at CARDGAME.PRO offset 0x1f958 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a2608, jump table (.rodata) at 0x80083ab0
 *  Symbols     D_8004B7D0=0x8004b7d0 D_80055c48=0x80055c48
 *              D_800a5dc8=0x800a5dc8
 *  Compare     2032 bytes from 0x800a2608 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a2608
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x40 (s0-s7, ra).
 *
 * Card game menu state machine on the byte at p1+0x568 (15-entry jump table at
 * 0x80083ab0 for states 1..15; state 0 and 1 share the default body; the cursor
 * switch of state 4 uses a second table at 0x80083af0). The scene object is
 * reloaded from p2+0x18 at every use. States 0/1 save a 0x78-byte block
 * (p1+0x420 -> p1+0x4f0) and 24 bytes of the scene object into the global
 * D_800a5dc8; state 15 restores both and returns 1.
 */

#include <stdint.h>

typedef int32_t (*pad0_t)(int32_t);
typedef int32_t (*pad1_t)(int32_t, int32_t);
typedef void (*m1_t)(int32_t);
typedef void (*m2_t)(int32_t, int32_t);
typedef void (*m5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);

struct Block78 { int32_t w[30]; };
struct Block24 { int32_t w[6]; };

extern uint32_t D_8004B7D0[];
extern void (*D_80055c48)(uint32_t);
extern struct Block24 D_800a5dc8;

#define P253()       ((pad0_t)D_8004B7D0[253])(0)
#define P255()       ((pad0_t)D_8004B7D0[255])(0)
#define P258(n)      ((pad1_t)D_8004B7D0[258])(0, (n))
#define PRESSED(n)   ((P253() >> P258(n)) & 1)
#define PAIR(n)      ((P253() & (1 << P258(n))) | (P255() & (1 << P258(n))))

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))
#define SCENE        S32(p2, 0x18)
#define METHOD(t, o) ((t)*(int32_t *)(SCENE + (o)))

int32_t CARDGAME_F0x800a2608(int32_t p1, int32_t p2)
{
    int32_t ret;
    int32_t v;

    ret = 0;
    switch (U8(p1, 0x568)) {
    default:
    case 1:
        *(struct Block78 *)(p1 + 0x4f0) = *(struct Block78 *)(p1 + 0x420);
        U8(p1, 0x569) = 0;
        D_800a5dc8 = *(struct Block24 *)(SCENE + 0xde4);
        METHOD(m2_t, 0xef4)(SCENE, 0);
        U8(p1, 0x568) = 2;
        break;
    case 2:
        if (S16(SCENE, 0xe0a) == 2)
            U8(p1, 0x568) = 3;
        break;
    case 3:
        if (PAIR(4)) {
            int32_t d = U8(p1, 0x569) - 1;
            if (d < 0)
                d = 4;
            U8(p1, 0x569) = d;
            D_80055c48(0x8004513e);
        } else if (PAIR(6)) {
            U8(p1, 0x569) = (uint8_t)(U8(p1, 0x569) + 1) % 5;
            D_80055c48(0x8004513e);
        }
        if (PRESSED(0xd)) {
            METHOD(m1_t, 0xefc)(SCENE);
            U8(p1, 0x568) = 4;
        } else if (PRESSED(0xe)) {
            D_80055c48(0x800450bd);
            METHOD(m1_t, 0xef8)(SCENE);
            U8(p1, 0x568) = 4;
            U8(p1, 0x569) = 3;
        }
        METHOD(m2_t, 0xf00)(SCENE, U8(p1, 0x569));
        break;
    case 4:
        if (S16(SCENE, 0xe0a) != 0)
            break;
        switch (U8(p1, 0x569)) {
        case 0:
            U8(p1, 0x568) = 5;
            break;
        case 1:
            U8(p1, 0x568) = 6;
            break;
        case 2:
            U8(p1, 0x568) = 7;
            break;
        case 3:
            U8(p1, 0x568) = 0xf;
            break;
        case 4:
            METHOD(m5_t, 0xee4)(SCENE, 0x2c, 1, 0, 1);
            U8(p1, 0x568) = 8;
            break;
        }
        break;
    case 5:
        U8(p1, 0x421) = 0x9b;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x568) = 0xb;
        break;
    case 6:
        U8(p1, 0x421) = 0xa8;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x568) = 0xc;
        break;
    case 7:
        U8(p1, 0x421) = 0x9c;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x568) = 0xd;
        break;
    case 8:
        if (U8(SCENE, 0xdfa) == 2)
            U8(p1, 0x568) = 9;
        break;
    case 9:
        if (PRESSED(0xd)) {
            METHOD(m1_t, 0xeec)(SCENE);
        } else {
            if (PAIR(4)) {
                if (S16(SCENE, 0xdf4) != 0)
                    D_80055c48(0x8004513e);
                S16(SCENE, 0xdf4) = 0;
                break;
            }
            if (PAIR(6)) {
                if (S16(SCENE, 0xdf4) != 1)
                    D_80055c48(0x8004513e);
                S16(SCENE, 0xdf4) = 1;
                break;
            }
            if (!PRESSED(0xe))
                break;
            D_80055c48(0x800450bd);
            S16(SCENE, 0xdf4) = 1;
            METHOD(m1_t, 0xeec)(SCENE);
            S16(SCENE, 0xdf4) = 1;
        }
        U8(p1, 0x568) = 10;
        break;
    case 10:
        v = SCENE;
        if (U8(v, 0xdfa) != 0)
            break;
        if (S16(v, 0xdf4) == 0)
            ret = 2;
        else
            U8(p1, 0x568) = 0xe;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
        METHOD(m2_t, 0xef4)(SCENE, U8(p1, 0x569));
        U8(p1, 0x568) = 2;
        break;
    case 15:
        *(struct Block78 *)(p1 + 0x420) = *(struct Block78 *)(p1 + 0x4f0);
        *(struct Block24 *)(SCENE + 0xde4) = D_800a5dc8;
        U8(p1, 0x568) = 0;
        ret = 1;
        break;
    }
    return ret;
}
