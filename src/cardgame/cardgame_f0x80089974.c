/*
 * CARDGAME:0x80089974 CARDGAME_F0x80089974
 * 1364 bytes at CARDGAME.PRO offset 0x6cc4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80089974, jump table (.rodata) at 0x80083364
 *  Symbols     CARDGAME_F0x80087dc8=0x80087dc8 CARDGAME_F0x800896f0=0x800896f0
 *              CARDGAME_F0x80089974=0x80089974 DAT_8004B7D0=0x8004b7d0
 *              D_8004df9c=0x8004df9c D_80055c48=0x80055c48
 *  Compare     1364 bytes from 0x80089974 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80089974
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Jump table 6 words at 0x80083364: 800899e0 80089a8c 80089c54 80089c94
 * 80089dfc 80089e70 = cases 1-6 of byte st+0x422 (lbu; -1; sltiu <6; default
 * returns 0). Case targets confirmed via PAL words.
 *
 * X-ref from: 6 computed jumps (jt), direct jal CARDGAME_F0x800896f0 (case 2,
 * delay move a1,s5) + jal CARDGAME_F0x80087dc8 (case 3, tested); indirect jalr
 * via ctx slots 0xf24/0xf0c/0xf28/0xeac/0xeb0 and EXE vectors
 * 0x8004bbcc/0x8004bbd8/0x8004bbc4 via shared base 0x8004b7d0 (lui 0x8005 +
 * addiu -0x4830), 0x80055c48 (lui 0x8005 + lw), 0x8004df9c (lui 0x8005 + lw
 * -0x2064).
 *
 * Codegen notes (portable C, no register variables): int ret = 0 in s8,
 * returned via shared move v0,s8; case-2 pair tests use || (bne-to-call then
 * beq-to-rest asymmetry proves short-circuit, not bitwise-or); sllv pairs use
 * plain 1 << x (no hardware-redundant &31); srav tests use signed >> with &1;
 * a3 defaults 0x43, set 0x44 when 0x434==0x440; vec args 0x4001c/0x40019 via
 * lui a0,0x4 + ori (delay-filled).
 *
 * Revision 5 (2026-09-11): reverted the PAD_MASK/PAD_PRESS macro refactor
 * (committed 2026-09-08, integration batch 036, commit 51c0c7ff) that
 * introduced named b0/b1/c0/c1 temporaries for the case-2/case-4 pad-state
 * tests. That refactor forced the values returned by the first two syscalls in
 * each test to be spilled across the following syscall calls, which pushed the
 * object from 1364 B to 1396 B (CODEGEN_SIZE, reproduced against
 * psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 in this revision). Restored
 * the original fully-inlined nested expressions from worker commit
 * de03b8d51bca2378cd8cb851e037cd47f9830a9b.
 */

#include <stdint.h>

typedef void (*cardgame_9974_f24_t)(void *ctx, int a1, int a2, int a3, int a4);
typedef void (*cardgame_9974_f0c_t)(void *ctx, int a1, int a2, int a3, int a4);
typedef void (*cardgame_9974_f28_t)(void *ctx, int a1);
typedef void (*cardgame_9974_eac_t)(void *ctx, int a1, int a2, int a3, int a4, int a5);
typedef void (*cardgame_9974_eb0_t)(void *ctx, int a1);
typedef int (*cardgame_9974_sys0_t)(int a0);
typedef int (*cardgame_9974_sys1_t)(int a0, int a1);
typedef int (*cardgame_9974_df9c_t)(void);
extern void (*D_80055c48)(uint32_t);
extern cardgame_9974_df9c_t D_8004df9c;

extern int32_t DAT_8004B7D0[];
extern void CARDGAME_F0x800896f0(void *st, void *ctx);
extern int CARDGAME_F0x80087dc8(void *st, void *ctx);

int CARDGAME_F0x80089974(void *st, void *ctx)
{
    int ret = 0;
    int cnt;
    int idx;
    int off;
    int sel;
    int a1_1;

    switch (((unsigned char *)st)[0x422]) {
    case 1: {
        int v1 = *(int32_t *)((unsigned char *)st + 0x424);
        void *a0_1;
        if (v1 == 0)
            goto c1_zero;
        if (v1 == 4)
            goto c1_four;
        goto c1_tail;
c1_zero:
        a0_1 = ctx;
        a1_1 = 0;
        goto c1_call;
c1_four:
        a0_1 = ctx;
        a1_1 = 1;
c1_call:
        ((cardgame_9974_f24_t)*(uint32_t *)((unsigned char *)ctx + 0xf24))(a0_1, a1_1, 10, 0x1000, 0x1000);
c1_tail:
        cnt = *(int32_t *)((unsigned char *)st + 0x424) + 1;
        *(int32_t *)((unsigned char *)st + 0x424) = cnt;
        if (cnt < 15)
            break;
        ((unsigned char *)st)[0x422] = 2;
        ((cardgame_9974_f0c_t)*(uint32_t *)((unsigned char *)ctx + 0xf0c))(ctx, 0, 5, 0x7400, 0x5c00);
        *(int16_t *)((unsigned char *)ctx + 0x12e) = 1;
        ((unsigned char *)ctx)[0x150] |= 1;
        break;
    }
    case 2:
        idx = *(int32_t *)((unsigned char *)st + 0x43c);
        off = idx * 76;
        ((unsigned char *)ctx + off)[0x150] |= 1;
        idx = *(int32_t *)((unsigned char *)st + 0x43c);
        off = idx * 76;
        *(int16_t *)((unsigned char *)ctx + off + 0x12e) = 1;
        if (((((cardgame_9974_sys0_t)DAT_8004B7D0[0xff])(0) & (1 << ((cardgame_9974_sys1_t)DAT_8004B7D0[0x102])(0, 7))) |
            (((cardgame_9974_sys0_t)DAT_8004B7D0[0xfd])(0) & (1 << ((cardgame_9974_sys1_t)DAT_8004B7D0[0x102])(0, 7)))) != 0 ||
            ((((cardgame_9974_sys0_t)DAT_8004B7D0[0xff])(0) & (1 << ((cardgame_9974_sys1_t)DAT_8004B7D0[0x102])(0, 5))) |
            (((cardgame_9974_sys0_t)DAT_8004B7D0[0xfd])(0) & (1 << ((cardgame_9974_sys1_t)DAT_8004B7D0[0x102])(0, 5)))) != 0) {
            CARDGAME_F0x800896f0(st, ctx);
            break;
        }
        if (((((cardgame_9974_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_9974_sys1_t)DAT_8004B7D0[0x102])(0, 13)) & 1) == 0)
            break;
        {
            int idx2 = *(int32_t *)((unsigned char *)st + 0x43c);
            ((unsigned char *)st)[0x422] = 3;
            *(int32_t *)((unsigned char *)st + 0x440) = idx2;
            ((unsigned char *)st + idx2)[0x46f] = 1;
        }
        *(int32_t *)((unsigned char *)st + 0x42c) = 0;
        *(int32_t *)((unsigned char *)st + 0x428) = 0;
        *(int32_t *)((unsigned char *)st + 0x424) = 0;
        D_80055c48(0x4001c);
        break;
    case 3:
        if (CARDGAME_F0x80087dc8(st, ctx) == 0)
            break;
        ((unsigned char *)st)[0x422] = 4;
        *(int32_t *)((unsigned char *)st + 0x42c) = 0;
        *(int32_t *)((unsigned char *)st + 0x428) = 0;
        *(int32_t *)((unsigned char *)st + 0x424) = 0;
        ((cardgame_9974_f28_t)*(uint32_t *)((unsigned char *)ctx + 0xf28))(ctx, *(int32_t *)((unsigned char *)st + 0x43c));
        break;
    case 4:
        if (*(int32_t *)((unsigned char *)st + 0x424) == 0x14) {
            ((cardgame_9974_f28_t)*(uint32_t *)((unsigned char *)ctx + 0xf28))(ctx, *(int32_t *)((unsigned char *)st + 0x43c) ^ 1);
            sel = 0x43;
            if (*(int32_t *)((unsigned char *)st + 0x434) == *(int32_t *)((unsigned char *)st + 0x440))
                sel = 0x44;
            ((cardgame_9974_eac_t)*(uint32_t *)((unsigned char *)ctx + 0xeac))(ctx, 5, 5, sel, 0, 0x42);
            D_80055c48(0x40019);
        }
        if (*(int32_t *)((unsigned char *)st + 0x424) >= 0x1f) {
            if (((((cardgame_9974_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_9974_sys1_t)DAT_8004B7D0[0x102])(0, 13)) & 1) != 0 ||
                ((((cardgame_9974_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_9974_sys1_t)DAT_8004B7D0[0x102])(0, 14)) & 1) != 0)
                *(int32_t *)((unsigned char *)st + 0x424) = 0x5a;
        }
        { int dres = D_8004df9c(); int c4 = *(int32_t *)((unsigned char *)st + 0x424) + dres;
        *(int32_t *)((unsigned char *)st + 0x424) = c4;
        if (c4 < 0x5b)
            break;
        ((unsigned char *)st)[0x422] = 5;
        *(int32_t *)((unsigned char *)st + 0x42c) = 0;
        *(int32_t *)((unsigned char *)st + 0x428) = 0;
        *(int32_t *)((unsigned char *)st + 0x424) = 0;
        ((cardgame_9974_eb0_t)*(uint32_t *)((unsigned char *)ctx + 0xeb0))(ctx, 5);
        break;
        }
    case 5: {
        int v5 = *(int32_t *)((unsigned char *)st + 0x424);
        void *a0_5;
        if (v5 == 0)
            goto c5_zero;
        if (v5 == 4)
            goto c5_four;
        goto c5_tail;
c5_zero:
        a0_5 = ctx;
        a1_1 = 0;
        goto c5_call;
c5_four:
        a0_5 = ctx;
        a1_1 = 1;
c5_call:
        ((cardgame_9974_f24_t)*(uint32_t *)((unsigned char *)a0_5 + 0xf24))(a0_5, a1_1, 5, 0, 0x1000);
c5_tail:
        cnt = *(int32_t *)((unsigned char *)st + 0x424) + 1;
        *(int32_t *)((unsigned char *)st + 0x424) = cnt;
        if (cnt < 15)
            break;
        ((unsigned char *)st)[0x422] = 6;
        break;
    }
    case 6:
        if (*(int32_t *)((unsigned char *)st + 0x434) == *(int32_t *)((unsigned char *)st + 0x440))
            *(int32_t *)((unsigned char *)st + 0x440) = 1;
        else
            *(int32_t *)((unsigned char *)st + 0x440) = 0;
        ret = 1;
        break;
    }
    return ret;
}
