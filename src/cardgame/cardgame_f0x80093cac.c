/*
 * CARDGAME:0x80093cac CARDGAME_F0x80093cac
 * 912 bytes at CARDGAME.PRO offset 0x10ffc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80093cac, jump table (.rodata) at 0x8008363c
 *  Symbols     CARDGAME_F0x80093cac=0x80093cac CARDGAME_LAB_80093bc4=0x80093bc4
 *              DAT_8004B7D0=0x8004b7d0 DAT_8004df9c=0x8004df9c
 *  Compare     912 bytes from 0x80093cac and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80093cac
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prev CARDGAME:0x80093c18 size 148 (0x94) ends exactly at 0x80093cac (gap 0);
 * next CARDGAME:0x8009403c at +0x390 (prologue addiu sp,-0x20) confirms size
 * 0x390.
 *
 * State machine on byte p1+0x422, cases 1..8 via table at 0x8008363c (lui
 * v0,0x8008 + addiu 0x363c, indexed by (state-1)*4, range-checked < 8).
 *
 * Returns 1 only on the case-8 path (s4=1); all other paths return s4=0.
 *
 * Leaf helper CARDGAME_LAB_80093bc4 lives in the gap after CARDGAME:0x80093b10
 * (which ends at 0x80093bc4); reached via 4 direct jals. EXE-space vectors
 * decoded from raw PAL words: lui 0x8005 + lw -0x2064 = *(0x8004df9c); s3 =
 * 0x80050000-0x4830 = 0x8004b7d0 with slots +0x3f4/+0x408 =
 * 0x8004bbc4/0x8004bbd8.
 *
 * r7 (o55/s0923d): the result lives in one local (s4) set to 0 before the
 * switch, 1 only in case 8, returned at the common exit; swap temporaries are
 * int (lh).
 *
 * Details: strategy-r7-o55/attempts-r7.
 */

#include "common/types.h"

typedef void (*cardgame_cb1_t)(void *a0);
typedef void (*cardgame_cb2_t)(void *a0, int32_t a1);
typedef int32_t (*cardgame_cb4_t)(void *a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*cardgame_cb5_t)(void *a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
typedef int32_t (*cardgame_get_t)(void);
typedef int32_t (*cardgame_rng_t)(int32_t a0);
typedef uint32_t (*cardgame_bit_t)(int32_t a0, int32_t a1);

/* Arity varies per call site: first site sets a0/a1/a2 only (a3 stale), later
   sites also load a3 from p1+0x424; unspecific prototype preserves both shapes. */
extern int32_t CARDGAME_LAB_80093bc4(int32_t, int32_t, int32_t, int32_t);
extern cardgame_get_t DAT_8004df9c;
extern int32_t DAT_8004B7D0[];

int32_t CARDGAME_F0x80093cac(uint8_t *p1, uint8_t *p2)
{
    int32_t ret;
    int32_t tmp;

    ret = 0;
    switch (p1[0x422]) {
    case 1:
        if (*(uint8_t *)(p2 + 0x656) != 1)
            break;
        ((cardgame_cb2_t)*(uint32_t *)(p2 + 0xf1c))(p2, 0x11);
        p1[0x422] = 2;
        break;
    case 2:
        if (*(uint8_t *)(p2 + 0x656) != 1)
            break;
        p1[0x422] = 3;
        *(int32_t *)(p1 + 0x424) = 0;
        break;
    case 3:
        if ((*(int32_t *)(p1 + 0x424) += DAT_8004df9c()) < 0xf) {
            tmp = CARDGAME_LAB_80093bc4((int32_t)*(int16_t *)(p1 + 0x664),
                                        (int32_t)*(int16_t *)(p1 + 0x59c), 0xf, *(int32_t *)(p1 + 0x424));
            ((cardgame_cb4_t)*(uint32_t *)(p2 + 0xea0))(p2, 0, 8, tmp);
            tmp = CARDGAME_LAB_80093bc4((int32_t)*(int16_t *)(p1 + 0x666),
                                        (int32_t)*(int16_t *)(p1 + 0x59e), 0xf, *(int32_t *)(p1 + 0x424));
            ((cardgame_cb4_t)*(uint32_t *)(p2 + 0xea0))(p2, 0, 9, tmp);
            tmp = CARDGAME_LAB_80093bc4((int32_t)*(int16_t *)(p1 + 0x59c),
                                        (int32_t)*(int16_t *)(p1 + 0x664), 0xf, *(int32_t *)(p1 + 0x424));
            ((cardgame_cb4_t)*(uint32_t *)(p2 + 0xea0))(p2, 1, 8, tmp);
            tmp = CARDGAME_LAB_80093bc4((int32_t)*(int16_t *)(p1 + 0x59e),
                                        (int32_t)*(int16_t *)(p1 + 0x666), 0xf, *(int32_t *)(p1 + 0x424));
            ((cardgame_cb4_t)*(uint32_t *)(p2 + 0xea0))(p2, 1, 9, tmp);
            break;
        }
        {
            int32_t t0 = *(int16_t *)(p1 + 0x59c);
            int32_t t1 = *(int16_t *)(p1 + 0x59e);
            *(int16_t *)(p1 + 0x59c) = *(int16_t *)(p1 + 0x664);
            *(int16_t *)(p1 + 0x664) = t0;
            *(int16_t *)(p1 + 0x59e) = *(int16_t *)(p1 + 0x666);
            *(int16_t *)(p1 + 0x666) = t1;
        }
        ((cardgame_cb4_t)*(uint32_t *)(p2 + 0xea0))(p2, 0, 8, (int32_t)*(int16_t *)(p1 + 0x59c));
        ((cardgame_cb4_t)*(uint32_t *)(p2 + 0xea0))(p2, 0, 9, (int32_t)*(int16_t *)(p1 + 0x59e));
        ((cardgame_cb4_t)*(uint32_t *)(p2 + 0xea0))(p2, 1, 8, (int32_t)*(int16_t *)(p1 + 0x664));
        ((cardgame_cb4_t)*(uint32_t *)(p2 + 0xea0))(p2, 1, 9, (int32_t)*(int16_t *)(p1 + 0x666));
        ((cardgame_cb5_t)*(uint32_t *)(p2 + 0xf24))(p2, 0x11, 0xa, 0, 0x1000);
        p1[0x422] = 4;
        break;
    case 4:
        if (*(uint8_t *)(p2 + 0x656) != 1)
            break;
        p1[0x422] = 5;
        ((cardgame_cb5_t)*(uint32_t *)(p2 + 0xee4))(p2, 0x2d, 0, 0, 1);
        break;
    case 5:
        if (*(uint8_t *)(p2 + 0xdfa) != 2)
            break;
        p1[0x422] = 6;
        break;
    case 6:
        if ((((cardgame_rng_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_bit_t)DAT_8004B7D0[0x102])(0, 0xd) & 1) == 0
            && (((cardgame_rng_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_bit_t)DAT_8004B7D0[0x102])(0, 0xe) & 1) == 0)
            break;
        p1[0x422] = 7;
        ((cardgame_cb1_t)*(uint32_t *)(p2 + 0xee8))(p2);
        break;
    case 7:
        if (*(uint8_t *)(p2 + 0xdfa) != 0)
            break;
        p1[0x422] = 8;
        break;
    case 8:
        ret = 1;
        break;
    }
    return ret;
}
