/*
 * CARDGAME:0x80087edc CARDGAME_F0x80087edc
 * 4144 bytes at CARDGAME.PRO offset 0x522c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80087edc, jump table (.rodata) at 0x800832ac
 *  Symbols     CARDGAME_F0x80085fd0=0x80085fd0 CARDGAME_F0x800860d4=0x800860d4
 *              CARDGAME_F0x80086a30=0x80086a30 CARDGAME_F0x8008722c=0x8008722c
 *              CARDGAME_F0x800875ac=0x800875ac CARDGAME_F0x800878b4=0x800878b4
 *              CARDGAME_F0x80087b80=0x80087b80 CARDGAME_F0x80087dc8=0x80087dc8
 *              CARDGAME_F0x80087edc=0x80087edc DAT_8004B7D0=0x8004b7d0
 *              DAT_8004df9c=0x8004df9c DAT_80055c48=0x80055c48
 *              DAT_800a5d9c=0x800a5d9c FUN_80087308=0x80087308
 *  Compare     4144 bytes from 0x80087edc and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80087edc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL base 0x80082cb0, size 0x1030.
 *
 * Recovered from CARDGAME disassembly before decompiler review. The state byte
 * is p1+0x422; pending transitions are p1+0x423.
 */

#include <stdint.h>

typedef int32_t (*cg0_t)(int32_t);
typedef int32_t (*cg_tick_t)(void);
typedef int32_t (*cg1_t)(int32_t);
typedef int32_t (*cg2_t)(int32_t, int32_t);
typedef int32_t (*cg3_t)(int32_t, int32_t, int32_t);
typedef int32_t (*cg5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef int32_t (*cg6_t)();

extern int32_t DAT_800a5d9c;
extern cg1_t DAT_80055c48;
extern cg_tick_t DAT_8004df9c;
extern int32_t DAT_8004B7D0[];
extern int32_t CARDGAME_F0x80085fd0(int32_t, int32_t, int32_t);
extern int32_t CARDGAME_F0x800860d4(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
extern void CARDGAME_F0x80086a30(int32_t, int32_t, int32_t);
extern void CARDGAME_F0x8008722c(int32_t, int32_t, int32_t);
extern int32_t FUN_80087308(int32_t, int32_t, int32_t);
extern void CARDGAME_F0x800875ac(int32_t, int32_t, int32_t);
extern void CARDGAME_F0x800878b4(int32_t, int32_t, int32_t);
extern void CARDGAME_F0x80087b80(int32_t, int32_t, int32_t);
extern int32_t CARDGAME_F0x80087dc8(int32_t, int32_t);

/*
 * Partial layout views: observed fields repeat every 0x4c bytes.
 */
typedef struct { uint8_t unknown[0x150]; uint8_t fields[1][0x4c]; } cg_flags_view_t;
typedef struct { uint8_t unknown[0x12e]; int16_t fields[1][0x26]; } cg_selected_view_t;
typedef struct { uint8_t unknown[0x14a]; uint8_t fields[1][0x4c]; } cg_state_view_t;

#define CG_SLOT(p, off) (*(int32_t *)((p) + (off)))
#define CG_ENTRY(p, n) ((p) + (n) * 0x4c)
#define cg_slot CG_SLOT
#define cg_entry CG_ENTRY

uint32_t CARDGAME_F0x80087edc(int32_t p1, int32_t p2, int32_t p3)
{
    int32_t n, p, r;
    uint32_t ret = 0xffffffff;

    if (*(uint8_t *)(p1 + 0x423) != 0) {
        int32_t i;
        switch (*(uint8_t *)(p1 + 0x423)) {
        case 1: case 2:
            switch (*(uint8_t *)(p1 + 0x420)) {
            case 0x99:
                if (*(uint8_t *)(p3 + 0x11) == 0) goto case_9a_9b;
                DAT_800a5d9c = 0x1c;
                break;
            case 0x9d:
                DAT_800a5d9c = 0x1c;
                break;
            case 0x9c:
                DAT_800a5d9c = 0x1b;
                break;
            case 0x9a: case 0x9b:
            case_9a_9b:
                DAT_800a5d9c = 0x19;
                break;
            }
            *(int32_t *)(p1 + 0x42c) = 0; *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x424) = 0; *(int32_t *)(p1 + 0x440) = 0;
            *(uint8_t *)(p1 + 0x444) = 0;
            break;
        case 5:
            DAT_80055c48(0x4001c);
            *(int32_t *)(p1 + 0x428) = 0;
            goto entry38;
        case 4:
            DAT_80055c48(0x4001c);
            break;
        case 14:
            if (*(uint8_t *)(p3 + 0x11) == 0) {
                *(uint8_t *)(p1 + 0x499) = 6;
                ((cg2_t)cg_slot(p2, 0xebc))(p2, 0);
            } else {
                *(uint8_t *)(p1 + 0x499) = 12;
            }
            *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x424) = 0;
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 4);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 0);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 1);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 2);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 3);
            break;
        case 10:
            *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x424) = 0;
            *(uint8_t *)(p1 + 0x499) = *(uint8_t *)(p1 + 0x49b);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 4);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 0);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 1);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 2);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 3);
            break;
        case 6: {
            int32_t i = 0;
            int32_t value, count;
            *(int32_t *)(p1 + 0x428) = 0;
            *(int32_t *)(p1 + 0x424) = 0;
            value = *(int32_t *)(p2 + 0xe1c);
            /* PAL reads the entry count before publishing the new value. */
            count = *(int16_t *)(p3 + 10);
            DAT_800a5d9c = value;
            if (count > 0) {
                do {
                    int32_t p = p2 + i * 0x4c;
                    if (*(int8_t *)(p1 + i + 0x46f) != 0) {
                        *(uint8_t *)(p + 0x151) = 0;
                        *(uint8_t *)(p + 0x150) |= 4;
                    } else {
                        *(uint8_t *)(p + 0x151) = 1;
                    }
                    i++;
                } while (i < *(int16_t *)(p3 + 10));
            }
            break;
        }
        case 7:
            *(int32_t *)(p1 + 0x440) = 0;
            *(int32_t *)(p1 + 0x424) = 0;
            break;
        case 3: case 8:
            *(int32_t *)(p1 + 0x428) = 0;
        entry38:
            *(int32_t *)(p1 + 0x424) = 0;
            break;
        case 11:
            *(uint8_t *)(p1 + 0x499) = *(uint8_t *)(p1 + 0x49b);
            DAT_800a5d9c = *(int32_t *)(p2 + 0xe1c);
            *(int32_t *)(p1 + 0x424) = *(uint8_t *)(p1 + 0x49b);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 4);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 0);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 1);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 2);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 3);
            if (*(uint8_t *)(p1 + 0x420) == 0x9a) ((cg2_t)cg_slot(p2, 0xeb0))(p2, 5);
            if (!*(uint8_t *)(p3 + 0x11)) ((cg2_t)cg_slot(p2, 0xebc))(p2, 0);
            break;
        case 13: {
            int32_t i, v;
            DAT_800a5d9c = 0x19;
            *(uint8_t *)(p1 + 0x499) = *(uint8_t *)(p1 + 0x424) - 1;
            v = *(uint8_t *)(p1 + 0x420);
            *(int32_t *)(p1 + 0x428) = 0;
            if (v >= 0x99) {
                if (v < 0x9b || v == 0x9d) {
                    for (i = 0; i < *(int16_t *)(p3 + 10); i++)
                        if (CARDGAME_F0x80085fd0(p1, p3 + 0xc, *(int16_t *)(p3 + 100 + i * 2)) != 0) *(uint8_t *)(p1 + i + 0x49e) = 0;
                        else *(uint8_t *)(p1 + i + 0x49e) = 1;
                }
            }
            ((cg6_t)cg_slot(p2, 0xeac))(p2, 0, 0, DAT_800a5d9c, 0, 0x42);
            ((cg6_t)cg_slot(p2, 0xeac))(p2, 2, 1, 0, 0x82, 0xa5);
            ((cg6_t)cg_slot(p2, 0xeac))(p2, 4, 2, 0, 0x86, 0x31);
            ((cg6_t)cg_slot(p2, 0xeac))(p2, 3, 1, 0, 0x82, 0x90);
            ((cg6_t)cg_slot(p2, 0xeac))(p2, 1, 3, 0, 0xfd, 0x90);
            if (*(uint8_t *)(p1 + 0x420) == 0x9a) ((cg6_t)CG_SLOT(p2, 0xeac))(p2, 5, 4, 0x24, 0, 0x14);
            if (*(uint8_t *)(p3 + 0x11) == 0) ((cg2_t)cg_slot(p2, 0xec0))(p2, 0);
            break;
        }
        case 9: {
            int32_t i;
            *(int32_t *)(p1 + 0x428) = 0; *(int32_t *)(p1 + 0x424) = 0;
            for (i = 0; i < *(int16_t *)(p3 + 10); i++) {
                int16_t v = *(int16_t *)(p3 + 100 + i * 2);
                int32_t p = p2 + i * 0x4c;
                if (CARDGAME_F0x80085fd0(p1, p3 + 0xc, v) != 0) *(uint8_t *)(p + 0x151) = 0;
                else *(uint8_t *)(p + 0x151) = 1;
                if (*(int8_t *)(p1 + i + 0x46f) != 0) { *(uint8_t *)(p + 0x151) = 0; *(uint8_t *)(p + 0x150) &= 0xfb; }
            }
            break;
        }
        }
        *(uint8_t *)(p1 + 0x422) = *(uint8_t *)(p1 + 0x423); *(uint8_t *)(p1 + 0x423) = 0;
    }

    switch (*(uint8_t *)(p1 + 0x422)) {
    case 1:
        *(int32_t *)(p1 + 0x42c) = CARDGAME_F0x800860d4(p1, p2, 2, DAT_800a5d9c, *(int32_t *)(p1 + 0x424), *(int32_t *)(p1 + 0x42c));
        CARDGAME_F0x80086a30(p1, p2, 0);
        if (!(*(int32_t *)(p1 + 0x4d4) * 4 + 14 < *(int32_t *)(p1 + 0x424))) goto tick;
        goto initial_select;
    case 2:
        ((cg5_t)cg_slot(p2, 0xf24))(p2, 0, 5, 0x1000, 0x1000);
        if (*(int32_t *)(p1 + 0x424) <= 10) goto tick;
    initial_select:
        *(uint8_t *)(p1 + 0x423) = 3;
        ((cg5_t)cg_slot(p2, 0xf0c))(p2, 0, 5, 0x1800, 0x5c00);
        ((cg_flags_view_t *)p2)->fields[*(int32_t *)(p1 + 0x43c)][0] |= 1;
        ((cg_selected_view_t *)p2)->fields[*(int32_t *)(p1 + 0x43c)][0] = 1;
        goto tick;
    case 3:
        switch (*(uint8_t *)(p1 + 0x420)) {
        case 0x99: CARDGAME_F0x800875ac(p1, p2, p3); goto redraw;
        case 0x9a: case 0x9d: CARDGAME_F0x800878b4(p1, p2, p3); goto redraw;
        case 0x9b: case 0x9c: CARDGAME_F0x80087b80(p1, p2, *(int32_t *)(p1 + 0x4d4)); goto redraw;
        }
        goto redraw;
    case 4:
        if (((cg_state_view_t *)p2)->fields[*(int32_t *)(p1 + 0x43c)][0] == 1) {
            CARDGAME_F0x8008722c(p1, p2, p3);
            n = FUN_80087308(p1, p2, p3);
            if (n != 0) { n = 6; goto shared423; }
            n = 3;
            goto shared423;
        }
        goto redraw;
    case 5:
        n = CARDGAME_F0x80087dc8(p1, p2);
        if (n == 0) goto redraw;
        n = 14;
    shared423:
        *(uint8_t *)(p1 + 0x423) = n;
        goto redraw;
    case 14:
        if (*(int16_t *)(p3 + 10) * 4 + 5 < *(int32_t *)(p1 + 0x424)) {
            ret = (*(int32_t *)(p1 + 0x440) != -1);
            *(uint8_t *)(p1 + 0x423) = 0;
            *(uint8_t *)(p1 + 0x421) = 0;
            *(uint8_t *)(p1 + 0x420) = 0;
            *(uint8_t *)(p1 + 0x421) = 0;
        }
        goto tick;
    case 10:
        if (*(int32_t *)(p1 + 0x4d4) * 4 + 14 < *(int32_t *)(p1 + 0x424)) {
            ret = 0;
            *(uint8_t *)(p1 + 0x423) = 0;
            *(uint8_t *)(p1 + 0x421) = 0;
            *(uint8_t *)(p1 + 0x420) = 0;
            *(uint8_t *)(p1 + 0x421) = 0;
        }
        goto tick;
    tick:
        r = DAT_8004df9c();
        *(int32_t *)(p1 + 0x424) += r;
        goto done;
    case 6:
        switch (*(int32_t *)(p1 + 0x424)) {
        case 0:
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 4);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 0);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 5);
            r = ((cg2_t)cg_slot(p2, 0xed8))(*(int16_t *)(p3 + 10), *(int32_t *)(p1 + 0x43c));
            ((cg5_t)cg_slot(p2, 0xf08))(p2, *(int32_t *)(p1 + 0x43c), 5, r + 0x1800, 0x6100);
            ((cg_flags_view_t *)p2)->fields[*(int32_t *)(p1 + 0x43c)][0] &= 0xfe;
            ((cg_selected_view_t *)p2)->fields[*(int32_t *)(p1 + 0x43c)][0] = 0;
            break;
        case 6:
            ((cg5_t)cg_slot(p2, 0xee4))(p2, 15, 1, 0, 2);
            break;
        }
        if (++*(int32_t *)(p1 + 0x424) > 18) {
            *(uint8_t *)(p1 + 0x423) = 7;
            return ret;
        }
        break;
    case 7: {
        int32_t b1, w1, b2, w2, b3, w3, b4, w4;
        if ((b1 = ((cg0_t)DAT_8004B7D0[0xfd])(0), w1 = ((cg2_t)DAT_8004B7D0[0x102])(0, 13), (b1 >> w1) & 1)) {
            ((cg1_t)cg_slot(p2, 0xeec))(p2);
            if (!*(int32_t *)(p1 + 0x440)) {
                *(uint8_t *)(p1 + 0x423) = 8;
                return ret;
            }
            goto set423_9;
        } else if (((b2 = ((cg0_t)DAT_8004B7D0[0xfd])(0), w2 = ((cg2_t)DAT_8004B7D0[0x102])(0, 4), (b2 >> w2) & 1))
                || ((b3 = ((cg0_t)DAT_8004B7D0[0xfd])(0), w3 = ((cg2_t)DAT_8004B7D0[0x102])(0, 6), (b3 >> w3) & 1))) {
            *(int32_t *)(p1 + 0x440) ^= 1;
            DAT_80055c48(0x8004513e);
            ((cg2_t)cg_slot(p2, 0xef0))(p2, *(int32_t *)(p1 + 0x440));
            return ret;
        } else if ((b4 = ((cg0_t)DAT_8004B7D0[0xfd])(0), w4 = ((cg2_t)DAT_8004B7D0[0x102])(0, 14), (b4 >> w4) & 1)) {
            DAT_80055c48(0x800450bd);
            *(int32_t *)(p1 + 0x440) = 1;
            ((cg2_t)cg_slot(p2, 0xef0))(p2, 1);
            ((cg1_t)cg_slot(p2, 0xeec))(p2);
        set423_9:
            *(uint8_t *)(p1 + 0x423) = 9;
        }
        return ret;
    }
    case 8: {
        int32_t i;
        switch (*(int32_t *)(p1 + 0x424)) {
        case 20:
            r = 0;
            for (i = 0; i < *(int16_t *)(p3 + 10); i++)
                if (*(int8_t *)(p1 + i + 0x46f)) {
                    ((cg5_t)cg_slot(p2, 0xf24))(p2, i, 6, 0x1400, 0x1400);
                    r++;
                }
            if (r) DAT_80055c48(0x4001c);
            break;
        case 25:
            for (i = 0; i < *(int16_t *)(p3 + 10); i++)
                if (*(int8_t *)(p1 + i + 0x46f))
                    ((cg5_t)cg_slot(p2, 0xf24))(p2, i, 6, 0x1000, 0x1000);
            break;
        case 35:
            if (*(uint8_t *)(p3 + 0x11) == 0) {
                *(uint8_t *)(p1 + 0x499) = 6;
                ((cg2_t)cg_slot(p2, 0xebc))(p2, 0);
            } else {
                *(uint8_t *)(p1 + 0x499) = 12;
            }
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 1);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 2);
            ((cg2_t)cg_slot(p2, 0xeb0))(p2, 3);
            break;
        }
        if (++*(int32_t *)(p1 + 0x424) > 45 && !*(uint8_t *)(p1 + 0x498)) {
            ret = 1;
            goto done;
        }
        break;
    }
    case 9:
        switch (*(int32_t *)(p1 + 0x424)) {
        case 6:
            break;
        case 18:
            ((cg6_t)cg_slot(p2, 0xeac))(p2, 0, 0, DAT_800a5d9c, 0, 0x42);
            ((cg6_t)cg_slot(p2, 0xeac))(p2, 4, 2, 0, 0x86, 0x31);
            ((cg6_t)cg_slot(p2, 0xeac))(p2, 5, 4, 0x24, 0, 0x14);
            CARDGAME_F0x80086a30(p1, p2, 0);
            break;
        }
        if (++*(int32_t *)(p1 + 0x424) <= 30) break;
    restore_select:
        *(uint8_t *)(p1 + 0x423) = 3;
        r = ((cg2_t)cg_slot(p2, 0xed8))(*(int16_t *)(p3 + 10), *(int32_t *)(p1 + 0x43c));
        ((cg5_t)cg_slot(p2, 0xf08))(p2, *(int32_t *)(p1 + 0x43c), 5, r + 0x1800, 0x5c00);
        ((cg_flags_view_t *)p2)->fields[*(int32_t *)(p1 + 0x43c)][0] |= 1;
        ((cg_selected_view_t *)p2)->fields[*(int32_t *)(p1 + 0x43c)][0] = 1;
        goto done;
    case 11:
        if (*(int16_t *)(p2 + 100) == 0 && !*(uint8_t *)(p1 + 0x498)) {
            *(uint8_t *)(p1 + 0x423) = 12;
            *(uint8_t *)(p1 + 0x568) = 0;
            *(uint8_t *)(p1 + 0x2f4) = 3;
            goto done;
        }
        break;
    case 12:
        *(uint8_t *)(p1 + 0x423) = 13;
        return ret;
    case 13: {
        int32_t k;
        if (*(uint8_t *)(p3 + 0x11) == 0) {
            if (*(int16_t *)(p2 + 100) == 2) goto state13_ready;
            CARDGAME_F0x80086a30(p1, p2, 0);
            goto done;
        } else if (++*(int32_t *)(p1 + 0x428) < 11) goto redraw;
    state13_ready:
        if (*(uint8_t *)(p1 + 0x498)) goto redraw;
        *(uint8_t *)(p1 + 0x423) = 3;
        r = ((cg2_t)cg_slot(p2, 0xed8))(*(int16_t *)(p3 + 10), *(int32_t *)(p1 + 0x43c));
        ((cg5_t)cg_slot(p2, 0xf0c))(p2, *(int32_t *)(p1 + 0x43c), 5, r + 0x1800, 0x5c00);
        ((cg_flags_view_t *)p2)->fields[*(int32_t *)(p1 + 0x43c)][0] |= 1;
        ((cg_selected_view_t *)p2)->fields[*(int32_t *)(p1 + 0x43c)][0] = 1;
        for (k = 0; k < *(int16_t *)(p3 + 10); k++) {
            int32_t p = p2 + k * 0x4c;
            if (*(int8_t *)(p1 + k + 0x46f) == 1) {
                *(uint8_t *)(p + 0x151) = 0;
                *(uint8_t *)(p + 0x150) |= 2;
            }
        }
        goto redraw;
    }
    }
    return ret;
redraw:
    CARDGAME_F0x80086a30(p1, p2, 0);
done:
    return ret;
}
