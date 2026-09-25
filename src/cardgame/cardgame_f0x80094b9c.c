/*
 * CARDGAME:0x80094b9c CARDGAME_F0x80094b9c
 * 1088 bytes at CARDGAME.PRO offset 0x11eec (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80094b9c, jump table (.rodata) at 0x80083684
 *  Symbols     CARDGAME_F0x80084320=0x80084320 CARDGAME_F0x8008da2c=0x8008da2c
 *              DAT_8004B7D0=0x8004b7d0 DAT_8004DE10=0x8004de10
 *  Compare     1088 bytes from 0x80094b9c and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80094b9c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Ghidra function entry CARDGAME_F0x80094b9c size 1088 confirms the boundary.
 *
 * decompile CARDGAME_F0x80094b9c; x-ref to from CARDGAME_F0x80084320 at
 * 0x8008527c and 0x8008529c (UNCONDITIONAL_CALL, args ctx/card/idx 0 then 1);
 * x-ref from: 1 direct jal CARDGAME_F0x8008da2c + indirect jalr
 * (card+0xf24/0xea0(x4)/0xeac/0xeb0/0xec4) + EXE vectors *0x8004df9c,
 * *0x8004bbc4/*0x8004bbd8, *0x80055c48; rest intra-function/stack.
 *
 * No Ghidra state change.
 *
 * Caller CARDGAME_F0x80084320 passes (ctx, card, idx 0/1) and tests the return.
 *
 * Semantics: state byte ctx+0x422 dispatches a 7-state card sequencer; state 1
 * paces a cell at ctx+idx*0x72+0x72c against counter ctx+0x428 with timer
 * ctx+0x438, EXE-paced into counters ctx+0x424/0x438, reset at 0x3d into state
 * 2 with short pairs ctx+0x5a4/0x5a6 and ctx+0x66c/0x66e; state 2 drains
 * counter pairs ctx+0x428/0x42c and ctx+0x430/0x434 with EXE vector 0x800452c6
 * or state 4 + card slot 0xeac, then 4 card slot 0xea0 writes; states 3/5/6 are
 * flag-gated transitions (card+0xe9b, EXE bit tests, card+0x64), state 7
 * reports done (returns 1, else 0).
 *
 * Matching notes (all portable C, no register variables): switch subject is the
 * state MEM load with cases 1..7 (range-check + table at 0x80083684);
 * case-1/case-2 EXE-paced accums reload MEM per statement (store aliasing
 * blocks CSE); case-2 pair drain stages a flag var set to 1 per arm and reused
 * for the sibling increment (addu) and the EXE/state-4 split; the EXE vector
 * arg stages exeseg = 0x80040000 with ori 0x52c6; case-4 bit tests are plain (f
 * >> t) & 1 (srav+andi, no explicit &31 in source); cell addresses use a scoped
 * stride (int k = idx * 0x72) so the final addu keeps ctx in rs;
 *
 * EXE vectors use extern word-array bases (DAT_8004DE10[0x63] -> 0x8004df9c,
 * DAT_8004B7D0[0xfd/0x102] -> 0x8004bbc4/0x8004bbd8) because only %hi/%lo
 * extern access reproduces the lui+addiu+lw split (raw absolute constants fold
 * to li+ori+lw under this cc1, proven by micro-experiment).
 *
 * Ranked alternate 1 (documented, rejected): cc1
 * psyq-gcc-2.7.2-sn32-3.7-build-0002 full-function object is 1092 B (4 over)
 * with wider scheduling divergence.
 *
 * (aspsx-2.77/2.67 produce byte-identical objects here; diagnostic only.)
 *
 * The base hoist orders arg setup before the pointer load and frees the second
 * jalr delay slot for the late save (move s0,v0); distinct lifetimes put the
 * pointer in v1 and the srav into s0, matching PAL. Portable C only.
 */

#include <stdint.h>

typedef void (*cardgame_94b9c_f24_t)(void *card, int val, unsigned int kind, int zero, int flag);
typedef void (*cardgame_94b9c_ea0_t)(void *card, unsigned int x, unsigned int y, int val);
typedef void (*cardgame_94b9c_eac_t)(void *card, unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned int e);
typedef void (*cardgame_94b9c_eb0_t)(void *card, unsigned int kind);
typedef void (*cardgame_94b9c_ec4_t)(void *card);
typedef int (*cardgame_94b9c_exe0_t)(void);
typedef int (*cardgame_94b9c_exe1_t)(int arg);
typedef int (*cardgame_94b9c_exe2_t)(int a0, int a1);
typedef void (*cardgame_94b9c_exe_vec_t)(uint32_t arg);

extern void CARDGAME_F0x8008da2c(void *ctx, void *card, int idx, int val);

extern uint32_t DAT_8004DE10[];
extern uint32_t DAT_8004B7D0[];

int CARDGAME_F0x80094b9c(void *ctx, void *card, int idx)
{
    int ret = 0;
    int v;
    int r;
    unsigned int exeseg;

    switch (((unsigned char *)ctx)[0x422]) {
    case 1:
        if (*(int *)((unsigned char *)ctx + 0x438) > 2) {
            int k = idx * 0x72;
            if (*(int *)((unsigned char *)ctx + 0x428) < ((unsigned char *)ctx + k)[0x72c]) {
                v = *(int *)((unsigned char *)ctx + 0x428);
                if (idx != 0)
                    v += 6;
                ((cardgame_94b9c_f24_t)*(uint32_t *)((unsigned char *)card + 0xf24))(card, v, 4, 0, 0x1000);
                CARDGAME_F0x8008da2c(ctx, card, idx, *(int *)((unsigned char *)ctx + 0x428));
                *(int *)((unsigned char *)ctx + 0x428) = *(int *)((unsigned char *)ctx + 0x428) + 1;
            }
            *(int *)((unsigned char *)ctx + 0x438) = *(int *)((unsigned char *)ctx + 0x438) - 3;
        }
        r = ((cardgame_94b9c_exe0_t)DAT_8004DE10[0x63])();
        *(int *)((unsigned char *)ctx + 0x424) = *(int *)((unsigned char *)ctx + 0x424) + r;
        r = ((cardgame_94b9c_exe0_t)DAT_8004DE10[0x63])();
        *(int *)((unsigned char *)ctx + 0x438) = *(int *)((unsigned char *)ctx + 0x438) + r;
        if (*(int *)((unsigned char *)ctx + 0x424) > 0x3c) {
            int k = idx * 0x72;
            ((unsigned char *)ctx + k)[0x72c] = 0;
            *(int *)((unsigned char *)ctx + 0x424) = 0;
            ((unsigned char *)ctx)[0x422] = 2;
            *(int *)((unsigned char *)ctx + 0x438) = 0;
            *(int *)((unsigned char *)ctx + 0x428) = (int)*(short *)((unsigned char *)ctx + 0x5a6);
            *(int *)((unsigned char *)ctx + 0x42c) = (int)*(short *)((unsigned char *)ctx + 0x5a4);
            *(int *)((unsigned char *)ctx + 0x430) = (int)*(short *)((unsigned char *)ctx + 0x66e);
            *(int *)((unsigned char *)ctx + 0x434) = (int)*(short *)((unsigned char *)ctx + 0x66c);
        }
        break;
    case 2:
        {
            int b;
            int f;
            int d;
            r = ((cardgame_94b9c_exe0_t)DAT_8004DE10[0x63])();
            *(int *)((unsigned char *)ctx + 0x424) = *(int *)((unsigned char *)ctx + 0x424) + r;
            r = ((cardgame_94b9c_exe0_t)DAT_8004DE10[0x63])();
            *(int *)((unsigned char *)ctx + 0x438) = *(int *)((unsigned char *)ctx + 0x438) + r;
            if (*(int *)((unsigned char *)ctx + 0x438) > 2) {
                f = 0;
                b = *(int *)((unsigned char *)ctx + 0x428);
                if (b > 0) {
                    f = 1;
                    *(int *)((unsigned char *)ctx + 0x428) = b - 1;
                    *(int *)((unsigned char *)ctx + 0x42c) = *(int *)((unsigned char *)ctx + 0x42c) + f;
                }
                d = *(int *)((unsigned char *)ctx + 0x430);
                if (d > 0) {
                    f = 1;
                    *(int *)((unsigned char *)ctx + 0x430) = d - 1;
                    *(int *)((unsigned char *)ctx + 0x434) = *(int *)((unsigned char *)ctx + 0x434) + f;
                }
                if (f == 0) {
                    ((unsigned char *)ctx)[0x422] = 4;
                    ((cardgame_94b9c_eac_t)*(uint32_t *)((unsigned char *)card + 0xeac))(card, 5, 5, 0x14, 0, 0x6e);
                } else {
                    exeseg = 0x80040000;
                    ((cardgame_94b9c_exe_vec_t)*(uint32_t *)0x80055c48)(exeseg | 0x52c6);
                }
                ((cardgame_94b9c_ea0_t)*(uint32_t *)((unsigned char *)card + 0xea0))(card, 0, 6, *(int *)((unsigned char *)ctx + 0x428));
                ((cardgame_94b9c_ea0_t)*(uint32_t *)((unsigned char *)card + 0xea0))(card, 0, 5, *(int *)((unsigned char *)ctx + 0x42c));
                ((cardgame_94b9c_ea0_t)*(uint32_t *)((unsigned char *)card + 0xea0))(card, 1, 6, *(int *)((unsigned char *)ctx + 0x430));
                ((cardgame_94b9c_ea0_t)*(uint32_t *)((unsigned char *)card + 0xea0))(card, 1, 5, *(int *)((unsigned char *)ctx + 0x434));
                *(int *)((unsigned char *)ctx + 0x438) = *(int *)((unsigned char *)ctx + 0x438) - 3;
            }
        }
        break;
    case 3:
        if (((unsigned char *)card)[0xe9b] == 2) {
            ((unsigned char *)ctx)[0x422] = 4;
        }
        break;
    case 4:
        {
            uint32_t *bb = DAT_8004B7D0;
            int f1 = ((cardgame_94b9c_exe1_t)bb[0xfd])(0);
            int t1 = ((cardgame_94b9c_exe2_t)bb[0x102])(0, 0xd);
            if (((f1 >> t1) & 1) == 0) {
                int f2 = ((cardgame_94b9c_exe1_t)bb[0xfd])(0);
                int t2 = ((cardgame_94b9c_exe2_t)bb[0x102])(0, 0xe);
                if (((f2 >> t2) & 1) == 0)
                    break;
            }
            ((unsigned char *)ctx)[0x422] = 5;
            ((cardgame_94b9c_eb0_t)*(uint32_t *)((unsigned char *)card + 0xeb0))(card, 5);
        }
        break;
    case 5:
        if (((unsigned char *)card)[0xe9b] == 0) {
            ((unsigned char *)ctx)[0x422] = 6;
            ((cardgame_94b9c_ec4_t)*(uint32_t *)((unsigned char *)card + 0xec4))(card);
        }
        break;
    case 6:
        if (*(short *)((unsigned char *)card + 0x64) == 0) {
            ((unsigned char *)ctx)[0x422] = 7;
        }
        break;
    case 7:
        ret = 1;
        break;
    }
    return ret;
}