/* CARDGAME:0x80092e60, PAL-SLES-03936, full body 0x7c4 bytes.
 * Reference base 0x80082cb0; jump tables at 0x800835dc (96 bytes).
 * Native GCC 2.8.1 SN32 4.0.0010 / ASPSX 2.79, O2/G0.
 * Target-specific evidence: docs/c-matching-guide/submissions/cardgame-80092e60/.
 */
#include <stdint.h>

extern int32_t CARDGAME_F0x80092c54(uint8_t *a0, uint8_t *a1);
extern int32_t CARDGAME_F0x8008d854(uint8_t *a0, uint8_t *a1, int32_t a2, int32_t a3);
extern int32_t (*DAT_80055c48)(int32_t a0);
extern uint32_t DAT_8004B7D0[];
extern uint32_t DAT_8004DE10[];
extern int32_t DAT_8005ccb0;
extern uint32_t D0x800a5958[];

typedef void (*cardgame_cb0_t)(uint8_t *a0);
typedef void (*cardgame_cb1_t)(uint8_t *a0, int32_t a1);
typedef void (*cardgame_cb3_t)(uint8_t *a0, int32_t a1, int32_t a2, int32_t a3);
typedef void (*cardgame_cb4_t)(uint8_t *a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
typedef int32_t (*exe_getter0_t)(int32_t a0);
typedef int32_t (*exe_getter1_t)(int32_t a0, int32_t a1);
typedef int32_t (*exe_finalizer_t)(void);

typedef struct {
    uint8_t _pad0[0xea0];
    cardgame_cb3_t f_ea0;
    uint8_t _pad1[0xeb8 - 0xea4];
    cardgame_cb1_t f_eb8;
    uint8_t _pad2[0xec4 - 0xebc];
    cardgame_cb0_t f_ec4;
    cardgame_cb0_t f_ec8;
    uint8_t _pad3[0xee4 - 0xecc];
    cardgame_cb4_t f_ee4;
    cardgame_cb0_t f_ee8;
    uint8_t _pad4[0xf08 - 0xeec];
    cardgame_cb4_t f_f08;
    uint8_t _pad5[0xf1c - 0xf0c];
    cardgame_cb1_t f_f1c;
    cardgame_cb3_t f_f20;
    cardgame_cb4_t f_f24;
    cardgame_cb1_t f_f28;
} cardgame_92e60_tbl_t;

/* Local view of the six halfword accesses; not the complete object layout. */
typedef struct {
    uint8_t _pad0[0x600];
    int16_t values[6];
} cardgame_92e60_halfwords_t;

int32_t CARDGAME_F0x80092e60(uint8_t *a0, uint8_t *a1) {
    int32_t ret = 0;
    int32_t tmp;
    uint8_t b;

    if (a0[0x423] != 0) {
        switch (a0[0x423]) {
        case 1:
            ((cardgame_92e60_tbl_t *)a1)->f_ec8(a1);
            break;
        case 2:
            CARDGAME_F0x80092c54(a0, a1);
            break;
        case 3:
        case 4:
            *(int32_t *)(a0 + 0x424) = 0;
            *(int32_t *)(a0 + 0x428) = 0;
            *(int32_t *)(a0 + 0x434) = 0;
            break;
        case 5:
            *(int32_t *)(a0 + 0x424) = 0;
            *(int32_t *)(a0 + 0x428) = 0;
            *(int32_t *)(a0 + 0x434) = 0;
            ((cardgame_92e60_tbl_t *)a1)->f_ec4(a1);
            break;
        case 6:
            ((cardgame_92e60_tbl_t *)a1)->f_eb8(a1, 0x1000);
            ((cardgame_92e60_tbl_t *)a1)->f_ee4(a1, 0x21, 0, 1, 1);
            *(int32_t *)(a0 + 0x440) = 1;
            break;
        case 7:
            DAT_80055c48(0x6004001e);
            ((cardgame_92e60_tbl_t *)a1)->f_eb8(a1, 0x2000);
            ((cardgame_92e60_tbl_t *)a1)->f_ee4(a1, 0x22, 0, 1, 1);
            *(int32_t *)(a0 + 0x440) = 2;
            break;
        case 9:
            *(int32_t *)(a0 + 0x424) = 0;
            *(int32_t *)(a0 + 0x428) = 0;
            *(int32_t *)(a0 + 0x434) = 0;
            ((cardgame_92e60_tbl_t *)a1)->f_ec4(a1);
            ((cardgame_92e60_tbl_t *)a1)->f_ee8(a1);
            break;
        case 11:
            ((cardgame_92e60_tbl_t *)a1)->f_ee4(a1, *(int32_t *)(a0 + 0x2ec), 0, 0, 3);
            break;
        case 8:
        case 10:
        case 12:
        default:
            break;
        }
        b = a0[0x423];
        a0[0x423] = 0;
        a0[0x422] = b;
    }
    switch (a0[0x422]) {
    case 1: {
        int32_t sh = *(int16_t *)(a1 + 0x64);
        if (sh != 2)
            return ret;
        if (*(int16_t *)(a0 + 0x5a4) < 6) {
            a0[0x423] = 6;
        } else if (*(int16_t *)(a0 + 0x66c) < 6) {
            a0[0x423] = 7;
        } else {
            a0[0x423] = (uint8_t)sh;
        }
        break;
    }
    case 2:
        if (*(int32_t *)(a0 + 0x434) > 6) {
            uint8_t *p = a0;
            int32_t c = *(int32_t *)(p + 0x428);
            if (c < 6) {
                uint32_t *table_base = D0x800a5958;
                int32_t sc = c * 0x2900;
                uint32_t *row = &table_base[DAT_8005ccb0 * 4];
                ((cardgame_92e60_tbl_t *)a1)->f_f08(a1, *(int32_t *)(a0 + 0x428), 0x14,
                    row[0] + sc, row[1]);
                ((cardgame_92e60_tbl_t *)a1)->f_f20(a1, *(int32_t *)(a0 + 0x428), 0x1000, 0x1000);
                {
                    int32_t c2 = *(int32_t *)(a0 + 0x428);
                    uint32_t *row2 = &table_base[DAT_8005ccb0 * 4];
                    ((cardgame_92e60_tbl_t *)a1)->f_f08(a1, c2 + 6, 0x14,
                        row2[2] + c2 * 0x2900, row2[3]);
                }
                ((cardgame_92e60_tbl_t *)a1)->f_f20(a1, *(int32_t *)(a0 + 0x428) + 6, 0x1000, 0x1000);
                *(int32_t *)(a0 + 0x428) += 1;
                *(int32_t *)(a0 + 0x42c) -= 1;
                *(int32_t *)(a0 + 0x430) -= 1;
                ((cardgame_92e60_tbl_t *)a1)->f_ea0(a1, 0, 6, *(int32_t *)(a0 + 0x428));
                ((cardgame_92e60_tbl_t *)a1)->f_ea0(a1, 1, 6, *(int32_t *)(a0 + 0x428));
                ((cardgame_92e60_tbl_t *)a1)->f_ea0(a1, 0, 5, *(int32_t *)(a0 + 0x42c));
                ((cardgame_92e60_tbl_t *)a1)->f_ea0(a1, 1, 5, *(int32_t *)(a0 + 0x430));
            }
            *(int32_t *)(a0 + 0x434) -= 7;
        }
        if (*(int32_t *)(a0 + 0x424) >= 0x3d)
            a0[0x423] = 3;
        tmp = ((exe_finalizer_t)DAT_8004DE10[0x63])();
        *(int32_t *)(a0 + 0x424) += tmp;
        tmp = ((exe_finalizer_t)DAT_8004DE10[0x63])();
        *(int32_t *)(a0 + 0x434) += tmp;
        break;
    case 3:
        if (*(int32_t *)(a0 + 0x434) > 6) {
            if (*(int32_t *)(a0 + 0x428) < 6) {
                ((cardgame_92e60_tbl_t *)a1)->f_f28(a1, *(int32_t *)(a0 + 0x428));
                *(int32_t *)(a0 + 0x428) += 1;
            }
            *(int32_t *)(a0 + 0x434) -= 7;
        }
        if (*(int32_t *)(a0 + 0x424) >= 0x42)
            a0[0x423] = 4;
        tmp = ((exe_finalizer_t)DAT_8004DE10[0x63])();
        *(int32_t *)(a0 + 0x424) += tmp;
        tmp = ((exe_finalizer_t)DAT_8004DE10[0x63])();
        *(int32_t *)(a0 + 0x434) += tmp;
        break;
    case 4:
        if (*(int32_t *)(a0 + 0x434) > 6) {
            if (*(int32_t *)(a0 + 0x428) < 6) {
                if (CARDGAME_F0x8008d854(a0, a1, 0,
                        ((cardgame_92e60_halfwords_t *)a0)->values[*(int32_t *)(a0 + 0x428)]) != 0)
                    ((cardgame_92e60_tbl_t *)a1)->f_f1c(a1, *(int32_t *)(a0 + 0x428));
                if (CARDGAME_F0x8008d854(a0, a1, 1,
                        ((cardgame_92e60_halfwords_t *)(a0 + 0xc8))->values[*(int32_t *)(a0 + 0x428)]) != 0)
                    ((cardgame_92e60_tbl_t *)a1)->f_f1c(a1, *(int32_t *)(a0 + 0x428) + 6);
                *(int32_t *)(a0 + 0x428) += 1;
            }
            *(int32_t *)(a0 + 0x434) -= 7;
        }
        if (*(int32_t *)(a0 + 0x424) >= 0x51)
            a0[0x423] = 5;
        tmp = ((exe_finalizer_t)DAT_8004DE10[0x63])();
        *(int32_t *)(a0 + 0x424) += tmp;
        tmp = ((exe_finalizer_t)DAT_8004DE10[0x63])();
        *(int32_t *)(a0 + 0x434) += tmp;
        break;
    case 5:
        if (*(int32_t *)(a0 + 0x434) > 2) {
            if (*(int32_t *)(a0 + 0x428) < 6) {
                ((cardgame_92e60_tbl_t *)a1)->f_f24(a1, *(int32_t *)(a0 + 0x428), 5, 0, 0x1000);
                ((cardgame_92e60_tbl_t *)a1)->f_f24(a1, *(int32_t *)(a0 + 0x428) + 6, 5, 0, 0x1000);
                *(int32_t *)(a0 + 0x428) += 1;
            }
            *(int32_t *)(a0 + 0x434) -= 3;
        }
        if (*(int32_t *)(a0 + 0x424) >= 0x29)
            a0[0x423] = 10;
        tmp = ((exe_finalizer_t)DAT_8004DE10[0x63])();
        *(int32_t *)(a0 + 0x424) += tmp;
        tmp = ((exe_finalizer_t)DAT_8004DE10[0x63])();
        *(int32_t *)(a0 + 0x434) += tmp;
        break;
    case 6:
    case 7:
        if (a1[0xdfa] != 2)
            return ret;
        a0[0x423] = 8;
        break;
    case 8: {
        int32_t buttons;
        buttons = ((exe_getter0_t)DAT_8004B7D0[0xfd])(0) >> ((exe_getter1_t)DAT_8004B7D0[0x102])(0, 0xd);
        buttons &= 1;
        if (buttons != 0) {
            a0[0x423] = 9;
        } else {
            buttons = ((exe_getter0_t)DAT_8004B7D0[0xfd])(0) >> ((exe_getter1_t)DAT_8004B7D0[0x102])(0, 0xe);
            buttons &= 1;
            if (buttons != 0)
                a0[0x423] = 9;
            else
                return ret;
        }
        break;
    }
    case 9:
        if (a1[0xdfa] != 0)
            return ret;
        if (*(int16_t *)(a1 + 0x64) != 0)
            return ret;
        a0[0x423] = 10;
        break;
    case 10:
        if (*(int32_t *)(a0 + 0x440) != 2)
            ret = 1;
        else
            a0[0x423] = 11;
        break;
    case 11:
        if (a1[0xdfa] != 2)
            return ret;
        a0[0x423] = 12;
        break;
    case 12: {
        int32_t r0;
        r0 = ((exe_getter0_t)DAT_8004B7D0[0xfd])(0) >> ((exe_getter1_t)DAT_8004B7D0[0x102])(0, 0xd);
        r0 &= 1;
        if (r0 != 0) {
            ret = 1;
        } else {
            r0 = ((exe_getter0_t)DAT_8004B7D0[0xfd])(0) >> ((exe_getter1_t)DAT_8004B7D0[0x102])(0, 0xe);
            r0 &= 1;
            if (r0 == 0)
                return ret;
            ret = 1;
        }
        break;
    }
    default:
        break;
    }
    return ret;
}
