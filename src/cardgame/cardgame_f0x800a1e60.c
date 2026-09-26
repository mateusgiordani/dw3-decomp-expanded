/*
 * CARDGAME:0x800a1e60 CARDGAME_F0x800a1e60
 * 800 bytes at CARDGAME.PRO offset 0x1f1b0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a1e60, jump table (.rodata) at 0x80083a50
 *  Symbols     D_8004B7D0=0x8004b7d0 D_8004DF9C=0x8004df9c
 *              D_80055C48=0x80055c48 D_8005CCB0=0x8005ccb0
 *              D_800A5D84=0x800a5d84
 *  Compare     800 bytes from 0x800a1e60 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a1e60
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x30 (s0-s3, ra).
 *
 * Card presentation state machine on the word at p1+0x4e8 (6-entry jump table
 * at 0x80083a50; state 4 and out-of-range states return 0), with the tick
 * counter at p1+0x4ec advanced by the EXE timer vector at 0x8004df9c.
 *
 * p2+0x18 is the scene object, reloaded at every use. Returns 1 when the last
 * state has waited 15 ticks.
 *
 * Matching notes: states 1 and 3 each write their own tail and break (jump2
 * cross-jumps state 1 into state 3's copy); the table base is a block local
 * taken after the card offset and before the position offset.
 */

#include <stdint.h>

typedef int32_t (*pad0_t)(int32_t);
typedef int32_t (*pad1_t)(int32_t, int32_t);
typedef void (*m2_t)(int32_t, int32_t);
typedef void (*m5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*m6_t)(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);

struct Pos {
    int16_t x;
    int16_t y;
};

extern uint32_t D_8004B7D0[];
extern int32_t (*D_8004DF9C)(void);
extern void (*D_80055C48)(uint32_t);
extern int32_t D_8005CCB0;
extern struct Pos D_800A5D84[][2];

#define P254()       ((pad0_t)D_8004B7D0[254])(0)
#define P258(n)      ((pad1_t)D_8004B7D0[258])(0, (n))

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S8(p, o)     (*(int8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))
#define POS          ((int32_t)D_800A5D84)
#define SCENE        S32(p2, 0x18)
#define METHOD(t, o) ((t)S32(SCENE, (o)))

int32_t CARDGAME_F0x800a1e60(int32_t p1, int32_t p2)
{
    int32_t ret;
    int32_t i;

    ret = 0;
    switch (S32(p1, 0x4e8)) {
    case 0:
        S32(p1, 0x4e8) = 1;
        S32(p1, 0x4ec) = 0;
        {
            int32_t k = S8(p1, 0x575) - 1;
            int32_t o = k * 8;
            int32_t slot = p1 + o;
            int32_t c = S16(slot, 0x580) * 2;
            int32_t tbl = POS;
            int32_t q = U8(slot, 0x584) * 4 + D_8005CCB0 * 8;
            int32_t pos = tbl + q;

            METHOD(m6_t, 0xeac)(SCENE, 2, 1, S16(p1 + c, 0x50) + 1, S16(pos, 0), S16(pos, 2));
        }
        for (i = 0; i < 15; i++) {
            int32_t o = i * 0x4c;

            S16(SCENE + o, 0x12e) = 0;
        }
        return ret;
    case 1:
        if (S32(p1, 0x4ec) >= 20) {
            if ((P254() >> P258(0xd)) & 1)
                S32(p1, 0x4ec) = 0x23;
        }
        S32(p1, 0x4ec) += D_8004DF9C();
        if (S32(p1, 0x4ec) < 0x23)
            return ret;
        METHOD(m2_t, 0xeb0)(SCENE, 2);
        METHOD(m5_t, 0xf24)(SCENE, S8(p1, 0x575) + 0xb, 8, 0x1400, 0x1400);
        METHOD(m2_t, 0xea8)(SCENE, S8(p1, 0x575) - 1);
        S32(p1, 0x4e8) = 2;
        S32(p1, 0x4ec) = 0;
        D_80055C48(0x8004603c);
        break;
    case 2:
        S32(p1, 0x4ec) += D_8004DF9C();
        if (S32(p1, 0x4ec) < 8)
            return ret;
        METHOD(m2_t, 0xf1c)(SCENE, S8(p1, 0x575) + 0xb);
        S32(p1, 0x4e8) = 3;
        S32(p1, 0x4ec) = 0;
        break;
    case 3:
        S32(p1, 0x4ec) += D_8004DF9C();
        if (S32(p1, 0x4ec) < 15)
            return ret;
        METHOD(m5_t, 0xf24)(SCENE, S8(p1, 0x575) + 0xb, 4, 0, 0);
        S32(p1, 0x4e8) = 5;
        S32(p1, 0x4ec) = 0;
        D_80055C48(0x8004603c);
        break;
    case 5:
        S32(p1, 0x4ec) += D_8004DF9C();
        if (S32(p1, 0x4ec) < 15)
            return ret;
        ret = 1;
        break;
    }
    return ret;
}
