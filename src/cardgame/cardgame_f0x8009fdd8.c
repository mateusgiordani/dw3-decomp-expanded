/*
 * CARDGAME:0x8009fdd8 CARDGAME_F0x8009fdd8
 * 1904 bytes at CARDGAME.PRO offset 0x1d128 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009fdd8, jump table (.rodata) at 0x80083910
 *  Symbols     CARDGAME_F0x8009dec4=0x8009dec4 CARDGAME_F0x8009f998=0x8009f998
 *              CARDGAME_F0x8009fb18=0x8009fb18
 *              CARDGAME_RODATA_800A5D54=0x800a5d54
 *              CARDGAME_RODATA_800A5D56=0x800a5d56
 *              CARDGAME_RODATA_800A5D58=0x800a5d58
 *              CARDGAME_RODATA_800A5D5A=0x800a5d5a DAT_8004B7D0=0x8004b7d0
 *              FUN_8009d8fc=0x8009d8fc FUN_800a4978=0x800a4978
 *  Compare     1904 bytes from 0x8009fdd8 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009fdd8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Next framed CARDGAME:0x800a0548 at +0x770 (contiguous, no overlap).
 *
 * No Ghidra state change.
 *
 * Next-state nibbles live at 0x800a5d54..0x800a5d5b (bytes 02 03 04 05 06 07 08
 * 09), referenced with four base offsets.
 *
 * The two EXE bit-test vectors share one lui (0x8005) base in s2: 0x8004b7d0 +
 * 0x3f4 = 0x8004bbc4, + 0x408 = 0x8004bbd8.
 */

#include <stdint.h>

typedef void (*cardgame_dd8_ec8_t)(void *card);
typedef void (*cardgame_dd8_ea0_t)(void *card, int x, int y, int v);
typedef void (*cardgame_dd8_eac_t)(void *card, int a1, int a2, int a3, int a4, int a5);
typedef void (*cardgame_dd8_eb0_t)(void *card, int x);
typedef void (*cardgame_dd8_ed0_t)(void *card, int x);
typedef void (*cardgame_dd8_ed4_t)(void *card, int x);
typedef void (*cardgame_dd8_814_t)(void *ctx, void *buf, int v, int w);
typedef void (*cardgame_dd8_exe19_t)(uint32_t arg);
typedef int (*cardgame_dd8_exe4_t)(int arg);
typedef int (*cardgame_dd8_exe4_2_t)(int a0, int a1);

extern void FUN_8009d8fc(void *ctx);
extern int FUN_800a4978(void *ctx, void *card);
extern int CARDGAME_F0x8009f998(void *ctx, void *par);
extern void CARDGAME_F0x8009fb18(void *ctx, void *card);
extern int CARDGAME_F0x8009dec4(void *ctx, void *par);

extern unsigned char CARDGAME_RODATA_800A5D54[];
extern unsigned char CARDGAME_TBL_800A5D54[];
extern unsigned char CARDGAME_RODATA_800A5D56[];
extern unsigned char CARDGAME_RODATA_800A5D58[];
extern unsigned char CARDGAME_RODATA_800A5D5A[];
extern uint32_t DAT_8004B7D0[];

int CARDGAME_F0x8009fdd8(void *ctx, void *par)
{
    int ret = 0;

    switch (((unsigned char *)ctx)[0x574]) {
    case 0:
    default: {
        unsigned int seed = ((unsigned char *)ctx)[0x2f5];
        ((unsigned char *)ctx)[0x574] = 0x10;
        ((unsigned char *)ctx)[0x575] = 0;
        ((unsigned char *)ctx)[0x577] = 0;
        *(uint16_t *)((unsigned char *)ctx + 0x582) = 0;
        *(uint16_t *)((unsigned char *)ctx + 0x58a) = 0;
        *(uint16_t *)((unsigned char *)ctx + 0x592) = 0;
        ((unsigned char *)ctx)[0x49d] = 1;
        ((unsigned char *)ctx)[0x499] = 1;
        ((unsigned char *)ctx)[0x579] = (unsigned char)seed;
        ((unsigned char *)ctx)[0x578] = (unsigned char)seed;
        {
            void *card = *(void **)((unsigned char *)par + 0x18);
            ((cardgame_dd8_ec8_t)*(uint32_t *)((unsigned char *)card + 0xec8))(card);
        }
        break;
    }
    case 16: {
        void *guard_card = *(void **)((unsigned char *)par + 0x18);
        if (*(int16_t *)((unsigned char *)guard_card + 0x64) == 2 &&
            ((unsigned char *)ctx)[0x498] == 0) {
            ((unsigned char *)ctx)[0x574] = 0x11;
            {
                void *method_card = *(void **)((unsigned char *)par + 0x18);
                int a3 = (((unsigned char *)ctx)[0x2f8] == 5) ? 0x3c : 0x3d;
                ((cardgame_dd8_eac_t)*(uint32_t *)((unsigned char *)method_card + 0xeac))(
                    *(void **)((unsigned char *)par + 0x18), 5, 5, a3, 0, 0x6e);
            }
        }
        break;
    }
    case 17: {
        int bit0;
        int bit1;
        if (*(unsigned char *)((unsigned char *)*(void **)((unsigned char *)par + 0x18) + 0xe9b) != 2)
            break;
        {
            uint32_t *exevec = DAT_8004B7D0;
            bit0 = ((cardgame_dd8_exe4_t)exevec[0x3f4 / 4])(0) >>
                ((cardgame_dd8_exe4_2_t)exevec[0x408 / 4])(0, 13);
            bit0 &= 1;
            if (bit0 == 0) {
                bit1 = ((cardgame_dd8_exe4_t)exevec[0x3f4 / 4])(0) >>
                    ((cardgame_dd8_exe4_2_t)exevec[0x408 / 4])(0, 14);
                bit1 &= 1;
                if (bit1 == 0)
                    break;
            }
        }
        ((unsigned char *)ctx)[0x574] = 0x12;
        {
            void *card = *(void **)((unsigned char *)par + 0x18);
            ((cardgame_dd8_eb0_t)*(uint32_t *)((unsigned char *)card + 0xeb0))(card, 5);
        }
        break;
    }
    case 18: {
        void *card = *(void **)((unsigned char *)par + 0x18);
        if (*(unsigned char *)((unsigned char *)card + 0xe9b) == 0)
            ((unsigned char *)ctx)[0x574] = 1;
        break;
    }
    case 1: {
        int v = ((signed char *)ctx)[0x575];
        if (v != 1) {
            if (v < 2) {
                if (v != 0) {
                    ((unsigned char *)ctx)[0x574] = 12;
                    goto case1_tail;
                }
            } else if (v != 2) {
                ((unsigned char *)ctx)[0x574] = 12;
                goto case1_tail;
            }
            if (((unsigned char *)ctx)[0x578] != 0)
                ((unsigned char *)ctx)[0x574] = 3;
            else
                ((unsigned char *)ctx)[0x574] = 2;
        } else {
            if (((unsigned char *)ctx)[0x578] != 0)
                ((unsigned char *)ctx)[0x574] = 2;
            else
                ((unsigned char *)ctx)[0x574] = 3;
        }
    case1_tail:
        if (((signed char *)ctx)[0x575] != 0)
            ((unsigned char *)ctx)[0x577] = 0;
        ((signed char *)ctx)[0x57a] = -1;
        {
            void *card = *(void **)((unsigned char *)par + 0x18);
            ((cardgame_dd8_ea0_t)*(uint32_t *)((unsigned char *)card + 0xea0))(
                card, 0, 6, *(int16_t *)((unsigned char *)ctx + 0x5a6));
            card = *(void **)((unsigned char *)par + 0x18);
            ((cardgame_dd8_ea0_t)*(uint32_t *)((unsigned char *)card + 0xea0))(
                card, 1, 6, *(int16_t *)((unsigned char *)ctx + 0x66e));
        }
        break;
    }
    case 3: {
        int ok;
        FUN_8009d8fc(ctx);
        {
            void *card = *(void **)((unsigned char *)par + 0x18);
            ok = FUN_800a4978(ctx, card);
        }
        if (ok != 0) {
            int pick = CARDGAME_F0x8009f998(ctx, par);
            if ((unsigned int)(pick - 0x83) < 2) {
                int i = 14;
                unsigned char *q = (unsigned char *)ctx + 14;
                do {
                    q[0x46f] = 0;
                    q--;
                    i--;
                } while (i >= 0);
                ((unsigned char *)ctx)[0x47a + ((signed char *)ctx)[0x575]] = 1;
                *(uint16_t *)(0x582 + ((unsigned char *)ctx +
                    ((((signed char *)ctx)[0x575] - 1) << 3))) = 1;
            }
            if (((unsigned char *)ctx)[0x577] != 0) {
                ((cardgame_dd8_exe19_t)*(uint32_t *)0x80055c48)(0x40019);
                {
                    void *card = *(void **)((unsigned char *)par + 0x18);
                    ((cardgame_dd8_ed4_t)*(uint32_t *)((unsigned char *)card + 0xed4))(card, 0);
                }
            }
            ((unsigned char *)ctx)[0x574] = 9;
            {
                void *card = *(void **)((unsigned char *)par + 0x18);
                CARDGAME_F0x8009fb18(ctx, card);
            }
        } else {
            if (((signed char *)ctx)[0x575] == 0) {
                ((unsigned char *)ctx)[0x574] = 0x0b;
                *(int *)((unsigned char *)ctx + 0x57c) = 0x14;
                ((unsigned char *)ctx)[0x577]++;
                ((cardgame_dd8_exe19_t)*(uint32_t *)0x80055c48)(0x40019);
                {
                    void *card = *(void **)((unsigned char *)par + 0x18);
                    ((cardgame_dd8_ed0_t)*(uint32_t *)((unsigned char *)card + 0xed0))(card, 1);
                }
            } else {
                ((unsigned char *)ctx)[0x574] = 0x0c;
            }
        }
        break;
    }
    case 2:
        if (((signed char *)ctx)[0x57a] == -1) {
            ((unsigned char *)ctx)[0x2f4] = 1;
            ((unsigned char *)ctx)[0x421] = 0x98;
            ((cardgame_dd8_814_t)*(uint32_t *)((unsigned char *)ctx + 0x814))(
                ctx, (unsigned char *)ctx + 0x600,
                ((int)*(int16_t *)((unsigned char *)ctx + 0x5a6)) << 16, 0);
        } else if (((signed char *)ctx)[0x57a] != 0) {
            unsigned int next = CARDGAME_RODATA_800A5D54[((unsigned char *)ctx)[0x579] + 2];
            ((signed char *)ctx)[0x57a] = -1;
            ((unsigned char *)ctx)[0x574] = (unsigned char)next;
            if (((unsigned char *)ctx)[0x577] != 0) {
                void *card = *(void **)((unsigned char *)par + 0x18);
                *(unsigned char *)((unsigned char *)card + 0x104) = 0;
            }
        } else {
            if (((signed char *)ctx)[0x575] == 0) {
                ((unsigned char *)ctx)[0x574] = 0x0b;
                *(int *)((unsigned char *)ctx + 0x57c) = 0x14;
                ((unsigned char *)ctx)[0x577]++;
                ((cardgame_dd8_exe19_t)*(uint32_t *)0x80055c48)(0x40019);
                {
                    void *card = *(void **)((unsigned char *)par + 0x18);
                    ((cardgame_dd8_ed0_t)*(uint32_t *)((unsigned char *)card + 0xed0))(card, 0);
                }
            } else {
                ((unsigned char *)ctx)[0x574] = 0x0c;
            }
        }
        break;
    case 4:
    case 5:
        if (((signed char *)ctx)[0x57a] == -1) {
            ((unsigned char *)ctx)[0x2f4] = 1;
            ((unsigned char *)ctx)[0x421] = 0x99;
        } else if (((signed char *)ctx)[0x57a] != 0) {
            unsigned int next = CARDGAME_RODATA_800A5D54[((unsigned char *)ctx)[0x579] + 4];
            ((signed char *)ctx)[0x57a] = -1;
            ((unsigned char *)ctx)[0x574] = (unsigned char)next;
        } else {
            unsigned int next = CARDGAME_RODATA_800A5D54[((unsigned char *)ctx)[0x579]];
            ((signed char *)ctx)[0x57a] = -1;
            ((unsigned char *)ctx)[0x574] = (unsigned char)next;
            if (((unsigned char *)ctx)[0x577] != 0) {
                void *card = *(void **)((unsigned char *)par + 0x18);
                *(unsigned char *)((unsigned char *)card + 0x104) = 2;
            }
        }
        break;
    case 6:
    case 7: {
        int unset = -1;
        if (((signed char *)ctx)[0x57a] == unset) {
            ((unsigned char *)ctx)[0x421] =
                (unsigned char)CARDGAME_F0x8009f998(ctx, par);
            ((unsigned char *)ctx)[0x2f4] = 1;
        } else if (((signed char *)ctx)[0x57a] != 0) {
            ((unsigned char *)ctx)[0x574] =
                CARDGAME_RODATA_800A5D54[((unsigned char *)ctx)[0x579] + 6];
        } else {
            unsigned int next;
            unsigned int idx = ((unsigned char *)ctx)[0x579];
            unsigned char *row = (unsigned char *)ctx + idx * 200;
            int slot = *(int16_t *)(row + 0x5a6);
            int off = (slot << 1) + (int)(idx * 200);
            unsigned char *ptr = (unsigned char *)ctx + off;
            *(uint16_t *)(ptr + 0x600) =
                *(uint16_t *)((unsigned char *)ctx +
                    (((signed char *)ctx)[0x575] << 3) + 0x580);
            (*(uint16_t *)(row + 0x5a6))++;
            ((cardgame_dd8_814_t)*(uint32_t *)((unsigned char *)ctx + 0x814))(
                ctx, (unsigned char *)ctx + 0x600,
                ((int)*(int16_t *)((unsigned char *)ctx + 0x5a6)) << 16, 0);
            next = CARDGAME_RODATA_800A5D54[((unsigned char *)ctx)[0x579] + 2];
            ((signed char *)ctx)[0x57a] = unset;
            ((unsigned char *)ctx)[0x574] = (unsigned char)next;
        }
        break;
    }
    case 9:
        ((unsigned char *)ctx)[0x421] = 0x13;
        ((unsigned char *)ctx)[0x2f4] = 1;
        ((unsigned char *)ctx)[0x574] = 0x0a;
        break;
    case 8:
        ((unsigned char *)ctx)[0x421] = 0x12;
        ((unsigned char *)ctx)[0x2f4] = 1;
        ((unsigned char *)ctx)[0x574] = 0x0a;
        break;
    case 10:
        ((unsigned char *)ctx)[0x574] = 1;
        ((unsigned char *)ctx)[0x579] ^= 1;
        ((unsigned char *)ctx)[0x575]++;
        break;
    case 11: {
        int left = *(int *)((unsigned char *)ctx + 0x57c) - 1;
        *(int *)((unsigned char *)ctx + 0x57c) = left;
        if (left <= 0)
            ((unsigned char *)ctx)[0x574] = 0x0e;
        break;
    }
    case 12:
        if (((signed char *)ctx)[0x575] > 0) {
            ((unsigned char *)ctx)[0x2f4] = 2;
            ((unsigned char *)ctx)[0x574] = 0x0d;
            ((unsigned char *)ctx)[0x4dc] = 0;
            *(int *)((unsigned char *)ctx + 0x4ec) = 0;
            *(int *)((unsigned char *)ctx + 0x4e8) = 0;
            ((unsigned char *)ctx)[0x4de] = ((unsigned char *)ctx)[0x575];
        } else {
            ((unsigned char *)ctx)[0x574] = 0x0e;
        }
        break;
    case 13:
        ((unsigned char *)ctx)[0x574] = 0x0c;
        break;
    case 14:
        ((unsigned char *)ctx)[0x574] = 1;
        ((unsigned char *)ctx)[0x575] = 0;
        *(uint16_t *)((unsigned char *)ctx + 0x582) = 0;
        *(uint16_t *)((unsigned char *)ctx + 0x58a) = 0;
        *(uint16_t *)((unsigned char *)ctx + 0x592) = 0;
        {
            unsigned char sel = (unsigned char)(((unsigned char *)ctx)[0x578] ^ 1);
            ((unsigned char *)ctx)[0x578] = sel;
            ((unsigned char *)ctx)[0x579] = sel;
        }
        ((unsigned char *)ctx)[0x576]++;
        if (((unsigned char *)ctx)[0x577] > 1) {
            ((unsigned char *)ctx)[0x574] = 0x0f;
            *(int *)((unsigned char *)ctx + 0x4e8) = 0;
        }
        break;
    case 15: {
        void *card = *(void **)((unsigned char *)par + 0x18);
        ((cardgame_dd8_ed4_t)*(uint32_t *)((unsigned char *)card + 0xed4))(card, 0);
        card = *(void **)((unsigned char *)par + 0x18);
        ((cardgame_dd8_ed4_t)*(uint32_t *)((unsigned char *)card + 0xed4))(card, 1);
        if (CARDGAME_F0x8009dec4(ctx, par) != 0)
            ret = 1;
        break;
    }
    }
    return ret;
}
