/*
 * CARDGAME:0x80087b80 CARDGAME_F0x80087b80
 * 584 bytes at CARDGAME.PRO offset 0x4ed0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80087b80
 *  Symbols     CARDGAME_F0x80086f24=0x80086f24 CARDGAME_F0x80087b80=0x80087b80
 *              D_8004B7D0=0x8004b7d0 D_80055c48=0x80055c48
 *  Compare     584 bytes from 0x80087b80 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80087b80
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Link symbols: --symbol CARDGAME_F0x80087b80=0x80087b80 --symbol
 * CARDGAME_F0x80086f24=0x80086f24 --symbol D_8004B7D0=0x8004B7D0 --symbol
 * D_80055c48=0x80055c48
 *
 * Caller: CARDGAME_F0x80087edc via jal at 0x80088634. Next function
 * CARDGAME:0x80087dc8 at +0x248; no overlap.
 *
 * Body: set bit0 at p2+idx*76+0x150 and half 1 at +0x12e (idx=*(p1+0x43c)
 * re-read, stride 76); r0=(*0x8004bbc4)(0), r1=(*0x8004bbd8)(0,14): if
 * ((r0>>r1)&1) (*0x80055c48)(0x800450bd), *(p1+0x440)=-1, *(p1+0x423)=10; if
 * p3!=0: two bitmask rounds (kinds 7 then 5); round1 && idx>0 ->
 *
 * CARDGAME_F0x80086f24(p1,p2,p3,-1); round2 && idx<p3-1 -> (...,1).
 *
 * - Round 1 indexes the local tbl (s3); round 2 indexes D_8004B7D0 directly,
 * giving PAL's fresh base in s0, which round 2's third result then reuses.
 */

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
