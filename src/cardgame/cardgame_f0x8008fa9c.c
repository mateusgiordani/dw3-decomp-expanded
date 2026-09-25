/* CARDGAME:0x8008fa9c (size 1132, 0x46c) */
/* PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x0cdec */
/* Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #72 */
/* Prologue 27bdffd0 addiu sp,-0x30 ; epilogue jr ra + addiu sp,+0x30 at 0x8008ff00 */
/* Next CARDGAME:0x8008ff08 at +0x46c (contiguous, no gap) */
/* State machine on byte st+0x422 (cases 1..4, default returns 0; case 4 returns 1) */
/* Counter at st+0x424 accumulates EXE tick vector *0x8004df9c (lui 0x8005 + lw -0x2064) */
/* Table at 0x800a5900 indexed by idx*8 (two words passed as 4th/5th args to ctx+0xf08) */
/* Per-side stride 200 (0xc8) at st+idx*200; halfword arrays at +0x5a0/+0x5a2/+0x5a4/+0x5a6, */
/* +0x5b0/+0x600/+0x614; bytes at +0x30a/+0x30b, +0x41b/+0x41c; indirect ctx slots */
/* +0xf08/+0xf20/+0xea0; gate byte ctx+0x656 must be 1 in state 2 */
/* Ghidra CARDGAME read-only: disasm (283 + 150 insns) word-equal vs PAL, decompile */
/* CARDGAME_F0x8008fa9c, x-ref to: 3 callers in CARDGAME_F0x80084320 */
/*   (0x80085548 a2=s4,a3=0 / 0x8008556c a2=s2,a3=0 / 0x80085590 a2=s4,a3=1) */
/* Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base; the same
   object with o2-g0-no-strength-reduce): exact_byte_match 1132/1132 (r8 o55).
   The loops recompute idx * 200 / st + idx * 200 in their bodies so loop.c hoists
   copies into the preheaders (which also keeps the duplicated exit-test guards);
   each loop copies its decremented/incremented counter back (n) so the single-set
   n gets the sched1 boost; the fill loop reuses the else-branch counter c. */
#include "common/types.h"

typedef int32_t (*cardgame_fa9c_tick_t)(void);
extern cardgame_fa9c_tick_t D_8004df9c;
extern uint32_t D_800A5900[];

typedef void (*cardgame_fa9c_f08_t)(void *ctx, int32_t a1, int32_t a2, uint32_t a3, uint32_t a4);
typedef void (*cardgame_fa9c_f20_t)(void *ctx, int32_t a1, int32_t a2, int32_t a3);
typedef void (*cardgame_fa9c_ea0_t)(void *ctx, int32_t a1, int32_t a2, int32_t a3);

/* 200-byte per-side records at st + 0x59c (fields +4..+0xa = st+0x5a0..0x5a6). */
typedef struct {
    uint8_t pad00[4];
    int16_t h04;
    int16_t h06;
    int16_t h08;
    int16_t h0a;
    uint8_t pad0c[0xc8 - 0x0c];
} cardgame_fa9c_side_t;
typedef struct {
    uint8_t pad[0x59c];
    cardgame_fa9c_side_t side[2];
} cardgame_fa9c_st_t;
#define SIDE(i) (((cardgame_fa9c_st_t *)st)->side[i])

typedef struct { int8_t b[2]; } cardgame_fa9c_pair_t;
typedef struct { uint32_t a; uint32_t b; } cardgame_fa9c_ent_t;

int32_t CARDGAME_F0x8008fa9c(uint8_t *st, uint8_t *ctx, int32_t idx, int32_t mode)
{
    int32_t ret = 0;
    int16_t hv;

    switch (st[0x422]) {
    case 1:
        *(int32_t *)(st + 0x424) += D_8004df9c();
        if (*(int32_t *)(st + 0x424) < 0x15)
            break;
        ((cardgame_fa9c_f08_t)*(void **)(ctx + 0xf08))(ctx, 0x11, 10, ((cardgame_fa9c_ent_t *)D_800A5900)[idx].a, ((cardgame_fa9c_ent_t *)D_800A5900)[idx].b);
        ((cardgame_fa9c_f20_t)*(void **)(ctx + 0xf20))(ctx, 0x11, 0, 0);
        st[0x422] = 2;
        break;
    case 2:
        if (ctx[0x656] != 1)
            break;
        st[0x422] = 3;
        *(int32_t *)(st + 0x424) = 0;
        *(uint16_t *)(st + (idx * 200 + SIDE(idx).h06 * 2) + 0x614) = *(uint16_t *)(st + 0x438);
        SIDE(idx).h06 += 1;
        switch (mode) {
        case 0: {
            int32_t c = *(int32_t *)(st + 0x440);
            if (c < SIDE(idx).h0a - 1) {
                int32_t s = idx * 200;
                do {
                    int32_t o = c * 2 + s;
                    int32_t n = c + 1;
                    *(uint16_t *)(st + o + 0x600) = *(uint16_t *)(st + (n * 2 + s) + 0x600);
                    c = n;
                } while (c < SIDE(idx).h0a - 1);
            }
            hv = (int16_t)(SIDE(idx).h0a - 1);
            SIDE(idx).h0a = hv;
            ((cardgame_fa9c_ea0_t)*(void **)(ctx + 0xea0))(ctx, idx, 6, (int32_t)hv);
            break;
        }
        case 1:
            if (idx == 0) {
                int32_t c = *(int32_t *)(st + 0x440);
                while (SIDE(idx).h04 + 1 <= c) {
                    int32_t o = c * 2 + idx * 200;
                    int32_t n = c - 1;
                    *(uint16_t *)(st + o + 0x5b0) = *(uint16_t *)(st + (idx * 200 + n * 2) + 0x5b0);
                    c = n;
                }
                SIDE(idx).h04 += 1;
                SIDE(idx).h08 -= 1;
            } else {
                int32_t c = *(int32_t *)(st + 0x440);
                uint8_t *fp;

                while (SIDE(idx).h04 + 1 <= c) {
                    int32_t b = c * 2;
                    int32_t o = b + idx * 200;
                    int32_t n = c - 1;
                    int32_t e;
                    *(uint16_t *)(st + o + 0x5b0) = *(uint16_t *)(st + (idx * 200 + n * 2) + 0x5b0);
                    e = n * 2;
                    *(cardgame_fa9c_pair_t *)(st + b + 0x30a) = *(cardgame_fa9c_pair_t *)(st + e + 0x30a);
                    c = n;
                }
                st[0x41b] += 1;
                if (st[0x41b] > 0x27)
                    st[0x41b] = 0x27;
                st[0x41c] += 1;
                if (st[0x41c] > 0x27)
                    st[0x41c] = 0x27;
                SIDE(idx).h04 += 1;
                SIDE(idx).h08 -= 1;
                c = 0x27;
                fp = st + 0x4e;
                do {
                    fp[0x30a] = (uint8_t)c;
                    c--;
                    fp -= 2;
                } while (c >= 0);
            }
            ((cardgame_fa9c_ea0_t)*(void **)(ctx + 0xea0))(ctx, idx, 5, (int32_t)SIDE(idx).h08);
            break;
        }
        ((cardgame_fa9c_ea0_t)*(void **)(ctx + 0xea0))(ctx, idx, 7, (int32_t)SIDE(idx).h06);
        break;
    case 3:
        *(int32_t *)(st + 0x424) += D_8004df9c();
        if (*(int32_t *)(st + 0x424) < 0x2e)
            break;
        st[0x422] = 4;
        break;
    case 4:
        ret = 1;
        break;
    }
    return ret;
}
