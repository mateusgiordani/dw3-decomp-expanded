/*
 * CARDGAME:0x80093710 CARDGAME_F0x80093710
 * 972 bytes at CARDGAME.PRO offset 0x10a60 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80093710
 *  Symbols     CARDGAME_F0x80093710=0x80093710 D0x800a5958=0x800a5958
 *              D_8005ccb0=0x8005ccb0
 *  Compare     972 bytes from 0x80093710 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80093710
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue addiu sp,-0x38 with saves s0-s5/ra (cc1 emits subu,
 * folded to addiu by aspsx); epilogue jr ra / addiu sp,+0x38 at
 * 0x80093ad4/0x80093ad8; body ends 0x80093adc.
 *
 * Flag dispatch on byte p+0x422 (lbu; 1 = main body, 2 = return 1).
 *
 * Indirect callees via second context qb (loaded overlay slots, preserved as
 * indirect): +0xf10 (5 args), +0xea0 x2 (4 args), +0xf2c (2 args, loop).
 *
 * Data: EXE word D_8005ccb0 (lui 0x8006/lw -0x3350); CARDGAME 16-byte-entry
 * table D0x800a5958 indexed by D_8005ccb0 (sll 4 + addu, lw 0).
 *
 * Row strides from shift/add sequences: p-rows 0x72 (byte at +0x72c), q-rows
 * 0x4c (short at +0x12e), p-rows 200 (short at +0x59e); /27 via mult-magic
 * 0x4bda12f7 with multiply-back compare; the runtime div is the bare div+mflo
 * shape of card-cage overlays (ASPSX guard stripped: --strip-div-guard).
 */

#include <stdint.h>

extern int D_8005ccb0;
extern int D0x800a5958[];

/* Views that index 114-byte rows at p + 0x72c, 200-byte entries at p + 0x59c and
   76-byte rows at q + 0x120 as member arrays (base-first address arithmetic). */
typedef struct { uint8_t n; uint8_t pad[113]; } cg3710_row_t;
typedef struct { uint8_t pad[0x72c]; cg3710_row_t r[2]; } cg3710_rows_t;
typedef struct { uint8_t pad0[2]; int16_t h02; uint8_t pad[196]; } cg3710_ent_t;
typedef struct { uint8_t pad[0x59c]; cg3710_ent_t e[2]; } cg3710_ents_t;
typedef struct { uint8_t pad0[0xe]; int16_t h0e; uint8_t pad[60]; } cg3710_qrow_t;
typedef struct { uint8_t pad[0x120]; cg3710_qrow_t r[12]; } cg3710_q_t;

typedef void (*CgCall5)(void *ctx, int a1, int a2, int a3, int a4);
typedef void (*CgCall4)(void *ctx, int a1, int a2, int a3);
typedef void (*CgCall2)(void *ctx, int a1);

int CARDGAME_F0x80093710(char *pb, char *qb, int idx)
{
    int j = idx ^ 1;
    int ret = 0;
    unsigned char flag = *(unsigned char *)(pb + 0x422);

    switch (flag) {
    case 1: {
        int cnt = *(int *)(pb + 0x424);
        if (cnt == cnt / 27 * 27) {
            if (*(int *)(pb + 0x428) < (int)((cg3710_rows_t *)pb)->r[idx].n) {
                int s0 = *(int *)(pb + 0x428);
                int dv;
                int a3;
                if (idx != 0)
                    s0 += 6;
                a3 = D0x800a5958[(dv = D_8005ccb0) * 4] + (((cg3710_rows_t *)pb)->r[j].n - 1) * 0x1480;
                if (dv != 0)
                    ((CgCall5)*(void **)(qb + 0xf10))(qb, s0, 6, a3, idx != 0 ? 0x6d00 : 0x5500);
                else
                    ((CgCall5)*(void **)(qb + 0xf10))(qb, s0, 6, a3, 0x6100);
                *(int *)(pb + 0x428) += 1;
                ((cg3710_q_t *)qb)->r[s0].h0e = 1;
            }
        }
        {
            int cnt2 = *(int *)(pb + 0x424);
            if (cnt2 >= 16) {
                unsigned char lim = ((cg3710_rows_t *)pb)->r[idx].n;
                int a3b = 0;
                if (cnt2 < (int)lim * 28) {
                    int dd = (int)lim * 56 + 1;
                    int d = *(int *)(pb + 0x42c);
                    a3b = d - (d / dd + 1) * cnt2;
                    if (a3b < 0)
                        a3b = 0;
                }
                ((CgCall4)*(void **)(qb + 0xea0))(qb, idx, 8, a3b);
                if (*(int *)(pb + 0x424) < (int)((cg3710_rows_t *)pb)->r[idx].n * 28) {
                    int diff = *(int *)(pb + 0x430) - *(int *)(pb + 0x434);
                    *(int *)(pb + 0x430) = diff;
                    {
                        int shv = ((int)((cg3710_ents_t *)pb)->e[j].h02) << 8;
                        if (diff < shv)
                            *(int *)(pb + 0x430) = shv;
                    }
                } else {
                    *(int *)(pb + 0x430) = ((int)((cg3710_ents_t *)pb)->e[j].h02) << 8;
                }
                ((CgCall4)*(void **)(qb + 0xea0))(qb, j, 9, *(int *)(pb + 0x430) >> 8);
            }
        }
        if ((*(int *)(qb + 0x54) & 2) != 0) {
            int n = 0;
            if (((cg3710_rows_t *)pb)->r[j].n != 0) {
                char *row = pb + j * 114;
                do {
                    int av = n;
                    if (idx == 0)
                        av = n + 6;
                    ((CgCall2)*(void **)(qb + 0xf2c))(qb, av);
                    n++;
                } while (n < (int)*(unsigned char *)(row + 0x72c));
            }
        }
        {
            int nc = *(int *)(pb + 0x424) + 1;
            *(int *)(pb + 0x424) = nc;
            if ((int)((cg3710_rows_t *)pb)->r[idx].n * 28 + 25 < nc)
                pb[0x422] = 2;
        }
        break;
    }
    case 2:
        ret = 1;
        break;
    default:
        break;
    }
    return ret;
}
