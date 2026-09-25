/*
 * CARDGAME:0x80097acc CARDGAME_F0x80097acc
 * 2496 bytes at CARDGAME.PRO offset 0x14e1c (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x80097acc
 *  Symbols     CARDGAME_F0x80097958=0x80097958 CARDGAME_F0x80097a24=0x80097a24
 *              D0x800a59b0=0x800a59b0 DAT_80044f4c=0x80044f4c
 *              DAT_8004df9c=0x8004df9c DAT_8005cca8=0x8005cca8
 *              D_80044B38=0x80044b38 func_0x8001fcc0=0x8001fcc0
 *              func_0x8002abcc=0x8002abcc
 *  Compare     2496 bytes from 0x80097acc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80097acc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL-SLES-03936, [0x80097acc, 0x8009848c).
 *
 * Local row view has stride 8; indirect callback identities remain slot-based.
 *
 * Keep the independent row/base and scratch lifetimes: GCC 2.8.1 uses them.
 */

#include <stdint.h>
typedef struct {
    int32_t x, y;
} position_row_t;

typedef void (*cardgame_v0_t)(int32_t a0);
typedef void (*cardgame_v1_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_v2_t)(int32_t a0, int32_t a1, int32_t a2);
typedef int32_t (*cardgame_r1_t)(int32_t a0, int32_t a1, int32_t a2);
typedef int32_t (*cardgame_svc_t)(int32_t a0);
typedef int32_t (*cardgame_tick_t)(void);

extern int32_t CARDGAME_F0x80097a24(int32_t a0, int32_t *a1, int32_t a2, int32_t a3);
extern void CARDGAME_F0x80097958(int32_t a0, int32_t *a1, int32_t a2, int32_t a3, int32_t a4);
extern int32_t func_0x8002abcc(int32_t a0);
extern void func_0x8001fcc0(void *buf);
extern cardgame_svc_t DAT_80044f4c;
extern uint8_t D_80044B38[];
extern cardgame_tick_t DAT_8004df9c;
extern uint32_t DAT_8005cca8;
extern uint8_t D0x800a59b0[];

void CARDGAME_F0x80097acc(int32_t p0, int32_t *p1)
{
    int32_t value_or_resource;
    int16_t ratio;
    int32_t x_offset;
    int32_t y_offset;
    int32_t count;
    int32_t callbacks[3];
    int32_t *entry_cursor;
    uint8_t *record_cursor;
    int32_t state;
    int32_t sub;

    if (*(uint8_t *)(p0 + 0xdfa) == 0)
        return;
    state = *(uint8_t *)(p0 + 0xdfa);
    if (state == 2)
        goto L_case2;
    if (state < 3)
        goto L_dflt;
    if (state == 4)
        goto L_case4;
    if (state == 5)
        goto L_case5;
L_dflt: {
    int32_t q;

    q = (int32_t)*(int16_t *)(p0 + 0xdf0) << 12;
    if (*(int16_t *)(p0 + 0xdf2) != 0) {
        int32_t quotient = q / *(int16_t *)(p0 + 0xdf2);
        ratio = 0x1000 - quotient;
    } else {
        ratio = 0x1000 - q;
    }
    {
        int32_t elapsed, remaining;
        elapsed = DAT_8004df9c();
        remaining = *(uint16_t *)(p0 + 0xdf0) - elapsed;
        *(int16_t *)(p0 + 0xdf0) = remaining;
        remaining <<= 16;
        if (remaining > 0)
            goto L_tail;
    }
}
    sub = *(uint8_t *)(p0 + 0xdfb);
    /* This value is consumed by the state store before the resource calls. */
    value_or_resource = 2;
    *(uint8_t *)(p0 + 0xdfa) = value_or_resource;
    if (sub == 1)
        goto L_sub1;
    if (sub < 2)
        goto L_subelse;
    if (sub < 4)
        goto L_midB;
L_subelse:
    ((cardgame_v1_t) * (void **)(p1[13] + 0x144))(p1[13], 0);
    ((cardgame_v1_t) * (void **)(p1[0] + 0x80))(p1[0], 0);
    goto L_subjoin;
L_sub1:
    ((cardgame_v1_t) * (void **)(p1[0] + 0x80))(p1[0], 1);
    {
        position_row_t *position = &((position_row_t *)D0x800a59b0)[*(int32_t *)(p0 + 0xde4)];
        int32_t y = *(int16_t *)(p0 + 0xdf4) * 14 + 0x11;
        ((cardgame_v2_t) * (void **)(p1[0] + 0x84))(p1[0], 0x14, position->y + y);
    }
    value_or_resource = DAT_80044f4c(DAT_8005cca8 + 15);
    ((cardgame_v2_t) * (void **)(p1[13] + 0x114))(p1[13], value_or_resource, 0x18);
    {
        uint8_t *wA;

        wA = D0x800a59b0 + (*(int32_t *)(p0 + 0xde4)) * 8;
        ((cardgame_v2_t) * (void **)(p1[13] + 0x134))(p1[13], (int16_t)(((uint16_t *)wA)[0] + 0x1b),
                                                      (int16_t)(((uint16_t *)wA)[2] + 0x12));
    }
    goto L_subjoin;
L_midB: {
    uint8_t *svcB;
    uint8_t *tableB;

    ((cardgame_v1_t) * (void **)(p1[0] + 0x80))(p1[0], 1);
    /* The immutable base starts after the callback; each row is freshly read. */
    tableB = D0x800a59b0;
    {
        position_row_t *position = &((position_row_t *)tableB)[*(int32_t *)(p0 + 0xde4)];
        int32_t y = *(int16_t *)(p0 + 0xdf4) * 14 + 0x11;
        ((cardgame_v2_t) * (void **)(p1[0] + 0x84))(p1[0], 0x14, position->y + y);
    }
    svcB = D_80044B38;
    value_or_resource = (*(cardgame_svc_t *)(svcB + 0x414))(DAT_8005cca8 + 15);
    ((cardgame_v2_t) * (void **)(p1[13] + 0x114))(p1[13], value_or_resource, 0x45);
    if (*(uint8_t *)(p0 + 0xdfb) == 3)
        ((cardgame_v1_t) * (void **)(p1[13] + 0x138))(p1[13], 7);
    {
        uint8_t *wB;

        wB = tableB + (*(int32_t *)(p0 + 0xde4)) * 8;
        ((cardgame_v2_t) * (void **)(p1[13] + 0x134))(p1[13], (int16_t)(((uint16_t *)wB)[0] + 0x1b),
                                                      (int16_t)(((uint16_t *)wB)[2] + 0x12));
    }
    value_or_resource = (*(cardgame_svc_t *)(svcB + 0x414))(DAT_8005cca8 + 15);
    ((cardgame_v2_t) * (void **)(p1[1] + 0x114))(p1[1], value_or_resource, 0x46);
    ((cardgame_v1_t) * (void **)(p1[1] + 0x148))(p1[1], 0);
    {
        uint8_t *wB2;

        wB2 = tableB + (*(int32_t *)(p0 + 0xde4)) * 8;
        ((cardgame_v2_t) * (void **)(p1[1] + 0x134))(p1[1], (int16_t)(((uint16_t *)wB2)[0] + 0x1b),
                                                     (int16_t)(((uint16_t *)wB2)[2] + 0x20));
    }
}
L_subjoin:;
    if (*(int32_t *)(p0 + 0xde4) == 3) {
        uint8_t *svcC;
        uint8_t *tableC = D0x800a59b0;

        ((cardgame_v2_t) * (void **)(p1[7] + 0x134))(p1[7],
                                                     (int16_t)(*(uint16_t *)(tableC + 0x18) + 0x40),
                                                     (int16_t)(*(uint16_t *)(tableC + 0x1c) + 4));
        svcC = D_80044B38;
        value_or_resource = (*(cardgame_svc_t *)(svcC + 0x414))(DAT_8005cca8 + 15);
        ((cardgame_v2_t) * (void **)(p1[7] + 0x114))(p1[7], value_or_resource, 0x42);
        value_or_resource = (*(cardgame_svc_t *)(svcC + 0x414))(DAT_8005cca8 + 0x6a);
        ((cardgame_v2_t) * (void **)(p1[13] + 0x114))(p1[13], value_or_resource,
                                                      *(int32_t *)(p0 + 0xde8));
        {
            uint16_t *row = (uint16_t *)(tableC + (*(int32_t *)(p0 + 0xde4)) * 8);
            ((cardgame_v2_t) * (void **)(p1[13] + 0x134))(p1[13], (int16_t)(row[0] + 0x40),
                                                          (int16_t)(row[2] + 0x12));
        }
    } else {
        if (*(int32_t *)(p0 + 0xde8) == 0)
            goto L_empty;
        {
            uint8_t *svcD;
            uint8_t *tableD = D0x800a59b0;

            {
                uint16_t *row = (uint16_t *)(tableD + (*(int32_t *)(p0 + 0xde4)) * 8);
                ((cardgame_v2_t) * (void **)(p1[7] + 0x134))(p1[7], (int16_t)(row[0] + 0x1b),
                                                             (int16_t)(row[2] + 4));
            }
            svcD = D_80044B38;
            value_or_resource = (*(cardgame_svc_t *)(svcD + 0x414))(DAT_8005cca8 + 15);
            ((cardgame_v2_t) * (void **)(p1[7] + 0x114))(p1[7], value_or_resource,
                                                         *(int32_t *)(p0 + 0xde8));
            if (*(int32_t *)(p0 + 0xde8) == 0x13) {
                uint8_t *loopTable;
                count = 0;
                value_or_resource = (*(cardgame_svc_t *)(svcD + 0x414))(DAT_8005cca8 + 15);
                ((cardgame_v2_t) * (void **)(p1[13] + 0x114))(p1[13], value_or_resource, 0x2b);
                /* Separate base lifetime covers this call and the following loop. */
                loopTable = tableD;
                {
                    uint16_t *row = (uint16_t *)(loopTable + (*(int32_t *)(p0 + 0xde4)) * 8);
                    ((cardgame_v2_t) * (void **)(p1[13] + 0x134))(p1[13], (int16_t)(row[0] + 0x1b),
                                                                  (int16_t)(row[2] + 0x12));
                }
                func_0x8001fcc0(callbacks);
                y_offset = 0x12;
                x_offset = ((cardgame_r1_t)callbacks[1])(p1[13] + 0x5c, *(int32_t *)(p1[13] + 0x50),
                                                         *(int16_t *)(p1[13] + 0xb4)) +
                           0x1b;
                record_cursor = (uint8_t *)p0;
                entry_cursor = p1;
                do {
                    uint8_t *w;

                    count += 1;
                    {
                        int32_t offset = (*(int32_t *)(p0 + 0xde4)) * 8;
                        w = (uint8_t *)(offset + (int32_t)loopTable);
                    }
                    ((cardgame_v2_t) * (void **)(entry_cursor[1] + 0x134))(
                        entry_cursor[1], (int16_t)(((uint16_t *)w)[0] + x_offset + 0xe),
                        (int16_t)(((uint16_t *)w)[2] + y_offset));
                    ((cardgame_v2_t) * (void **)(entry_cursor[1] + 0x118))(
                        entry_cursor[1], 0, *(int32_t *)(record_cursor + 0xa4));
                    record_cursor += 0x54;
                    y_offset += 0xe;
                    ((cardgame_v1_t) * (void **)(entry_cursor[1] + 0x148))(entry_cursor[1], 0);
                    entry_cursor += 1;
                } while (count < 2);
            } else if (*(int32_t *)(p0 + 0xde8) == 0x3e) {
                {
                    uint16_t *row = (uint16_t *)(tableD + (*(int32_t *)(p0 + 0xde4)) * 8);
                    ((cardgame_v2_t) * (void **)(p1[1] + 0x134))(p1[1], (int16_t)(row[0] + 0x3e),
                                                                 (int16_t)(row[2] + 4));
                }
                ((cardgame_v2_t) * (void **)(p1[1] + 0x118))(p1[1], 0, *(int16_t *)(p0 + 0x5e));
                ((cardgame_v1_t) * (void **)(p1[1] + 0x148))(p1[1], 1);
                value_or_resource = (*(cardgame_svc_t *)(svcD + 0x414))(DAT_8005cca8 + 0x2b);
                ((cardgame_v2_t) * (void **)(p1[2] + 0x114))(p1[2], value_or_resource,
                                                             (*(int16_t *)(p0 + 0x5c) + 1) / 2);
                {
                    uint16_t *row = (uint16_t *)(tableD + (*(int32_t *)(p0 + 0xde4)) * 8);
                    ((cardgame_v2_t) * (void **)(p1[2] + 0x134))(p1[2], (int16_t)(row[0] + 0x1b),
                                                                 (int16_t)(row[2] + 0x12));
                }
            }
        }
    }
    goto L_tail;
L_empty:
    ((cardgame_v1_t) * (void **)(p1[7] + 0x144))(p1[7], 0);
    goto L_tail;
L_case2:
    ratio = 0x1000;
    if (*(int32_t *)(p0 + 0xde4) == 3)
        CARDGAME_F0x80097a24(p0, p1, 0x1c, 100);
    if (*(uint8_t *)(p0 + 0xdfb) != 0) {
        {
            position_row_t *position = &((position_row_t *)D0x800a59b0)[*(int32_t *)(p0 + 0xde4)];
            int32_t y = *(int16_t *)(p0 + 0xdf4) * 14 + 0x11;
            ((cardgame_v2_t) * (void **)(p1[0] + 0x84))(p1[0], 0x14, position->y + y);
        }
    }
    goto L_tail;
L_case4: {
    int32_t dv;

    dv = func_0x8002abcc(((int32_t)*(int16_t *)(p0 + 0xdf0) << 12) / 20);
    ratio = 0x1000;
    if (dv < 0)
        dv += 511;
    dv >>= 9;
    {
        position_row_t *position = &((position_row_t *)D0x800a59b0)[*(int32_t *)(p0 + 0xde4)];
        int32_t y = *(int16_t *)(p0 + 0xdf4) * 14 + 0x11;
        ((cardgame_v2_t) * (void **)(p1[0] + 0x84))(p1[0], 0x14 - dv, position->y + y);
    }
}
    {
        int32_t q;

        q = DAT_8004df9c();
        q = *(uint16_t *)(p0 + 0xdf0) - q;
        *(int16_t *)(p0 + 0xdf0) = (int16_t)q;
        if ((int16_t)q > 0)
            goto L_tail;
    }
    ((cardgame_v1_t) * (void **)(p1[0] + 0x80))(p1[0], 0);
    ((cardgame_v0_t) * (void **)(p0 + 0xee8))(p0);
    ((cardgame_v1_t) * (void **)(p1[0] + 0x8c))(p1[0], 0x20);
    *(uint8_t *)(p0 + 0xdfa) = 5;
    goto L_tail;
L_case5:
    ((cardgame_v1_t) * (void **)(p1[0] + 0x80))(p1[0], 0);
    ((cardgame_v1_t) * (void **)(p1[7] + 0x144))(p1[7], 0);
    ((cardgame_v1_t) * (void **)(p1[13] + 0x138))(p1[13], 0);
    ((cardgame_v1_t) * (void **)(p1[13] + 0x144))(p1[13], 0);
    ((cardgame_v1_t) * (void **)(p1[1] + 0x144))(p1[1], 0);
    ((cardgame_v1_t) * (void **)(p1[2] + 0x144))(p1[2], 0);
    ((cardgame_v1_t) * (void **)(p1[3] + 0x144))(p1[3], 0);
    {
        int32_t q;

        q = DAT_8004df9c();
        q = *(uint16_t *)(p0 + 0xdf0) - q;
        *(int16_t *)(p0 + 0xdf0) = (int16_t)q;
        if ((int16_t)q <= 0)
            *(uint8_t *)(p0 + 0xdfa) = 0;
        ratio = ((int32_t)*(int16_t *)(p0 + 0xdf0) << 12) / *(int16_t *)(p0 + 0xdf2);
    }
    goto L_tail;
L_tail:
    CARDGAME_F0x80097958(p0, p1, (ratio << 16) >> 16,
                         *(int32_t *)(D0x800a59b0 + (*(int32_t *)(p0 + 0xde4)) * 8),
                         ((position_row_t *)D0x800a59b0)[*(int32_t *)(p0 + 0xde4)].y);
}
