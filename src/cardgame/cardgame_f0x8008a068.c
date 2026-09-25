/* CARDGAME:0x8008a068..0x8008ad48, PAL-SLES-03936.
 * Two 15-entry dispatch tables occupy 124 bytes at 0x8008337c.
 * Expansion-local poll results preserve four-call ordering and real lifetimes.
 * Clamp temporaries belong to each mutually exclusive path; GCC merges the
 * resulting suffixes after allocation. See the r9 Astra submission for controls.
 * Callback names retain offsets; broader semantics remain uncertain. */
#include <stdint.h>

typedef int32_t (*cardgame_sys0_t)(int32_t);
typedef int32_t (*cardgame_sys1_t)(int32_t, int32_t);
typedef void (*cardgame_card0_t)(int32_t);
typedef void (*cardgame_card1_t)(int32_t, int32_t);
typedef void (*cardgame_card4_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_exe_vec_t)(uint32_t);

extern int32_t DAT_8004B7D0[];
extern int32_t CARDGAME_F0x80089ed4(int32_t p1, int32_t p2);
extern void CARDGAME_F0x80086a18(int32_t p1, int32_t p2);
extern int32_t CARDGAME_F0x800860d4(int32_t p1, int32_t p2, int32_t a2, int32_t a3, int32_t s0, int32_t s1);
extern void CARDGAME_F0x80086a30(int32_t p1, int32_t p2, int32_t a2);
extern void CARDGAME_F0x80089f18(int32_t p1, int32_t p2, int32_t a2, int32_t a3);
extern void CARDGAME_F0x80086d28(int32_t p1, int32_t p2, int32_t a2);
extern uint16_t D0x800a5da0[];
extern uint32_t D0x800a58a4[];
extern uint8_t D0x800a58e4[];

#define POLL(k) ({ uint8_t *_sv = (uint8_t *)DAT_8004B7D0; int32_t _b = (*(int32_t (**)(int32_t))(_sv+0x3f4))(0); int32_t _s = (*(int32_t (**)(int32_t,int32_t))(_sv+0x408))(0,k); (_b >> _s)&1; })
#define REPEAT(k) ({ uint8_t *_sv = (uint8_t *)DAT_8004B7D0; int32_t _b1 = (*(int32_t (**)(int32_t))(_sv+0x3f4))(0); int32_t _s1 = (*(int32_t (**)(int32_t,int32_t))(_sv+0x408))(0,k); int32_t _b2 = (*(int32_t (**)(int32_t))(_sv+0x3fc))(0); int32_t _s2 = (*(int32_t (**)(int32_t,int32_t))(_sv+0x408))(0,k); (_b1 & (1u << _s1)) | (_b2 & (1u << _s2)); })

int32_t CARDGAME_F0x8008a068(int32_t p1, int32_t p2)
{
    int32_t ret = 0;
    int32_t i;
    int32_t v;
    uint8_t c;

    if (*(uint8_t *)(p1 + 0x423) != 0) {
        switch (*(uint8_t *)(p1 + 0x423)) {
        case 1:
            D0x800a5da0[0] = (uint16_t)*(uint8_t *)(p2 + 0xb0);
            D0x800a5da0[1] = (uint16_t)*(uint8_t *)(p2 + 0x104);
            *(uint8_t *)(p2 + 0xb0) = 0;
            *(uint8_t *)(p2 + 0x104) = 0;
            *(uint8_t *)(p1 + 0x49d) = 1;
            *(uint8_t *)(p1 + 0x499) = 1;
            *(int32_t *)(p1 + 0x42c) = CARDGAME_F0x80089ed4(p1, p2);
            *(int32_t *)(p1 + 0x43c) = 0;
            ((cardgame_card0_t)*(uint32_t *)(p2 + 0xecc))(p2);
            ((cardgame_card0_t)*(uint32_t *)(p2 + 0xec8))(p2);
            CARDGAME_F0x80086a18(p1, p2);
            break;
        case 2:
            CARDGAME_F0x80086a18(p1, p2);
            *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x424) = 0;
            *(int32_t *)(p1 + 0x434) = 0;
            break;
        case 4:
            CARDGAME_F0x80086a18(p1, p2);
            v = 1;
            goto shared34;
        case 3:
            CARDGAME_F0x80086a18(p1, p2);
            v = 2;
shared34:
            *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x424) = 0;
            *(int32_t *)(p1 + 0x434) = v;
            break;
        case 8:
        {
            int32_t t;
            int32_t i;
            t = *(int32_t *)(p1 + 0x43c) * 76;
            *(uint8_t *)(p2 + t + 0x150) &= 0xfe;
            i = *(int32_t *)(p1 + 0x43c);
            t = i * 76;
            *(uint16_t *)(p2 + t + 0x12e) = 0;
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 4);
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 1);
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 2);
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 3);
            *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x424) = 0;
            break;
        }
        case 9:
        {
            int32_t t;
            int32_t i;
            t = (*(int32_t *)(p1 + 0x43c) + 12) * 76;
            *(uint8_t *)(p2 + t + 0x150) &= 0xfe;
            i = *(int32_t *)(p1 + 0x43c) + 12;
            t = i * 76;
            *(uint16_t *)(p2 + t + 0x12e) = 0;
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 4);
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 1);
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 2);
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 3);
            *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x424) = 0;
            break;
        }
        case 10:
        {
            int32_t t;
            int32_t i;
            t = (*(int32_t *)(p1 + 0x43c) + 6) * 76;
            *(uint8_t *)(p2 + t + 0x150) &= 0xfe;
            i = *(int32_t *)(p1 + 0x43c) + 6;
            t = i * 76;
            *(uint16_t *)(p2 + t + 0x12e) = 0;
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 4);
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 1);
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 2);
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 3);
            *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x424) = 0;
            break;
        }
        case 11:
            ((cardgame_card0_t)*(uint32_t *)(p2 + 0xec4))(p2);
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 4);
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 1);
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 2);
            ((cardgame_card1_t)*(uint32_t *)(p2 + 0xeb0))(p2, 3);
            *(uint8_t *)(p1 + 0x499) = 2;
            break;
        case 12:
            ((cardgame_card4_t)*(uint32_t *)(p2 + 0xee4))(p2, 23, 0, 0, 1);
            break;
        case 14:
            ((cardgame_card0_t)*(uint32_t *)(p2 + 0xee8))(p2);
            ((cardgame_card0_t)*(uint32_t *)(p2 + 0xec4))(p2);
            break;
        case 15:
            break;
        default:
            break;
        }
        {
            uint8_t pending = *(uint8_t *)(p1 + 0x423);
            *(uint8_t *)(p1 + 0x423) = 0;
            *(uint8_t *)(p1 + 0x422) = pending;
        }
    }
    switch (*(uint8_t *)(p1 + 0x422)) {
    case 1:
        if (*(int16_t *)(p2 + 100) == 2 && *(uint8_t *)(p1 + 0x498) == 0) {
            if (*(uint8_t *)(p1 + 0x72c) != 0) {
                *(uint8_t *)(p1 + 0x423) = 2;
                *(int32_t *)(p1 + 0x434) = 0;
            } else if (*(uint8_t *)(p1 + 0x79e) != 0) {
                *(uint8_t *)(p1 + 0x423) = 4;
                *(int32_t *)(p1 + 0x434) = 1;
            } else if (*(int8_t *)(p1 + 0x575) > 0) {
                *(uint8_t *)(p1 + 0x423) = 3;
                *(int32_t *)(p1 + 0x434) = 2;
            } else {
                *(uint8_t *)(p1 + 0x423) = 12;
                *(int32_t *)(p1 + 0x434) = 3;
            }
        }
        break;
    case 2:
    case 3:
    case 4:
        *(int32_t *)(p1 + 0x428) = CARDGAME_F0x800860d4(p1, p2, *(int32_t *)(p1 + 0x434), 0, *(int32_t *)(p1 + 0x424), *(int32_t *)(p1 + 0x428));
        CARDGAME_F0x80086a30(p1, p2, (int32_t)D0x800a58a4[*(int32_t *)(p1 + 0x434)]);
        i = *(int32_t *)(p1 + 0x424) + 1;
        *(int32_t *)(p1 + 0x424) = i;
        if (i < 11)
            break;
        CARDGAME_F0x80089f18(p1, p2, *(int32_t *)(p1 + 0x434), 0);
        *(uint8_t *)(p1 + 0x423) = D0x800a58e4[*(int32_t *)(p1 + 0x434) * 4];
        break;
    case 5:
    {
        if (POLL(4) != 0 &&
            (*(int8_t *)(p1 + 0x575) > 0 || *(uint8_t *)(p1 + 0x79e) != 0))
            *(uint8_t *)(p1 + 0x423) = 8;
        if ((REPEAT(5) != 0) &&
            (*(int32_t *)(p1 + 0x43c) < (int32_t)(*(uint8_t *)(p1 + 0x72c) - 1)))
            CARDGAME_F0x80089f18(p1, p2, 0, 1);
        if ((REPEAT(7) != 0) &&
            (0 < *(int32_t *)(p1 + 0x43c)))
            CARDGAME_F0x80089f18(p1, p2, 0, -1);
        CARDGAME_F0x80086d28(p1, p2, 0);
        goto exetest;
    }
    case 6:
    {
        if (POLL(6) != 0 &&
            (*(int8_t *)(p1 + 0x575) > 0 || *(uint8_t *)(p1 + 0x72c) != 0))
            *(uint8_t *)(p1 + 0x423) = 10;
        if ((REPEAT(5) != 0) &&
            (*(int32_t *)(p1 + 0x43c) < (int32_t)(*(uint8_t *)(p1 + 0x79e) - 1)))
            CARDGAME_F0x80089f18(p1, p2, 1, 1);
        if ((REPEAT(7) != 0) &&
            (0 < *(int32_t *)(p1 + 0x43c)))
            CARDGAME_F0x80089f18(p1, p2, 1, -1);
        CARDGAME_F0x80086d28(p1, p2, 1);
    }
exetest:
    {
        if (POLL(14) != 0) {
            ((cardgame_exe_vec_t)*(uint32_t *)0x80055c48)(0x800450bd);
            *(uint8_t *)(p1 + 0x423) = 11;
        }
        break;
    }
    case 7:
    {
        if (POLL(4) != 0) {
            if (*(uint8_t *)(p1 + 0x79e) != 0) {
                *(int32_t *)(p1 + 0x42c) = 1;
                *(uint8_t *)(p1 + 0x423) = 9;
            }
        } else {
            if (POLL(6) != 0 && *(uint8_t *)(p1 + 0x72c) != 0) {
                *(int32_t *)(p1 + 0x42c) = 0;
                *(uint8_t *)(p1 + 0x423) = 9;
            }
        }
        if ((REPEAT(5) != 0) &&
            (*(int32_t *)(p1 + 0x43c) < *(int8_t *)(p1 + 0x575) - 1))
            CARDGAME_F0x80089f18(p1, p2, 2, 1);
        if ((REPEAT(7) != 0) &&
            (0 < *(int32_t *)(p1 + 0x43c)))
            CARDGAME_F0x80089f18(p1, p2, 2, -1);
        if (POLL(14) != 0) {
            ((cardgame_exe_vec_t)*(uint32_t *)0x80055c48)(0x800450bd);
            *(uint8_t *)(p1 + 0x423) = 11;
        }
        CARDGAME_F0x80086d28(p1, p2, 2);
        break;
    }
    case 8:
        i = *(int32_t *)(p1 + 0x424) + 1;
        *(int32_t *)(p1 + 0x424) = i;
        if (i < 11) return ret;
        if (*(int8_t *)(p1 + 0x575) > 0) {
            int32_t n, pos;
            *(uint8_t *)(p1 + 0x423)=3;
            pos=*(int32_t *)(p1 + 0x43c)/2;
            *(int32_t *)(p1 + 0x43c)=pos;
            n=*(int8_t *)(p1 + 0x575);
            if (pos>n-1) *(int32_t *)(p1 + 0x43c)=n-1;
        } else {
            int32_t n;
            if (*(uint8_t *)(p1 + 0x79e) == 0) return ret;
            n=*(uint8_t *)(p1 + 0x79e);
            *(uint8_t *)(p1 + 0x423)=4;
            if (*(int32_t *)(p1 + 0x43c)>n-1) *(int32_t *)(p1 + 0x43c)=n-1;
        }
        break;
    case 9:
        i = *(int32_t *)(p1 + 0x424) + 1;
        *(int32_t *)(p1 + 0x424) = i;
        if (i < 11) return ret;
        *(int32_t *)(p1 + 0x43c) = *(int32_t *)(p1 + 0x43c) * 2 + 1;
        if (*(int32_t *)(p1 + 0x42c) != 0) {
            int32_t n=*(uint8_t *)(p1 + 0x79e);
            *(uint8_t *)(p1 + 0x423)=4;
            if (*(int32_t *)(p1 + 0x43c)>n-1) *(int32_t *)(p1 + 0x43c)=n-1;
        } else {
            int32_t n=*(uint8_t *)(p1 + 0x72c);
            *(uint8_t *)(p1 + 0x423)=2;
            if (*(int32_t *)(p1 + 0x43c)>n-1) *(int32_t *)(p1 + 0x43c)=n-1;
        }
        break;
    case 10:
        i = *(int32_t *)(p1 + 0x424) + 1;
        *(int32_t *)(p1 + 0x424) = i;
        if (i < 11) return ret;
        if (*(int8_t *)(p1 + 0x575) > 0) {
            int32_t n, pos;
            *(uint8_t *)(p1 + 0x423)=3;
            pos=*(int32_t *)(p1 + 0x43c)/2;
            *(int32_t *)(p1 + 0x43c)=pos;
            n=*(int8_t *)(p1 + 0x575);
            if (pos>n-1) *(int32_t *)(p1 + 0x43c)=n-1;
        } else {
            int32_t n;
            if (*(uint8_t *)(p1 + 0x72c) == 0) return ret;
            n=*(uint8_t *)(p1 + 0x72c);
            *(uint8_t *)(p1 + 0x423)=2;
            if (*(int32_t *)(p1 + 0x43c)>n-1) *(int32_t *)(p1 + 0x43c)=n-1;
        }
        break;
    case 11:
        if (*(int16_t *)(p2 + 100) != 0)
            return ret;
        c = *(uint8_t *)(p1 + 0x498);
        goto shared_be;
    case 12:
        if (*(uint8_t *)(p2 + 0xdfa) != 2)
            return ret;
        *(uint8_t *)(p1 + 0x423) = 13;
        break;
    case 13:
    {
        if (POLL(13) == 0) {
            if (POLL(14) == 0)
                return ret;
        }
        *(uint8_t *)(p1 + 0x423) = 14;
        break;
    }
    case 14:
        c = *(uint8_t *)(p2 + 0xdfa);
shared_be:
        if (c == 0)
            *(uint8_t *)(p1 + 0x423) = 15;
        break;
    case 15:
        *(uint8_t *)(p2 + 0xb0) = *(uint8_t *)&D0x800a5da0[0];
        *(uint8_t *)(p2 + 0x104) = *(uint8_t *)&D0x800a5da0[1];
        ret = 1;
        break;
    default:
        break;
    }
    return ret;
}
