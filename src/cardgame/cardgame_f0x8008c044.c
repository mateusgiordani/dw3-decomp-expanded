// CARDGAME:0x8008c044, PAL-SLES-03936; 1452 text bytes + 44-byte table at 0x8008346c.
// Exact with PsyQ GCC 2.8.1 SN32 4.0.0010 + ASPSX 2.79, -O2 -G0 base.
// Evidence and controls: docs/c-matching-guide/submissions/cardgame-8008c044/strategy-r7.md.
// Pending/active dispatches are switches; distinct locals preserve their actual lifetimes.
// Indexed stores can overlap the signed header: reload it after each store.
// Keep the old signed element wide and each calculated address in value context.
// The duplicate store to ctx+0x421 is present in PAL. Callback targets remain indirect.
#include <stdint.h>

typedef void (*cardgame_c044_cb2_t)(int32_t, int32_t);
typedef void (*cardgame_c044_cb3_t)(int32_t, int32_t, int32_t);
typedef void (*cardgame_c044_cb4_t)(int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_c044_cb5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_c044_exe_vec_t)(uint32_t);
typedef int32_t (*cardgame_c044_vec0_t)(int32_t);
typedef uint32_t (*cardgame_c044_vec1_t)(int32_t, int32_t);
typedef void (*cardgame_c044_slot_t)(int32_t, int32_t, int32_t);

extern int32_t DAT_8004B7D0[];
extern int32_t DAT_800A5DA4;
extern int32_t DAT_800A5958[];
extern int32_t DAT_8005CCB0;
extern int CARDGAME_F0x800860d4(int32_t p1, int32_t p2, int32_t p3, int32_t p4, int32_t p5, int32_t p6);
extern int CARDGAME_F0x80086a30(int32_t p1, int32_t p2, int32_t p3);
extern void CARDGAME_F0x8008be04(int32_t p1, int32_t p2);
extern int CARDGAME_F0x80087dc8(int32_t p1, int32_t p2);
extern void CARDGAME_F0x80085de8(int32_t p1);

int CARDGAME_F0x8008c044(int32_t p1, int32_t p2, int32_t p3)
{
    int32_t ret = 0;
    int32_t b;
    int32_t v;

    if (*(uint8_t *)(p1 + 0x423) != 0) {
        switch (*(uint8_t *)(p1 + 0x423)) {
        case 1:
            v = *(int32_t *)(p1 + 0x438);
            *(uint8_t *)(p1 + 0x499) = *(uint8_t *)(p1 + 0x438);
            switch (v) {
            case 5:
                DAT_800A5DA4 = 0x19;
                break;
            case 7:
                DAT_800A5DA4 = 0x1a;
                break;
            case 0xd:
                DAT_800A5DA4 = 0x1d;
                break;
            case 9:
                DAT_800A5DA4 = 0x1b;
                break;
            case 0xb:
            case 0xf:
                DAT_800A5DA4 = 0x1c;
                break;
            }
            if (p3 == 2)
                ((cardgame_c044_cb2_t)*(uint32_t *)(p2 + 0xec0))(p2, 0);
            *(int32_t *)(p1 + 0x42c) = 0;
            *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x424) = 0;
            *(int32_t *)(p1 + 0x440) = 0;
            *(uint8_t *)(p1 + 0x444) = 0;
            break;
        case 3:
            (*(cardgame_c044_exe_vec_t)*(uint32_t *)0x80055c48)(0x4001c);
            *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x424) = 0;
            break;
        case 2:
            *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x424) = 0;
            break;
        case 4:
            if (p3 == 1)
                ((cardgame_c044_cb5_t)*(uint32_t *)(p2 + 0xf24))(p2, 0xf, 8, 0, 0x1000);
            *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x424) = 0;
            *(uint8_t *)(p1 + 0x499) = *(uint8_t *)(p1 + 0x49b);
            ((cardgame_c044_cb2_t)*(uint32_t *)(p2 + 0xeb0))(p2, 4);
            ((cardgame_c044_cb2_t)*(uint32_t *)(p2 + 0xeb0))(p2, 0);
            ((cardgame_c044_cb2_t)*(uint32_t *)(p2 + 0xeb0))(p2, 1);
            ((cardgame_c044_cb2_t)*(uint32_t *)(p2 + 0xeb0))(p2, 2);
            ((cardgame_c044_cb2_t)*(uint32_t *)(p2 + 0xeb0))(p2, 3);
            break;
        }
        {
            int32_t pending = *(uint8_t *)(p1 + 0x423);
            *(uint8_t *)(p1 + 0x423) = 0;
            *(uint8_t *)(p1 + 0x422) = (uint8_t)pending;
        }
    }
    b = *(uint8_t *)(p1 + 0x422);
    switch (b) {
    case 1: {
        int32_t idx;
        int32_t address;
        v = CARDGAME_F0x800860d4(p1, p2, 2, DAT_800A5DA4, *(int32_t *)(p1 + 0x424), *(int32_t *)(p1 + 0x42c));
        *(int32_t *)(p1 + 0x42c) = v;
        CARDGAME_F0x80086a30(p1, p2, 0);
        if ((*(int32_t *)(p1 + 0x424) == 2) && (p3 == 1)) {
            int32_t *table = DAT_800A5958;
            int32_t *pair = &table[DAT_8005CCB0 * 4];
            ((cardgame_c044_cb4_t)*(uint32_t *)(p2 + 0xf14))(p2, 0xf, pair[0], pair[1]);
            ((cardgame_c044_cb3_t)*(uint32_t *)(p2 + 0xf3c))(p2, 0xf, *(int16_t *)(p1 + (*(int8_t *)(p1 + 0x575) << 3) + 0x580));
            *(uint16_t *)(p2 + 0x594) = 0;
            ((cardgame_c044_cb5_t)*(uint32_t *)(p2 + 0xf24))(p2, 0xf, 8, 0x1000, 0x1000);
        }
        if (*(int32_t *)(p1 + 0x4d4) * 4 + 0xe < *(int32_t *)(p1 + 0x424)) {
            *(uint8_t *)(p1 + 0x423) = 2;
            ((cardgame_c044_cb5_t)*(uint32_t *)(p2 + 0xf0c))(p2, 0, 5, 0x1800, 0x5c00);
            idx = *(int32_t *)(p1 + 0x43c) * 76;
            *(uint8_t *)(p2 + idx + 0x150) |= 1;
            address = p2 + *(int32_t *)(p1 + 0x43c) * 76;
            *(uint16_t *)(address + 0x12e) = 1;
        }
        *(int32_t *)(p1 + 0x424) = *(int32_t *)(p1 + 0x424) + 1;
        break;
    }
    case 2:
        CARDGAME_F0x8008be04(p1, p2);
        if (p3 == 1) {
            int32_t bit = ((cardgame_c044_vec0_t)DAT_8004B7D0[0xfd])(0);
            uint32_t sel = ((cardgame_c044_vec1_t)DAT_8004B7D0[0x102])(0, 0xe);
            if (((bit >> sel) & 1) != 0) {
                (*(cardgame_c044_exe_vec_t)*(uint32_t *)0x80055c48)(0x800450bd);
                *(uint8_t *)(p1 + 0x445) = (uint8_t)p3;
                *(uint8_t *)(p1 + 0x423) = 4;
            }
        }
        CARDGAME_F0x80086a30(p1, p2, 0);
        break;
    case 3:
        if (CARDGAME_F0x80087dc8(p1, p2) != 0) {
            /* NOTE: constant 2 (s3), not p3: the sb executes before the p3==2 test. */
            *(uint8_t *)(p1 + 0x445) = 2;
            *(uint8_t *)(p1 + 0x423) = 4;
            if (p3 == 2)
                ((cardgame_c044_cb2_t)*(uint32_t *)(p2 + 0xebc))(p2, 0);
        }
        CARDGAME_F0x80086a30(p1, p2, 0);
        break;
    case 4: {
        int32_t selected;
        int32_t cur = (*(int32_t *)(p1 + 0x424))++;
        if (cur <= *(int32_t *)(p1 + 0x434) * 4 + 5)
            return ret;
        selected = *(int32_t *)(p1 + 0x43c);
        *(int32_t *)(p1 + 0x440) = selected;
        switch (*(int32_t *)(p1 + 0x438)) {
        case 7: {
            int32_t first_address, selected_address, reload_address;
            int32_t old;
            int16_t h = *(int16_t *)(p1 + 0x5a0);
            first_address = p1 + h * 2;
            old = *(int16_t *)(first_address + 0x5b0);
            selected_address = p1 + (h + selected) * 2;
            *(uint16_t *)(first_address + 0x5b0) = *(uint16_t *)(selected_address + 0x5b0);
            reload_address = p1 + (*(int16_t *)(p1 + 0x5a0) + *(int32_t *)(p1 + 0x440)) * 2;
            *(uint16_t *)(reload_address + 0x5b0) = (uint16_t)old;
            *(int32_t *)(p1 + 0x440) = *(int16_t *)(p1 + 0x5a0);
            ((cardgame_c044_slot_t)*(uint32_t *)(p1 + 0x810))(p1, *(int16_t *)(p1 + 0x5a0) + 1, *(int16_t *)(p1 + 0x5a4) - 1);
            break;
        }
        case 0xd: {
            int32_t address;
            address = p1 + (selected + *(int16_t *)(p1 + 0x668)) * 2;
            *(int32_t *)(p1 + 0x440) = *(uint8_t *)(address + 0x30a);
            CARDGAME_F0x80085de8(p1);
            break;
        }
        }
        *(uint8_t *)(p1 + *(int32_t *)(p1 + 0x440) + 0x46f) = 1;
        ret = *(uint8_t *)(p1 + 0x445);
        *(uint8_t *)(p1 + 0x423) = 0;
        *(uint8_t *)(p1 + 0x421) = 0;
        *(uint8_t *)(p1 + 0x420) = 0;
        *(uint8_t *)(p1 + 0x421) = 0;
        break;
    }
    }
    return ret;
}
