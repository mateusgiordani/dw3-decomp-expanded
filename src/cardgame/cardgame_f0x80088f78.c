#include <stdint.h>

typedef void (*cardgame_8f78_f28_t)(int32_t ctx, int32_t arg);
typedef void (*cardgame_8f78_ea0_t)(int32_t ctx, int32_t a, int32_t b, int32_t c);
typedef void (*cardgame_8f78_ec4_t)(int32_t ctx);
typedef int32_t (*cardgame_sys0_t)(int32_t);
typedef int32_t (*cardgame_sys1_t)(int32_t, int32_t);
typedef int32_t (*cardgame_poll_t)(void);
typedef void (*cardgame_exe_vec_t)(uint32_t arg);

extern int32_t DAT_8004B7D0[];
extern int32_t DAT_8004DE10[];

int32_t CARDGAME_F0x80088f78(int32_t p1, int32_t p2)
{
    int32_t ret = 0;
    int32_t v;
    int32_t lim;
    int32_t any = 0;
    int32_t b0;
    int32_t b1;
    int32_t sh;

    switch (*(uint8_t *)(p1 + 0x422)) {
    case 1:
        if (*(int16_t *)(p2 + 100) == 2 && *(uint8_t *)(p1 + 0x498) == 0) {
            if ((uint32_t)*(uint8_t *)(p1 + 0x72c) + (uint32_t)*(uint8_t *)(p1 + 0x79e) == 0) {
                *(uint8_t *)(p1 + 0x422) = 3;
                *(int32_t *)(p1 + 0x424) = 0;
                *(int32_t *)(p1 + 0x428) = 0;
                *(int32_t *)(p1 + 0x42c) = 0;
                *(int32_t *)(p1 + 0x430) = 0;
                *(int32_t *)(p1 + 0x434) = 0;
            } else {
                *(uint8_t *)(p1 + 0x422) = 2;
                *(int32_t *)(p1 + 0x424) = 0;
                *(int32_t *)(p1 + 0x428) = 0;
                *(int32_t *)(p1 + 0x434) = 0;
                if ((uint32_t)*(uint8_t *)(p1 + 0x72c) > (uint32_t)*(uint8_t *)(p1 + 0x79e))
                    *(int32_t *)(p1 + 0x42c) = (uint32_t)*(uint8_t *)(p1 + 0x72c);
                else
                    *(int32_t *)(p1 + 0x42c) = (uint32_t)*(uint8_t *)(p1 + 0x79e);
                *(int32_t *)(p1 + 0x42c) = *(int32_t *)(p1 + 0x42c) * 6 + 0x12;
            }
        }
        break;
    case 2:
        if (*(int32_t *)(p1 + 0x434) > 5) {
            if (*(int32_t *)(p1 + 0x428) < *(uint8_t *)(p1 + 0x79e))
                ((cardgame_8f78_f28_t)*(uint32_t *)(p2 + 0xf28))(p2, *(int32_t *)(p1 + 0x428) + 6);
            *(int32_t *)(p1 + 0x428) += 1;
            *(int32_t *)(p1 + 0x434) -= 6;
        }
        if (*(int32_t *)(p1 + 0x424) > *(int32_t *)(p1 + 0x42c)) {
            *(uint8_t *)(p1 + 0x422) = 3;
            *(int32_t *)(p1 + 0x424) = 0;
            *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x42c) = 0;
            *(int32_t *)(p1 + 0x430) = 0;
            *(int32_t *)(p1 + 0x434) = 0;
        }
        v = ((cardgame_poll_t)DAT_8004DE10[0x63])();
        *(int32_t *)(p1 + 0x424) += v;
        v = ((cardgame_poll_t)DAT_8004DE10[0x63])();
        *(int32_t *)(p1 + 0x434) += v;
        break;
    case 3:
        {
            int32_t quotient;
            int32_t count;
            quotient = (int16_t)(*(int16_t *)(p1 + 0x59c) / 60);
            count = *(int32_t *)(p1 + 0x428);
            *(int32_t *)(p1 + 0x428) = quotient == 0 ? count + 1 : count + quotient;
            lim = *(int16_t *)(p1 + 0x59c);
            if (*(int32_t *)(p1 + 0x428) > lim)
                *(int32_t *)(p1 + 0x428) = lim;
            else
                any = 1;
        }
        {
            int32_t quotient;
            int32_t count;
            quotient = (int16_t)(*(int16_t *)(p1 + 0x59e) / 60);
            count = *(int32_t *)(p1 + 0x42c);
            *(int32_t *)(p1 + 0x42c) = quotient == 0 ? count + 1 : count + quotient;
            lim = *(int16_t *)(p1 + 0x59e);
            if (*(int32_t *)(p1 + 0x42c) > lim)
                *(int32_t *)(p1 + 0x42c) = lim;
            else
                any = 1;
        }
        {
            int32_t quotient;
            int32_t count;
            quotient = (int16_t)(*(int16_t *)(p1 + 0x664) / 60);
            count = *(int32_t *)(p1 + 0x430);
            *(int32_t *)(p1 + 0x430) = quotient == 0 ? count + 1 : count + quotient;
            lim = *(int16_t *)(p1 + 0x664);
            if (*(int32_t *)(p1 + 0x430) > lim)
                *(int32_t *)(p1 + 0x430) = lim;
            else
                any = 1;
        }
        {
            int32_t quotient;
            int32_t count;
            quotient = (int16_t)(*(int16_t *)(p1 + 0x666) / 60);
            count = *(int32_t *)(p1 + 0x434);
            *(int32_t *)(p1 + 0x434) = quotient == 0 ? count + 1 : count + quotient;
            lim = *(int16_t *)(p1 + 0x666);
            if (*(int32_t *)(p1 + 0x434) > lim)
                *(int32_t *)(p1 + 0x434) = lim;
            else
                any = 1;
        }
        if ((*(int32_t *)(p1 + 0x424))++ >= 0x3d) {
            *(uint8_t *)(p1 + 0x422) = 4;
            *(int32_t *)(p1 + 0x424) = 0;
            *(int32_t *)(p1 + 0x428) = *(int16_t *)(p1 + 0x59c);
            *(int32_t *)(p1 + 0x42c) = *(int16_t *)(p1 + 0x59e);
            *(int32_t *)(p1 + 0x430) = *(int16_t *)(p1 + 0x664);
            *(int32_t *)(p1 + 0x434) = *(int16_t *)(p1 + 0x666);
        } else {
            if (any)
                ((cardgame_exe_vec_t)*(uint32_t *)0x80055c48)(0x80040000 | 0x52c6);
        }
        ((cardgame_8f78_ea0_t)*(uint32_t *)(p2 + 0xea0))(p2, 0, 8, *(int32_t *)(p1 + 0x428));
        ((cardgame_8f78_ea0_t)*(uint32_t *)(p2 + 0xea0))(p2, 0, 9, *(int32_t *)(p1 + 0x42c));
        ((cardgame_8f78_ea0_t)*(uint32_t *)(p2 + 0xea0))(p2, 1, 8, *(int32_t *)(p1 + 0x430));
        ((cardgame_8f78_ea0_t)*(uint32_t *)(p2 + 0xea0))(p2, 1, 9, *(int32_t *)(p1 + 0x434));
        break;
    case 4:
        b0 = ((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0);
        sh = ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 13);
        if (((b0 >> (sh)) & 1) != 0) {
            *(int32_t *)(p1 + 0x424) = 0x5a;
        } else {
            b1 = ((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0);
            sh = ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 14);
            if (((b1 >> (sh)) & 1) != 0)
                *(int32_t *)(p1 + 0x424) = 0x5a;
        }
        if (++*(int32_t *)(p1 + 0x424) > 0x5a) {
            ((cardgame_8f78_ec4_t)*(uint32_t *)(p2 + 0xec4))(p2);
            *(uint8_t *)(p1 + 0x422) = 5;
            *(uint8_t *)(p1 + 0x499) = 2;
        }
        break;
    case 5:
        if (*(int16_t *)(p2 + 100) == 0 && *(uint8_t *)(p1 + 0x498) == 0)
            ret = 1;
        break;
    }
    return ret;
}
