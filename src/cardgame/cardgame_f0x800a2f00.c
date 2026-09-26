/*
 * CARDGAME:0x800a2f00 CARDGAME_F0x800a2f00
 * 832 bytes at CARDGAME.PRO offset 0x20250 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a2f00
 *  Symbols     CARDGAME_F0x8009d310=0x8009d310 CARDGAME_F0x8009d6b4=0x8009d6b4
 *              CARDGAME_F0x8009d6e0=0x8009d6e0 CARDGAME_F0x800a1b2c=0x800a1b2c
 *              CARDGAME_F0x800a2df8=0x800a2df8 CARDGAME_F0x800a367c=0x800a367c
 *              D_80044B38=0x80044b38 D_80048AC0=0x80048ac0
 *              D_80048D34=0x80048d34 D_800519EC=0x800519ec
 *              D_80055C68=0x80055c68 EXE_F0x8001ffa8=0x8001ffa8
 *  Compare     832 bytes from 0x800a2f00 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a2f00
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x68 (s0-s3, ra).
 *
 * Scene state step on the word at p1+0xc: 1 waits on CARDGAME_F0x800a2df8 and
 * starts a fade, 2 waits for the fade object in p2[1] and records the result, 3
 * plays a sound; any other state (re)initialises the scene when the EXE vector
 * at 0x80055c60 reports idle. EXE vectors are reached through the tables at
 * 0x800519ec and 0x80044b38; p1+0x28 is a state-change method.
 *
 * Matching notes: case 0 shares the default body (PAL dispatch tree pivots on
 * 1); the default's final vector returns int, so its call is not cross-jumped
 * with case 3's void call of the same argument.
 */

#include <stdint.h>

typedef int32_t (*fn0_t)(void);
typedef int32_t (*fn1_t)(int32_t);
typedef void (*m1_t)(int32_t);
typedef void (*m2_t)(int32_t, int32_t);
typedef void (*m4_t)(int32_t, int32_t, int32_t, int32_t);
typedef void (*m6_t)(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);

struct Setup {
    int32_t pad0[9];
    void (*f24)(int32_t, int32_t);
    int32_t pad28;
    void (*f2c)(int32_t);
    int32_t pad30[2];
};

int32_t CARDGAME_F0x8009d310(int32_t p);
int32_t CARDGAME_F0x8009d6b4(void);
void CARDGAME_F0x8009d6e0(int32_t p1, int32_t *p2);
void CARDGAME_F0x800a1b2c(int32_t p1, int32_t *p2);
int32_t CARDGAME_F0x800a2df8(int32_t p1, int32_t *p2);
int32_t CARDGAME_F0x800a367c(int32_t kind);
void EXE_F0x8001ffa8(struct Setup *out);

extern int32_t D_800519EC[];
extern int32_t D_80044B38[];
extern int8_t D_80048D34[];
extern int32_t D_80048AC0;
extern void (*D_80055C68)(int32_t);

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))
#define METHOD(t, p, o) ((t)S32(p, o))

void CARDGAME_F0x800a2f00(int32_t p1, int32_t *p2)
{
    struct Setup setup;

    switch (S32(p1, 0xc)) {
    default:
    case 0:
        if (((fn0_t)D_800519EC[0x109d])() == 0) {
            ((fn1_t)D_80044B38[0x105])(0x25d);
            CARDGAME_F0x800a1b2c(p1, p2);
            CARDGAME_F0x8009d6e0(p1, p2);
            p2[6] = CARDGAME_F0x8009d310(p1 + 0x50);
            p2[0] = CARDGAME_F0x8009d6b4();
            METHOD(m1_t, p2[6], 0xecc)(p2[6]);
            *(int16_t *)(p2[6] + 0x5e) = U8(p1, 0x2e9);
            *(int16_t *)(p2[6] + 0x5c) = U8(p1, 0x2e8);
            EXE_F0x8001ffa8(&setup);
            setup.f24(0x280, 0);
            setup.f2c(((fn1_t)D_80044B38[0x109])(0x25d0000));
            p2[1] = CARDGAME_F0x800a367c(2);
            METHOD(m4_t, p2[1], 0x68)(p2[1], 0xff, 0xff, 0xff);
            METHOD(m6_t, p2[1], 0x6c)(p2[1], 0, 0, 0, 100, 0);
            METHOD(m2_t, p1, 0x28)(p1, 2);
            ((fn1_t)D_800519EC[0x1097])(0x609c0004);
        }
        break;
    case 1:
        if (CARDGAME_F0x800a2df8(p1, p2) == 0)
            break;
        p2[1] = CARDGAME_F0x800a367c(2);
        METHOD(m4_t, p2[1], 0x68)(p2[1], 0, 0, 0);
        METHOD(m6_t, p2[1], 0x6c)(p2[1], 0xff, 0xff, 0xff, 100, 0);
        METHOD(m2_t, p1, 0x28)(p1, 2);
        U8(p1, 0x303) = 1;
        break;
    case 2:
        if (S32(p1, 0x10) != 0)
            break;
        if (METHOD(fn1_t, p2[1], 0x70)(p2[1]) == 0)
            break;
        METHOD(m1_t, p2[1], 0x74)(p2[1]);
        if (U8(p1, 0x303) != 0) {
            if (U8(p1, 0x303) != 2) {
                if (U8(p1, 0x302) == 0) {
                    int8_t *q = D_80048D34 + S32(p1, 0x2ec);

                    if (q[0x7c] < 99)
                        q[0x7c]++;
                    D_80048AC0 = 1;
                } else {
                    D_80048AC0 = 0;
                }
            }
            U8(p1, 0x303) = 2;
        } else {
            METHOD(m2_t, p1, 0x28)(p1, 1);
        }
        S32(p1, 0x10) = 1;
        break;
    case 3:
        if (U8(p1, 0x302) == 0)
            D_80055C68(0x6004001e);
        else
            D_80055C68(0x609c0004);
        break;
    }
}
