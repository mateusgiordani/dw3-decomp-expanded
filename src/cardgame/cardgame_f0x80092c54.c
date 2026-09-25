/*
 * CARDGAME:0x80092c54 CARDGAME_F0x80092c54
 * 524 bytes at CARDGAME.PRO offset 0xffa4 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x80092c54
 *  Symbols     CARDGAME_F0x80092c54=0x80092c54 D_800a5958=0x800a5958
 *  Compare     524 bytes from 0x80092c54 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80092c54
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffd0 addiu sp,-0x30 ; sw s4/s5/s6/s3/s2/s1/s0/ra ; move s4,a0 ;
 * move s5,a1
 *
 * Body: init *(p1+0x424/0x428/0x434)=0, *(p1+0x42c)=lh(p1+0x5a4),
 * *(p1+0x430)=lh(p1+0x66c); copy 2x6 halves p1+outer*200+inner*2+0x600 from
 * pool *(p1+lh(row+0x5a0)*2+outer*200+0x5b0), row cursors
 * +0x5a6++/+0x5a4--/+0x5a0++; then 6x f14/f3c rounds (idx 0..5, table row
 * *0x8005ccb0 at 0x800a5958+idx*16+0/+4, +0x14800) writing p2+i*0x4c
 * (0x14d=2,0x122/0x120=0) reading p1+0x600+i*2; then 6x rounds (idx i+6, table
 * +8/+0xc) writing p2+0x1c8+i*0x4c reading p1+0x6c8+i*2.
 *
 * Epilogue lw s0-s6/ra + jr ra + addiu sp,+0x30 at 0x80092e58/0x80092e5c; next
 * CARDGAME:0x80092e60 prologue at +0x20c confirms size 0x20c contiguous
 * (prologue/epilogue pair).
 *
 * The per-inner destination p1+a1 reuses b and the copied halfword reuses a
 * (both assigned before), so sched1 gives their sets no birthing boost and dst
 * lands at the top of the inner body as in PAL; row updates in
 * 0x5a6/0x5a0/0x5a4 order; the inner counter is the pre-increment in the
 * do-while test; the f3c argument is a plain load with cur1 += 2 after the
 * call. Details: strategy-r9-o55/attempts-r9.
 */

#include <stdint.h>

extern int32_t D_800a5958[];

typedef void (*cardgame_f14_t)(int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_f3c_t)(int32_t, int32_t, int32_t);

void CARDGAME_F0x80092c54(int32_t p1, int32_t p2)
{
    int32_t a;
    int32_t b;
    int32_t row;
    int32_t outer_off;
    int32_t outer;
    int32_t t0;
    int32_t a0;
    int32_t a1;
    int32_t a2;
    int32_t off;
    int32_t cur1;
    int32_t cur2;
    int32_t i;
    int32_t kk;

    outer = 0;
    row = p1;
    a = *(int16_t *)(p1 + 0x5a4);
    b = *(int16_t *)(p1 + 0x66c);
    *(int32_t *)(p1 + 0x424) = 0;
    *(int32_t *)(p1 + 0x428) = 0;
    *(int32_t *)(p1 + 0x434) = 0;
    *(int32_t *)(p1 + 0x42c) = a;
    *(int32_t *)(p1 + 0x430) = b;
    outer_off = outer;
    do {
        a2 = 0;
        t0 = outer_off;
        a0 = row;
        a1 = outer_off;
        do {
            b = p1 + a1;
            off = (*(int16_t *)(a0 + 0x5a0) << 1) + t0;
            a = *(uint16_t *)(p1 + off + 0x5b0);
            *(int16_t *)(b + 0x600) = (int16_t)a;
            *(uint16_t *)(a0 + 0x5a6) += 1;
            *(uint16_t *)(a0 + 0x5a0) += 1;
            *(uint16_t *)(a0 + 0x5a4) -= 1;
            a1 += 2;
        } while (++a2 < 6);
        row += 200;
        outer += 1;
        outer_off += 200;
    } while (outer < 2);
    i = 0;
    {
        int32_t *tbl;
    tbl = D_800a5958;
    cur2 = p2;
    cur1 = p1;
    do {
        int32_t k;
        k = *(int32_t *)0x8005ccb0;
        ((cardgame_f14_t)*(uint32_t *)(p2 + 0xf14))(p2, i, *(tbl + k * 4) + 0x14800, *(tbl + k * 4 + 1));
        ((cardgame_f3c_t)*(uint32_t *)(p2 + 0xf3c))(p2, i, *(int16_t *)(cur1 + 0x600));
        cur1 += 2;
        i += 1;
        *(uint8_t *)(cur2 + 0x14d) = 2;
        *(int16_t *)(cur2 + 0x122) = 0;
        *(int16_t *)(cur2 + 0x120) = 0;
        cur2 += 0x4c;
    } while (i < 6);
    }
    {
        int32_t k;
        int32_t *tbl;
    i = 0;
    tbl = D_800a5958;
    k = 0x1c8;
    cur1 = p1;
    do {
        int32_t w;
        kk = i + 6;
        w = *(int32_t *)0x8005ccb0;
        ((cardgame_f14_t)*(uint32_t *)(p2 + 0xf14))(p2, kk, *(tbl + w * 4 + 2) + 0x14800, *(tbl + w * 4 + 3));
        ((cardgame_f3c_t)*(uint32_t *)(p2 + 0xf3c))(p2, kk, *(int16_t *)(cur1 + 0x6c8));
        cur1 += 2;
        i += 1;
        *(uint8_t *)(p2 + k + 0x14d) = 2;
        *(int16_t *)(p2 + k + 0x122) = 0;
        *(int16_t *)(p2 + k + 0x120) = 0;
        k += 0x4c;
    } while (i < 6);
    }
}
