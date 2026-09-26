/*
 * CARDGAME:0x800a0f94 CARDGAME_F0x800a0f94
 * 2660 bytes at CARDGAME.PRO offset 0x1e2e4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a0f94, jump table (.rodata) at 0x80083990
 *  Symbols     CARDGAME_F0x8009dec4=0x8009dec4 CARDGAME_F0x800a0cd4=0x800a0cd4
 *              CARDGAME_F0x800a367c=0x800a367c D_8004B7D0=0x8004b7d0
 *              D_8004df9c=0x8004df9c
 *  Compare     2660 bytes from 0x800a0f94 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a0f94
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x30 (s0-s4, ra).
 *
 * Card game flow state machine on the byte at p1+0x2f9 (27-entry jump table at
 * 0x80083990; case 24 shares the default target). p2+0x18 is the scene object
 * whose methods at +0xeac/+0xeb0/+0xee4/+0xee8 are called; the pad table at
 * D_8004B7D0 (slots 253/258) is polled for buttons 0xd/0xe.
 *
 * Every exit returns `ret` (s4): 0, or 2/1 for states 25/26.
 */

#include <stdint.h>

typedef int32_t (*pad0_t)(int32_t);
typedef int32_t (*pad1_t)(int32_t, int32_t);
typedef void (*exe_cb_t)(uint32_t);
typedef int32_t (*tick_t)(void);
typedef void (*m1_t)(int32_t);
typedef void (*m2_t)(int32_t, int32_t);
typedef void (*m4_t)(int32_t, int32_t, int32_t, int32_t);
typedef void (*m5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*m6_t)(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);

int32_t CARDGAME_F0x800a0cd4(int32_t p1, int32_t which, int32_t cursor);
int32_t CARDGAME_F0x800a367c(int32_t kind);
int32_t CARDGAME_F0x8009dec4(int32_t p1, int32_t p2);

extern uint32_t D_8004B7D0[];
extern tick_t D_8004df9c;

#define PAD_HELD()   ((pad0_t)D_8004B7D0[253])(0)
#define PAD_BIT(n)   ((pad1_t)D_8004B7D0[258])(0, (n))
#define PRESSED(n)   ((PAD_HELD() >> PAD_BIT(n)) & 1)

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))
#define METHOD(t, obj, o) ((t)*(int32_t *)((obj) + (o)))

int32_t CARDGAME_F0x800a0f94(int32_t p1, int32_t p2)
{
    int32_t obj;
    int32_t ret;
    int32_t v;
    uint32_t i;

    obj = S32(p2, 0x18);
    ret = 0;
    switch (U8(p1, 0x2f9)) {
    case 0:
        if (S16(S32(p2, 0x18), 0x64) != 2)
            break;
        if (U8(p1, 0x498) != 0)
            break;
        U8(p1, 0x2f9) = 1;
        S32(p1, 0x2fc) = 0;
        break;
    case 1:
        S32(p1, 0x2fc) = CARDGAME_F0x800a0cd4(p1, 0, S32(p1, 0x2fc));
        if (S32(p1, 0x2fc) == -1) {
            U8(p1, 0x2f9) = 2;
            S32(p1, 0x2fc) = 0;
        } else {
            U8(p1, 0x421) = 0x19;
            U8(p1, 0x2f4) = 1;
        }
        break;
    case 2:
        S32(p1, 0x2fc) = CARDGAME_F0x800a0cd4(p1, 1, S32(p1, 0x2fc));
        if (S32(p1, 0x2fc) != -1) {
            U8(p1, 0x421) = 0x1a;
            U8(p1, 0x2f4) = 1;
        } else {
            U8(p1, 0x2f9) = 8;
        }
        break;
    case 8:
        if (U8(p1, 0x304) != 0) {
            U8(p1, 0x421) = 0x1b;
            U8(p1, 0x2f4) = 1;
            U8(p1, 0x305) = U8(p1, 0x305) - 1;
        }
        if (U8(p1, 0x305) == 0)
            U8(p1, 0x2f9) = 9;
        else
            U8(p1, 0x2f9) = 8;
        break;
    case 9:
        if (U8(p1, 0x72c) == 0 || U8(p1, 0x79e) == 0) {
            if (S16(p1, 0x59e) > S16(p1, 0x666))
                U8(p1, 0x301) = 0;
            else
                U8(p1, 0x301) = 1;
            U8(p1, 0x2f9) = 0xb;
            {
                int32_t o = U8(p1, 0x301) * 0x72;
                if (U8(p1 + o, 0x72c) != 0)
                    break;
            }
            {
                int32_t o = (U8(p1, 0x301) ^ 1) * 0x72;
                if (U8(p1 + o, 0x72c) == 0)
                    break;
            }
            U8(p1, 0x2f9) = 10;
            S32(p1, 0x2fc) = 0;
        } else {
            U8(p1, 0x2f9) = 3;
            METHOD(m6_t, obj, 0xeac)(obj, 5, 5, 0xd, 0, 0x6e);
        }
        break;
    case 3:
        if (U8(obj, 0xe9b) == 2)
            U8(p1, 0x2f9) = 4;
        break;
    case 4:
        if (PRESSED(0xd) || PRESSED(0xe)) {
            U8(p1, 0x2f9) = 5;
            METHOD(m2_t, obj, 0xeb0)(obj, 5);
        }
        break;
    case 5:
        if (U8(obj, 0xe9b) == 0)
            U8(p1, 0x2f9) = 6;
        break;
    case 6:
        U8(p1, 0x421) = 0x15;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x2f9) = 7;
        break;
    case 7:
        U8(p1, 0x421) = 0x16;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x2f9) = 10;
        S32(p1, 0x2fc) = 0;
        break;
    case 10:
        if (S16(p1, 0x59e) > S16(p1, 0x666)) {
            U8(p1, 0x421) = 0x18;
            U8(p1, 0x301) = 0;
        } else {
            U8(p1, 0x421) = 0x17;
            U8(p1, 0x301) = 1;
        }
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x2f9) = 0xb;
        S32(p1, 0x2fc) = 0;
        break;
    case 11:
        i = U8(p1, 0x301);
        if (S32(p1, 0x2fc) & 1) {
            int32_t a = i * 0x54;
            int32_t b = i * 200;
            S32(S32(p2, 0x18) + a, 0xa4) = U8(p1 + b, 0x5ae) + 1;
        } else {
            int32_t a = i * 0x54;
            int32_t b = i * 200;
            S32(S32(p2, 0x18) + a, 0xa4) = U8(p1 + b, 0x5ae);
        }
        if (S32(p1, 0x2fc) == 0) {
            ((exe_cb_t)*(uint32_t *)0x80055c48)(0x9c0002);
            S32(p2, 4) = CARDGAME_F0x800a367c(1);
            METHOD(m4_t, S32(p2, 4), 0x68)(S32(p2, 4), 0x80, 0x80, 0x80);
            METHOD(m6_t, S32(p2, 4), 0x6c)(S32(p2, 4), 0, 0, 0, 10, 1);
        }
        S32(p1, 0x2fc) += D_8004df9c();
        if (S32(p1, 0x2fc) < 0x33)
            break;
        {
            int32_t a = i * 0x54;
            int32_t b = p1 + i * 200;
            S32(S32(p2, 0x18) + a, 0xa4) = U8(b, 0x5ae) + 1;
            U8(b, 0x5ae) = U8(b, 0x5ae) + 1;
        }
        U8(p1, 0x2f9) = 0xc;
        if (U8(p1, 0x301) == 0) {
            METHOD(m6_t, obj, 0xeac)(obj, 5, 5, 0x10, 0, 0x6e);
        } else if (S16(p1, 0x59e) != S16(p1, 0x666)) {
            METHOD(m6_t, obj, 0xeac)(obj, 5, 5, 0x11, 0, 0x6e);
        } else {
            METHOD(m5_t, obj, 0xee4)(obj, 0x12, 0, 0, 1);
        }
        break;
    case 12:
        v = (S16(p1, 0x59e) != S16(p1, 0x666) ? U8(obj, 0xe9b) : U8(obj, 0xdfa)) == 2;
        if (v)
            U8(p1, 0x2f9) = 0xd;
        break;
    case 13:
        if (PRESSED(0xd) || PRESSED(0xe)) {
            if (S16(p1, 0x59e) != S16(p1, 0x666))
                METHOD(m2_t, obj, 0xeb0)(obj, 5);
            else
                METHOD(m1_t, obj, 0xee8)(obj);
            U8(p1, 0x2f9) = 0xe;
        }
        break;
    case 14:
        v = (S16(p1, 0x59e) != S16(p1, 0x666) ? U8(obj, 0xe9b) : U8(obj, 0xdfa)) == 0;
        if (v) {
            U8(p1, 0x2f9) = 0xf;
            METHOD(m5_t, obj, 0xee4)(obj, 0x13, 0, 0, 1);
        }
        break;
    case 15:
        if (U8(obj, 0xdfa) == 2)
            U8(p1, 0x2f9) = 0x10;
        break;
    case 16:
        if (PRESSED(0xd) || PRESSED(0xe)) {
            METHOD(m1_t, obj, 0xee8)(obj);
            U8(p1, 0x2f9) = 0x11;
        }
        break;
    case 17:
        if (U8(obj, 0xdfa) != 0)
            break;
        if (U8(p1, 0x5ae) >= 2) {
            ((exe_cb_t)*(uint32_t *)0x80055c48)(0x6004001e);
            METHOD(m6_t, obj, 0xeac)(obj, 5, 5, 0x15, 0, 0x6e);
            U8(p1, 0x2f9) = 0x13;
            U8(p1, 0x302) = 0;
        } else if (U8(p1, 0x676) >= 2) {
            METHOD(m6_t, obj, 0xeac)(obj, 5, 5, 0x16, 0, 0x6e);
            U8(p1, 0x2f9) = 0x13;
            U8(p1, 0x302) = 1;
        } else {
            U8(p1, 0x2f9) = 0x12;
        }
        break;
    case 18:
        if (U8(p1, 0x301) != 0)
            U8(p1, 0x421) = 0x1d;
        else
            U8(p1, 0x421) = 0x1c;
        U8(p1, 0x2f4) = 1;
        U8(p1, 0x2f9) = 0x1a;
        break;
    case 19:
        if (U8(obj, 0xe9b) == 2)
            U8(p1, 0x2f9) = 0x14;
        break;
    case 20:
        if (PRESSED(0xd) || PRESSED(0xe)) {
            if (U8(p1, 0x301) == 0) {
                METHOD(m2_t, obj, 0xeb0)(obj, 5);
                U8(p1, 0x2f9) = 0x15;
                S32(p1, 0x4e8) = 0;
            } else {
                U8(p1, 0x2f9) = 0x19;
            }
        }
        break;
    case 21:
        if (U8(obj, 0xe9b) != 0)
            break;
        if (CARDGAME_F0x8009dec4(p1, p2) != 0) {
            U8(p1, 0x2f9) = 0x16;
            METHOD(m5_t, obj, 0xee4)(obj, S32(p1, 0x2ec), 0, 0, 3);
        }
        break;
    case 22:
        if (U8(obj, 0xdfa) == 2)
            U8(p1, 0x2f9) = 0x17;
        break;
    case 23:
        if (PRESSED(0xd) || PRESSED(0xe))
            U8(p1, 0x2f9) = 0x19;
        break;
    case 25:
        ret = 2;
        break;
    case 26:
        ret = 1;
        break;
    }
    return ret;
}
