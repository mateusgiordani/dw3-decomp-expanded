/*
 * CARDGAME:0x800942dc CARDGAME_F0x800942dc
 * 1356 bytes at CARDGAME.PRO offset 0x1162c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800942dc, jump table (.rodata) at 0x8008365c
 *  Symbols     DAT_8004B7D0=0x8004b7d0 DAT_8005CCB0=0x8005ccb0
 *              DAT_800A5928=0x800a5928 FUN_80093bc4=0x80093bc4
 *  Compare     1356 bytes from 0x800942dc and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800942dc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Switch on byte ctx+0x422 states 1..10 via table at 0x8008365c (10 entries
 * 0x80094344..0x800947f8 verified in PAL); out-of-range returns 0, state 10
 * returns 1. Indirect dispatches via p2+0xf1c/0xf24/0xeac/0xeb0/0xea0; EXE tick
 * *0x8004df9c (= DAT_8004B7D0[0x9f3]); EXE polls via DAT_8004B7D0[0xfd]/[0x102]
 * (= *0x8004bbc4/*0x8004bbd8, shared lui/addiu base pattern per 878b4);
 * short-pair table DAT_800A5928 indexed by p3*4 + DAT_8005CCB0*8.
 *
 * No Ghidra state change.
 *
 * Live-range notes: case-1 loop counter reuses p3 (idx dead after row setup),
 * forcing a fresh saved reg for the row base; case-8 FUN results flow nested
 * into the ea0 calls (delay-slot a3); case-8 timer is reloaded from ctx+0x424
 * for the second FUN call (ea0 call kills memory between).
 *
 * The EXE tick uses an absolute address (f2ec precedent) so it does not CSE
 * with the DAT_8004B7D0 base shared by the case-5 polls (ref: direct lui/lw).
 *
 * Both loops use an explicit temp address plus a saved row copy (ref computes
 * the row into a temp, guards on its count, then moves it to the saved reg).
 *
 * - cases 1/8: sum = *(p1+0x424) += tick() keeps the sum in the loaded reg.
 *
 * - case 2: addresses built as integer values in locals (p2 + idx*0x4c), not
 * inside the address, give PAL's base-first addu; the head uses its own
 * block-local e0 so it does not share the loop pseudo.
 *
 * - case 3: table base in its own local before the state store, then addr =
 * base + (p3*4 + DAT_8005CCB0*8) as a separate pseudo.
 */

#include <stdint.h>

typedef int32_t (*cardgame_tick_t)(void);
typedef int32_t (*cardgame_sys0_t)(int32_t);
typedef int32_t (*cardgame_sys1_t)(int32_t, int32_t);
typedef void (*cardgame_d2_t)(int32_t, int32_t);
typedef void (*cardgame_d4_t)(int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_d5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_d6_t)(int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);

extern int32_t DAT_8004B7D0[];
extern int32_t DAT_8005CCB0;
extern int16_t DAT_800A5928[];
extern int FUN_80093bc4(int32_t,int32_t,int32_t,int32_t);

int CARDGAME_F0x800942dc(int32_t p1, int32_t p2, int32_t p3)
{
    int32_t ret = 0;

    switch (*(uint8_t *)(p1 + 0x422)) {
    case 1: {
        int32_t sum;
        int32_t n;
        int32_t tmp;
        uint8_t *t;
        uint8_t *row;

        sum = *(int32_t *)(p1 + 0x424) += ((cardgame_tick_t)*(int32_t *)0x8004df9c)();
        if (sum < 0x15)
            return ret;
        *(uint8_t *)(p1 + 0x422) = 2;
        n = (p3 != 0) ? 6 : 0;
        t = (uint8_t *)(p1 + p3 * 0x72);
        if (*(t + 0x72c) != 0) {
            p3 = 0;
            row = t;
            do {
                if (*(signed char *)(p1 + p3 + 0x446) != 0) {
                    tmp = n + p3;
                    ((cardgame_d2_t)*(int32_t *)(p2 + 0xf1c))(p2, tmp);
                    *(int32_t *)(p1 + 0x434) = tmp;
                }
                p3++;
            } while (p3 < *(row + 0x72c));
        }
        break;
    }
    case 2: {
        int32_t e;
        int32_t n;
        int32_t i;
        uint8_t *t;
        uint8_t *row;

        {
            int32_t e0 = p2 + *(int32_t *)(p1 + 0x434) * 0x4c;
            if (*(uint8_t *)(e0 + 0x14a) != 1)
                return ret;
        }
        ((cardgame_d5_t)*(int32_t *)(p2 + 0xf24))(p2, 0x11, 10, 0x1000, 0x1000);
        *(uint8_t *)(p1 + 0x422) = 3;
        n = (p3 != 0) ? 6 : 0;
        t = (uint8_t *)(p1 + p3 * 0x72);
        if (*(t + 0x72c) != 0) {
            i = 0;
            row = t;
            do {
                if (*(signed char *)(p1 + i + 0x446) != 0) {
                    e = p2 + (n + i) * 0x4c;
                    *(uint8_t *)(e + 0x150) &= 0xfb;
                }
                i++;
            } while (i < *(row + 0x72c));
        }
        break;
    }
    case 3: {
        char *addr;
        char *base;

        if (*(uint8_t *)(p2 + 0x656) != 1)
            return ret;
        base = (char *)DAT_800A5928;
        *(uint8_t *)(p1 + 0x422) = 4;
        addr = base + (p3 * 4 + DAT_8005CCB0 * 8);
        ((cardgame_d6_t)*(int32_t *)(p2 + 0xeac))(p2, 2, 1, *(int32_t *)(p1 + 0x438),
            *(int16_t *)addr, *(int16_t *)(addr + 2));
        break;
    }
    case 4:
        if (*(uint8_t *)(p2 + 0xe53) != 2)
            return ret;
        *(uint8_t *)(p1 + 0x422) = 5;
        *(int32_t *)(p1 + 0x424) = 0x5a;
        break;
    case 5: {
        int32_t sum;

        sum = ((cardgame_tick_t)*(int32_t *)0x8004df9c)();
        sum = *(int32_t *)(p1 + 0x424) - sum;
        *(int32_t *)(p1 + 0x424) = sum;
        if (0 < sum) {
            if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 13)) & 1) == 0) {
                if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 14)) & 1) == 0)
                    return ret;
            }
        }
        *(uint8_t *)(p1 + 0x422) = 6;
        ((cardgame_d2_t)*(int32_t *)(p2 + 0xeb0))(p2, 2);
        break;
    }
    case 6:
        if (*(uint8_t *)(p2 + 0xe53) != 0)
            return ret;
        ((cardgame_d2_t)*(int32_t *)(p2 + 0xf1c))(p2, 0x11);
        *(uint8_t *)(p1 + 0x422) = 7;
        break;
    case 7:
        if (*(uint8_t *)(p2 + 0x656) != 1)
            return ret;
        *(uint8_t *)(p1 + 0x422) = 8;
        *(int32_t *)(p1 + 0x424) = 0;
        break;
    case 8: {
        int32_t sum;

        sum = *(int32_t *)(p1 + 0x424) += ((cardgame_tick_t)*(int32_t *)0x8004df9c)();
        if (sum < 0x14) {
            uint8_t *row = (uint8_t *)(p1 + p3 * 200);
            int32_t value = FUN_80093bc4(*(int16_t *)(row + 0x59c) + *(int32_t *)(p1 + 0x42c), *(int16_t *)(row + 0x59c), 0x14, sum);
            ((cardgame_d4_t)*(int32_t *)(p2 + 0xea0))(p2, p3, 8, value);
            value = FUN_80093bc4(*(int16_t *)(row + 0x59e) + *(int32_t *)(p1 + 0x430), *(int16_t *)(row + 0x59e), 0x14, *(int32_t *)(p1 + 0x424));
            ((cardgame_d4_t)*(int32_t *)(p2 + 0xea0))(p2, p3, 9, value);
        } else {
            uint8_t *row = (uint8_t *)(p1 + p3 * 200);
            *(int16_t *)(row + 0x59c) = (int16_t)(*(uint16_t *)(row + 0x59c) + *(uint16_t *)(p1 + 0x42c));
            *(int16_t *)(row + 0x59e) = (int16_t)(*(uint16_t *)(row + 0x59e) + *(uint16_t *)(p1 + 0x430));
            ((cardgame_d4_t)*(int32_t *)(p2 + 0xea0))(p2, p3, 8, *(int16_t *)(row + 0x59c));
            ((cardgame_d4_t)*(int32_t *)(p2 + 0xea0))(p2, p3, 9, *(int16_t *)(row + 0x59e));
            ((cardgame_d5_t)*(int32_t *)(p2 + 0xf24))(p2, 0x11, 10, 0, 0x1000);
            *(uint8_t *)(p1 + 0x422) = 9;
        }
        break;
    }
    case 9:
        if (*(uint8_t *)(p2 + 0x656) != 1)
            return ret;
        *(uint8_t *)(p1 + 0x422) = 10;
        break;
    case 10:
        ret = 1;
        break;
    }
    return ret;
}