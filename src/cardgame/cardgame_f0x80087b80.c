// CARDGAME:0x80087b80 (size 584, 0x248) -- portable C, exact_byte_match
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x4ed0.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, variant base (-O2 -G0).
// Link symbols: --symbol CARDGAME_F0x80087b80=0x80087b80
//   --symbol CARDGAME_F0x80086f24=0x80086f24 --symbol D_8004B7D0=0x8004B7D0
//   --symbol D_80055c48=0x80055c48
// Caller: CARDGAME_F0x80087edc via jal at 0x80088634. Next function
// CARDGAME:0x80087dc8 at +0x248; no overlap.
// Body: set bit0 at p2+idx*76+0x150 and half 1 at +0x12e (idx=*(p1+0x43c)
// re-read, stride 76); r0=(*0x8004bbc4)(0), r1=(*0x8004bbd8)(0,14): if
// ((r0>>r1)&1) (*0x80055c48)(0x800450bd), *(p1+0x440)=-1, *(p1+0x423)=10;
// if p3!=0: two bitmask rounds (kinds 7 then 5); round1 && idx>0 ->
// CARDGAME_F0x80086f24(p1,p2,p3,-1); round2 && idx<p3-1 -> (...,1).
//
// Matching-critical shapes (evidence: RTL -da dumps, see
// docs/c-matching-guide/submissions/cardgame-80087b80/strategy-r9-o55.md):
// - Each round is one expression (A & (1 << B)) | (C & (1 << D)): the four
//   calls are pre-expanded, so each result copy sinks into the next jalr
//   delay slot and the 2-arg pointers load into v1, as in PAL.
// - Round 1 indexes the local tbl (s3); round 2 indexes D_8004B7D0 directly,
//   giving PAL's fresh base in s0, which round 2's third result then reuses.
#include <stdint.h>

typedef int32_t (*cardgame_tbl0_t)(int32_t);
typedef int32_t (*cardgame_tbl1_t)(int32_t, int32_t);
typedef void (*cardgame_exe_cb_t)(uint32_t);

extern cardgame_exe_cb_t D_80055c48;

extern uint32_t D_8004B7D0[];

void CARDGAME_F0x80086f24(int32_t p1, int32_t p2, int32_t p3, int32_t step);

void CARDGAME_F0x80087b80(int32_t p1, int32_t p2, int32_t p3)
{
    uint32_t *tbl;
    int32_t r0;
    int32_t r1;
    int32_t off;

    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(uint8_t *)(p2 + off + 0x150) |= 1;
    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(int16_t *)(p2 + off + 0x12e) = 1;
    tbl = D_8004B7D0;
    r0 = ((cardgame_tbl0_t)tbl[253])(0);
    r1 = ((cardgame_tbl1_t)tbl[258])(0, 14);
    if (((r0 >> r1) & 1) != 0) {
        D_80055c48(0x800450bd);
        *(int32_t *)(p1 + 0x440) = -1;
        *(uint8_t *)(p1 + 0x423) = 10;
    }
    if (p3 != 0) {
        int32_t a;

        a = (((cardgame_tbl0_t)tbl[255])(0) & (1 << ((cardgame_tbl1_t)tbl[258])(0, 7)))
          | (((cardgame_tbl0_t)tbl[253])(0) & (1 << ((cardgame_tbl1_t)tbl[258])(0, 7)));
        if ((a != 0) && (*(int32_t *)(p1 + 0x43c) > 0)) {
            CARDGAME_F0x80086f24(p1, p2, p3, -1);
        }
        a = (((cardgame_tbl0_t)D_8004B7D0[255])(0) & (1 << ((cardgame_tbl1_t)D_8004B7D0[258])(0, 5)))
          | (((cardgame_tbl0_t)D_8004B7D0[253])(0) & (1 << ((cardgame_tbl1_t)D_8004B7D0[258])(0, 5)));
        if ((a != 0) && (*(int32_t *)(p1 + 0x43c) < p3 - 1)) {
            CARDGAME_F0x80086f24(p1, p2, p3, 1);
        }
    }
}
