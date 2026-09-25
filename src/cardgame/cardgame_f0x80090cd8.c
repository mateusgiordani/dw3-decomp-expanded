/*
 * CARDGAME:0x80090cd8 CARDGAME_F0x80090cd8
 * 2284 bytes at CARDGAME.PRO offset 0xe028 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80090cd8, jump table (.rodata) at 0x80083554
 *  Symbols     CARDGAME_F0x80090cd8=0x80090cd8 DAT_8004BBC4=0x8004bbc4
 *              DAT_8004BBD8=0x8004bbd8 DAT_8004DF9C=0x8004df9c
 *              DAT_8004b7d0=0x8004b7d0 EXE_F0x8001ebf8=0x8001ebf8
 *  Compare     2284 bytes from 0x80090cd8 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80090cd8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL-SLES-03936: 2284 bytes plus 40-byte switch table.
 *
 * Verified base 0x80082cb0; text file offset 0xe028; table 0x80083554.
 *
 * Callback offsets and field views follow PAL accesses; semantics remain
 * partial.
 *
 * Exact with PsyQ GCC 2.8.1-sn32-4.0.0010 / ASPSX 2.79, O2 -G0.
 */

#include <stdint.h>

extern void EXE_F0x8001ebf8(void *work);
extern uint8_t DAT_8004b7d0[];
extern void *DAT_8004DF9C;

typedef void (*cg_ee4_t)(void *ctx, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
typedef int32_t (*cg_ed8_t)(int32_t a0, int32_t a1);
typedef void (*cg_f08_t)(void *ctx, int32_t a1, int32_t a2, int32_t a3, int32_t a4);
typedef void (*cg_ea0_t)(void *ctx, int32_t a1, int32_t a2, int32_t a3);
typedef void (*cg_f1c_t)(void *ctx, int32_t a1);
typedef void (*cg_ee8_t)(void *ctx);
typedef void (*cg_ebc_t)(void *ctx, int32_t a1);
typedef void (*cg_local_t)(int32_t a0);
typedef int32_t (*cg_exe0_t)(int32_t a0);
typedef uint32_t (*cg_exe1_t)(int32_t a0, int32_t a1);
typedef int32_t (*cg_exe2_t)(void);

typedef struct { int8_t x, y; } CgPair;
typedef struct { uint8_t data[0x1000]; } CgByteView;
typedef struct { uint8_t pad[0x46f]; int8_t flag[40]; } CgScan;
typedef struct { uint8_t pad[0x46f]; uint8_t flag[40]; } CgFlags;
typedef struct { uint8_t pad[0x5b0]; uint16_t h[40]; } CgLeft;
typedef struct { uint8_t pad[0x678]; uint16_t h[40]; } CgRight;
typedef struct { uint8_t pad[0x678]; int16_t h[40]; } CgRightSigned;
typedef struct { uint8_t pad[0x30a]; CgPair pair[40]; } CgPairs;
typedef struct { uint8_t pad[0x5a8]; uint8_t value[1]; } CgCategory;
typedef struct { uint8_t pad[0x5a6]; int16_t bound[1][100]; } CgBounds;
typedef struct { uint8_t pad[0x50]; int16_t h[1]; } CgIds;
typedef struct { uint8_t pad[0x120]; uint16_t cell[1][38]; } CgCell;
typedef struct { uint8_t pad[0x14a]; uint8_t cell[1][76]; } CgCellState;


typedef struct {
    uint32_t w[11];
    cg_local_t fn;
    uint8_t tail[0x24];
} CgWork0cd8;

int32_t CARDGAME_F0x80090cd8(uint8_t *b, uint8_t *c, int32_t side)
{
    int32_t ret = 0;
    CgWork0cd8 work;

    switch (((CgByteView *)b)->data[0x422]) {
    case 1: {
        int32_t idx, ok;
        ok = 0;
        if (side == 0)
            ok = (*(int16_t *)(c + 0x64) == 2);
        else if (*(int16_t *)(c + 0xb8) == 2)
            ok = 1;
        for (idx = *(int32_t *)(b + 0x438); idx < ((CgBounds *)b)->bound[side][0]; idx++) {
            *(int32_t *)(c + idx * 76 + 0x108) = 0x14a00;
        }
        if (ok && ((CgByteView *)b)->data[0x498] == 0) {
            if (*(int32_t *)(b + 0x42c) == 0) {
                (*(cg_ee4_t)*(void **)(c + 0xee4))((void *)c, 0x35, 0, 0, side == 0 ? 2 : 0);
                ((CgByteView *)b)->data[0x422] = 5;
            } else {
                ((CgByteView *)b)->data[0x422] = 2;
            }
            *(int32_t *)(b + 0x424) = 0;
            *(int32_t *)(b + 0x428) = *(int32_t *)(b + 0x438);
        }
        break;
    }
    case 2: {
        int32_t idx, v0;
        idx = *(int32_t *)(b + 0x428);
        v0 = (*(cg_ed8_t)*(void **)(c + 0xed8))(
            ((CgBounds *)b)->bound[side][0], idx);
        idx = *(int32_t *)(b + 0x428);
        ((CgCell *)c)->cell[idx][0] = 0x1000;
        (*(cg_f08_t)*(void **)(c + 0xf08))((void *)c, *(int32_t *)(b + 0x428), 0xf, v0 + 0x1800, 0x6100);
        ((CgByteView *)b)->data[0x422] = 3;
        break;
    }
    case 3: {
        int32_t idx, i, v0, ok;
        uint8_t *cellp;
        idx = *(int32_t *)(b + 0x428);
        if (((CgCellState *)c)->cell[idx][0] != 1)
            break;
        {
            int32_t twice_idx = idx * 2;
            int32_t base = side * 200;
            int32_t sel;
            uint8_t *n;
            sel = *(int16_t *)(b + (twice_idx + base) + 0x600);
            EXE_F0x8001ebf8(&work);
            work.fn(((CgIds *)b)->h[sel] + 1);
            n = (uint8_t *)(void *)work.w[0];
            if (*n < 6) {
                uint8_t nv;
                uint8_t *countp;
                int32_t category_base;
                category_base = base;
                category_base -= 1;
                countp = b + (*n + category_base);
                nv = countp[0x5a8];
                if (nv < 99)
                    countp[0x5a8] = (uint8_t)(nv + 1);
                (*(cg_ea0_t)*(void **)(c + 0xea0))((void *)c, side, *(uint8_t *)(void *)work.w[0] - 1, ((CgCategory *)b)->value[*(uint8_t *)(void *)work.w[0] + category_base]);
                (*(cg_f1c_t)*(void **)(c + 0xf1c))((void *)c, *(int32_t *)(b + 0x428));
                ((CgByteView *)b)->data[0x422] = 4;
            } else {
                int32_t next = *(int32_t *)(b + 0x428) + 1;
                *(int32_t *)(b + 0x428) = next;
                if (next < ((CgBounds *)b)->bound[side][0]) {
                    ((CgByteView *)b)->data[0x422] = 2;
                } else if (((CgByteView *)b)->data[0x445] != 0) {
                    (*(cg_ee4_t)*(void **)(c + 0xee4))((void *)c, 0x35, 0, 0, side == 0 ? 2 : 0);
                    ((CgByteView *)b)->data[0x422] = 5;
                } else {
                    *(int32_t *)(b + 0x424) = 0x2d;
                    ((CgByteView *)b)->data[0x422] = 8;
                }
            }
        }
        /* Keep the scan index distinct from the left-copy predecessor. */
        i = 0x27;
        ok = 0;
        for (; i >= 0; i--) {
            if (((CgScan *)b)->flag[i] != 0) {
                ok = 1;
                break;
            }
        }
        if (ok) {
            ((CgFlags *)b)->flag[i] = 0;
            if (side == 0) {
                int32_t j = i;
                if (*(int16_t *)(b + 0x5a0) + 1 <= j) {
                    do {
                        int32_t predecessor;
                        uint32_t destination = j;
                        destination <<= 1;
                        predecessor = j - 1;
                        destination = (uint32_t)b - -destination;
                        *(uint16_t *)(destination + 0x5b0) = ((CgLeft *)b)->h[predecessor];
                        ((CgFlags *)b)->flag[j] = ((CgFlags *)b)->flag[predecessor];
                        j = predecessor;
                    } while (*(int16_t *)(b + 0x5a0) + 1 <= j);
                }
            } else {
                int32_t j;
                if (i < ((CgByteView *)b)->data[0x41b]) {
                    j = i;
                    if (*(int16_t *)(b + 0x668) + 1 <= j) {
                        do {
                            int32_t dest_offset = j * 2;
                            int32_t k = j - 1;
                            ((CgRight *)(b + dest_offset))->h[0] = ((CgRight *)b)->h[k];
                            ((CgFlags *)b)->flag[j] = ((CgFlags *)b)->flag[k];
                            ((CgPairs *)(b + dest_offset))->pair[0] = ((CgPairs *)b)->pair[k];
                            j = k;
                        } while (*(int16_t *)(b + 0x668) + 1 <= j);
                    }
                } else {
                    if ((uint8_t)((CgPairs *)b)->pair[i].y == 7) {
                        /* Preserve separate entry and continuation value flows. */
                        int32_t limit = *(int16_t *)(b + 0x668);
                        j = i;
                        goto selected_test;
selected_body:
                        do {
                            int32_t dest_offset = j * 2;
                            int32_t k = j - 1;
                            ((CgRight *)(b + dest_offset))->h[0] = ((CgRight *)b)->h[k];
                            ((CgFlags *)b)->flag[j] = ((CgFlags *)b)->flag[k];
                            ((CgPairs *)(b + dest_offset))->pair[0] = ((CgPairs *)b)->pair[k];
                            limit = *(int16_t *)(b + 0x668);
                            j = k;
                        } while (limit + 1 <= j);
                        goto selected_done;
selected_test:
                        if (limit + 1 <= j)
                            goto selected_body;
selected_done:
                        ;
                    } else {
                        int32_t tmp = ((CgRightSigned *)b)->h[i];
                        ((CgRight *)b)->h[i] = *(uint16_t *)(b + 0x6c6);
                        *(uint16_t *)(b + 0x6c6) = tmp;
                        j = 0x27;
                        while (*(int16_t *)(b + 0x668) + 1 <= j) {
                            int32_t dest_offset = j * 2;
                            int32_t k = j - 1;
                            ((CgRight *)(b + dest_offset))->h[0] = ((CgRight *)b)->h[k];
                            ((CgFlags *)b)->flag[j] = ((CgFlags *)b)->flag[k];
                            ((CgPairs *)(b + dest_offset))->pair[0] = ((CgPairs *)b)->pair[k];
                            j = k;
                        }
                    }
                    ((CgByteView *)b)->data[0x41b]++;
                    ((CgByteView *)b)->data[0x41c]++;
                }
                for (v0 = 0x27, cellp = b + 0x4e; v0 >= 0; v0--, cellp -= 2)
                    cellp[0x30a] = v0;
            }
            {
                uint8_t *row = b + side * 200;
                *(uint16_t *)(row + 0x5a0) += 1;
                *(uint16_t *)(row + 0x5a4) -= 1;
            }
        }
        break;
    }
    case 4: {
        int32_t idx;
        idx = *(int32_t *)(b + 0x428);
        if (((CgCellState *)c)->cell[idx][0] != 1)
            break;
        {
            int32_t next = idx + 1;
            *(int32_t *)(b + 0x428) = next;
            if (next < ((CgBounds *)b)->bound[side][0]) {
                ((CgByteView *)b)->data[0x422] = 2;
                break;
            }
            if (((CgByteView *)b)->data[0x445] != 0) {
                (*(cg_ee4_t)*(void **)(c + 0xee4))((void *)c, 0x35, 0, 0, side == 0 ? 2 : 0);
                ((CgByteView *)b)->data[0x422] = 5;
                break;
            }
            *(int32_t *)(b + 0x424) = 0x2d;
            ((CgByteView *)b)->data[0x422] = 8;
            break;
        }
    }
    case 5: {
        if (c[0xdfa] == 2)
            ((CgByteView *)b)->data[0x422] = 6;
        break;
    }
    case 6: {
        uint8_t *exe = DAT_8004b7d0;
        int32_t r1;
        r1 = (*(cg_exe0_t *)(exe + 0x3f4))(0) >> (*(cg_exe1_t *)(exe + 0x408))(0, 0xd);
        r1 &= 1;
        if (r1 == 0) {
            r1 = (*(cg_exe0_t *)(exe + 0x3f4))(0) >> (*(cg_exe1_t *)(exe + 0x408))(0, 0xe);
            r1 &= 1;
            if (r1 == 0) break;
        }
        ((CgByteView *)b)->data[0x422] = 7;
        (*(cg_ee8_t)*(void **)(c + 0xee8))((void *)c);
        break;
    }
    case 7: {
        if (c[0xdfa] != 0)
            break;
        *(int32_t *)(b + 0x424) = 0x2d;
        ((CgByteView *)b)->data[0x422] = 8;
        break;
    }
    case 8: {
        int32_t v0;
        (*(cg_ea0_t)*(void **)(c + 0xea0))((void *)c, 0, 5, *(int16_t *)(b + 0x5a4));
        (*(cg_ea0_t)*(void **)(c + 0xea0))((void *)c, 0, 6, *(int16_t *)(b + 0x5a6));
        (*(cg_ea0_t)*(void **)(c + 0xea0))((void *)c, 0, 7, *(int16_t *)(b + 0x5a2));
        (*(cg_ea0_t)*(void **)(c + 0xea0))((void *)c, 1, 5, *(int16_t *)(b + 0x66c));
        (*(cg_ea0_t)*(void **)(c + 0xea0))((void *)c, 1, 6, *(int16_t *)(b + 0x66e));
        (*(cg_ea0_t)*(void **)(c + 0xea0))((void *)c, 1, 7, *(int16_t *)(b + 0x66a));
        v0 = (*(cg_exe2_t)DAT_8004DF9C)();
        v0 = *(int32_t *)(b + 0x424) - v0;
        *(int32_t *)(b + 0x424) = v0;
        if (v0 < 1) {
            ((CgByteView *)b)->data[0x422] = 9;
            (*(cg_ebc_t)*(void **)(c + 0xebc))((void *)c, side);
            ((CgByteView *)b)->data[0x499] = ((CgByteView *)b)->data[0x49b];
        }
        break;
    }
    case 9: {
        int32_t ok;
        ok = 0;
        if (side == 0)
            ok = (*(int16_t *)(c + 0x64) == 0);
        else if (*(int16_t *)(c + 0xb8) == 0)
            ok = 1;
        if (ok && ((CgByteView *)b)->data[0x498] == 0)
            ((CgByteView *)b)->data[0x422] = 10;
        break;
    }
    case 10: {
        ret = 1;
        break;
    }
    }
    return ret;
}
