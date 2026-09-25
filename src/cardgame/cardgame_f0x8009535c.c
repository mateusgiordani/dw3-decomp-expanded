/*
 * CARDGAME:0x8009535c CARDGAME_F0x8009535c
 * 412 bytes at CARDGAME.PRO offset 0x126ac (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009535c
 *  Symbols     CARDGAME_F0x8009535c=0x8009535c D_80055c48=0x80055c48
 *  Compare     412 bytes from 0x8009535c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009535c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Caller: CARDGAME_F0x80084320 via 0x80085828 (return 0/1/2 =
 * stay/continue/advance to 0x4d).
 *
 * Callee: EXE vector *(uint32_t *)0x80055c48 with lui+ori arg (0x80040000 |
 * 0x52c6), same idiom as CARDGAME:0x80088f78 case 3.
 *
 * Both loops are real do-while loops, so flow weights refs by depth and global
 * alloc reproduces PAL homes (a a2, idx a3, dirty t4, outer t5, mark t6); the
 * clear loop is a goto loop (idx weight). With the byte read into a short
 * widened to int and the target a short, the inner loop has 59 insns at loop.c
 * time, so the two stored 1s stay in the loop as in PAL.
 */

#include <stdint.h>

typedef void (*cardgame_exe_vec_t)(uint32_t arg);

/* H13 (symbolic-absolute-base, cf. CARDGAME:0x8008ddc0 r7): reference the EXE
 * vector through the extern symbol D_80055c48 (--symbol D_80055c48=0x80055c48)
 * so cc1 emits separate lui/lw and the lui can fill the dirty-beq delay slot. */
extern cardgame_exe_vec_t D_80055c48;

int32_t CARDGAME_F0x8009535c(int32_t p1, int32_t p2)
{
    int32_t ret = 0;
    int32_t mode = *(uint8_t *)(p1 + 0x422);
    int32_t dirty = ret;
    int32_t outer;
    int32_t cur1;
    int32_t off;
    int32_t mark;
    uint8_t *row;
    int32_t idx;
    int32_t m;
    int32_t o;
    int32_t k;
    uint8_t *a;
    int16_t cur;
    int32_t c;
    int16_t targ;

    switch (mode) {
    case 1:
    default: {
        uint8_t *q;
        idx = 11;
        q = (uint8_t *)(p1 + idx);
clear_top:
        *(uint8_t *)(q + 0x46f) = 0;
        idx--;
        q--;
        if (idx >= 0)
            goto clear_top;
        *(uint8_t *)(p1 + 0x422) = 2;
        *(int32_t *)(p1 + 0x438) = 0;
        break;
    }
    case 2: {
        outer = 0;
        ret = 1;
        cur1 = p1;
        off = outer;
        mark = outer;
        do {
            row = (uint8_t *)(p2 + 0x2d0);
            if (outer == 0)
                row = (uint8_t *)(p2 + 0x108);
            idx = 0;
            if (*(uint8_t *)(cur1 + 0x72c) != 0) {
                m = mark;
                o = off;
                k = idx;
                a = row;
                do {
                    cur = a[0x43];
                    c = cur;
                    targ = *(int16_t *)(p1 + (k + off) + 0x734);
                    if (c != targ) {
                        targ = (targ < c) ? c - 1 : c + 1;
                        a[0x43] = targ;
                        ret = 0;
                        dirty = 1;
                    }
                    cur = a[0x44];
                    c = cur;
                    targ = *(int16_t *)(p1 + o + 0x736);
                    if (c != targ) {
                        targ = (targ < c) ? c - 1 : c + 1;
                        a[0x44] = targ;
                        ret = 0;
                        if (*(int16_t *)(p1 + o + 0x736) == 0) {
                            *(uint8_t *)(p1 + (idx + m) + 0x46f) = 1;
                            *(int32_t *)(p1 + 0x438) = 1;
                        }
                        dirty = 1;
                    }
                    o += 0xe;
                    k += 0xe;
                    a += 0x4c;
                    targ = *(uint8_t *)(cur1 + 0x72c);
                    idx += 1;
                } while (idx < targ);
            }
            cur1 += 0x72;
            off += 0x72;
            outer += 1;
            mark += 6;
        } while (outer < 2);
        if (ret != 0 && *(int32_t *)(p1 + 0x438) != 0)
            ret = 2;
        if (dirty != 0)
            D_80055c48(0x80040000 | 0x52c6);
        break;
    }
    }
    return ret;
}