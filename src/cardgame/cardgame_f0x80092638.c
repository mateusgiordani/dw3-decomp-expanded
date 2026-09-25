/*
 * CARDGAME:0x80092638 CARDGAME_F0x80092638
 * 1388 bytes at CARDGAME.PRO offset 0xf988 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80092638, jump table (.rodata) at 0x800835b4
 *  Symbols     CARDGAME_F0x800860d4=0x800860d4 CARDGAME_F0x80092638=0x80092638
 *              DAT_8004B7D0=0x8004b7d0 DAT_800A5910=0x800a5910
 *  Compare     1388 bytes from 0x80092638 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80092638
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL-SLES-03936; 1388 text bytes and 36 jump-table bytes.
 *
 * Verified base 0x80082cb0; table 0x800835b4 orders blocks 2,3,4,1,5..9.
 *
 * Keep the return value live across callbacks; case 9 supplies the only
 * success.
 *
 * The 76-byte record is an aligned opaque word view, not a semantic layout.
 *
 * Timer updates read the accumulator after tick; address locals preserve PAL
 * value formation. The direct shifts reproduce the native MIPS srav operation.
 */

#include <stdint.h>

typedef int (*cardgame_tick_t)(void);
typedef int (*cardgame_poll_t)(int32_t);
typedef uint32_t (*cardgame_mask_t)(int32_t, int32_t);
typedef void (*cardgame_draw_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_step_t)(int32_t, int32_t);
typedef void (*cardgame_show_t)(int32_t, int32_t, int32_t);
typedef void (*cardgame_mark_t)(int32_t, int32_t);
typedef void (*cardgame_span_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_done_t)(int32_t, int32_t);

typedef struct { int32_t words[19]; } cardgame_record76_t;

extern int32_t DAT_8004B7D0[];
extern int32_t DAT_800A5910[];

extern int CARDGAME_F0x800860d4(int32_t a0, int32_t a1, int32_t a2,
                                int32_t a3, int32_t a4, int32_t a5);

int CARDGAME_F0x80092638(int32_t p1, int32_t p2)
{
    int ret = 0;
    uint8_t state = *(uint8_t *)(p1 + 0x422);

    switch (state) {
    case 2: {
        int32_t t;
        int32_t r;
        *(int16_t *)(p2 + 0xe32) = 0;
        *(int32_t *)(p2 + 0xe34) = 0;
        *(int32_t *)(p2 + 0xe4c) = 0;
        *(int32_t *)(p2 + 0xe64) = 0;
        *(int32_t *)(p2 + 0xe7c) = 0;
        r = CARDGAME_F0x800860d4(p1, p2, 2, 0,
                                *(int32_t *)(p1 + 0x424),
                                *(int32_t *)(p1 + 0x428));
        *(int32_t *)(p1 + 0x428) = r;
        t = ((cardgame_tick_t)*(uint32_t *)0x8004df9c)();
        t = *(int32_t *)(p1 + 0x424) += t;

        if (t > 0x14) {
            *(int32_t *)(p1 + 0x424) = 0;
            *(int32_t *)(p1 + 0x428) = 0;
            *(uint8_t *)(p1 + 0x422) = 3;
        }
        break;
    }
    case 3: {
        int32_t b0 = ((cardgame_poll_t)DAT_8004B7D0[0xfd])(0);
        uint32_t m0 = ((cardgame_mask_t)DAT_8004B7D0[0x102])(0, 13);
        int32_t i1;
        int32_t i2;
        int32_t v;
        int32_t addr;
        if (((b0 >> m0) & 1) != 0) {
            *(uint8_t *)(p1 + 0x422) = 4;
        } else {
            int32_t b1 = ((cardgame_poll_t)DAT_8004B7D0[0xfd])(0);
            uint32_t m1 = ((cardgame_mask_t)DAT_8004B7D0[0x102])(0, 14);
            if (((b1 >> m1) & 1) != 0)
                *(uint8_t *)(p1 + 0x422) = 4;
        }
        i1 = *(int8_t *)(p1 + 0x575);
        addr = p1 + i1 * 8;
        i2 = *(int16_t *)(addr + 0x580);
        addr = p1 + i2 * 2;
        v = *(int16_t *)(addr + 0x50);
        *(uint8_t *)(p2 + 0xe82) = 0;
        *(int32_t *)(p2 + 0xe4c) = v + 1;
        *(int32_t *)(p2 + 0xe7c) = v + 1;
        break;
    }
    case 4: {
        int32_t idx = *(int8_t *)(p1 + 0x575) * 8;
        int32_t *pair = (int32_t *)((char *)DAT_800A5910 + idx);
        ((cardgame_draw_t)*(uint32_t *)(p2 + 0xf08))(
            p2, 0xf, 10,
            pair[0],
            pair[1]);
        ((cardgame_step_t)*(uint32_t *)(p2 + 0xeb0))(p2, 4);
        ((cardgame_step_t)*(uint32_t *)(p2 + 0xeb0))(p2, 1);
        ((cardgame_step_t)*(uint32_t *)(p2 + 0xeb0))(p2, 2);
        ((cardgame_step_t)*(uint32_t *)(p2 + 0xeb0))(p2, 3);
        *(uint8_t *)(p1 + 0x422) = 5;
        *(int32_t *)(p1 + 0x424) = 0;
        *(int32_t *)(p1 + 0x428) = 0;
        break;
    }
    case 1:
        if (*(int16_t *)(p2 + 100) == 2 && *(uint8_t *)(p1 + 0x498) == 0) {
            int32_t idx = *(int8_t *)(p1 + 0x575) * 8;
            int32_t *pair = (int32_t *)((char *)DAT_800A5910 + idx);
            ((cardgame_draw_t)*(uint32_t *)(p2 + 0xf08))(
                p2, 0xf, 10,
                pair[0],
                pair[1]);
            *(uint8_t *)(p1 + 0x422) = 5;
            *(int32_t *)(p1 + 0x424) = 0;
        }
        break;
    case 5: {
        int32_t t = ((cardgame_tick_t)*(uint32_t *)0x8004df9c)();
        int i;
        t = *(int32_t *)(p1 + 0x424) += t;

        if (t <= 0xb)
            break;
        *(int32_t *)(p1 + 0x424) = 0;
        *(uint8_t *)(p1 + 0x422) = 6;
        ((cardgame_mark_t)*(uint32_t *)(p2 + 0xf1c))(p2, 0xf);
        for (i = 0; i < 0xf; i++) {
            if (*(int8_t *)(p1 + i + 0x46f) != 0)
                ((cardgame_mark_t)*(uint32_t *)(p2 + 0xf1c))(p2, i);
        }
        break;
    }
    case 6: {
        int32_t t = ((cardgame_tick_t)*(uint32_t *)0x8004df9c)();
        int i;
        t = *(int32_t *)(p1 + 0x424) += t;

        if (t <= 0x14)
            break;
        *(uint8_t *)(p1 + 0x422) = 7;
        *(int32_t *)(p1 + 0x424) = 0;
        ((cardgame_span_t)*(uint32_t *)(p2 + 0xf24))(p2, 0xf, 5, 0, 0x1000);
        for (i = 0; i < 0xf; i++) {
            if (*(int8_t *)(p1 + i + 0x46f) != 0) {
                int32_t offset = *(int8_t *)(p1 + 0x575) + i * 0x4c;
                int32_t addr = p2 + offset;
                *(uint8_t *)(addr + 0x146) = 1;
            }
        }
        break;
    }
    case 7: {
        int32_t t = ((cardgame_tick_t)*(uint32_t *)0x8004df9c)();
        t = *(int32_t *)(p1 + 0x424) += t;

        if (t <= 6)
            break;
        *(int32_t *)(p1 + 0x424) = 0;
        *(uint8_t *)(p2 + 0x5c2) = (uint8_t)(*(uint8_t *)(p1 + 0x575) + 1);
        ((cardgame_span_t)*(uint32_t *)(p2 + 0xf24))(p2, 0xf, 8, 0x1000, 0x1000);
        *(uint8_t *)(p1 + 0x422) = 8;
        ((cardgame_show_t)*(uint32_t *)(p2 + 0xea4))(
            p2, *(int8_t *)(p1 + 0x575), *(uint8_t *)(p1 + 0x579));
        break;
    }
    case 8: {
        int32_t t = ((cardgame_tick_t)*(uint32_t *)0x8004df9c)();
        cardgame_record76_t *src;
        cardgame_record76_t *dst;
        int i;
        t = *(int32_t *)(p1 + 0x424) += t;

        if (t <= 10)
            break;
        *(int32_t *)(p1 + 0x424) = 0;
        *(uint8_t *)(p1 + 0x422) = 9;
        src = (cardgame_record76_t *)(p2 + 0x57c);
        dst = (cardgame_record76_t *)(p2 + (*(int8_t *)(p1 + 0x575) + 12) * 76 + 0x108);
        *dst = *src;
        ((cardgame_done_t)*(uint32_t *)(p2 + 0xf18))(p2, 0xf);
        for (i = 0; i < 0xc; i++)
            *(uint8_t *)(p2 + i * 0x4c + 0x150) &= 0xfe;
        break;
    }
    case 9:
        ret = 1;
        break;
    default:
        break;
    }
    return ret;
}
