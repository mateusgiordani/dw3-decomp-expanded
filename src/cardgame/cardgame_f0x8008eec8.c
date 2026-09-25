/*
 * CARDGAME:0x8008eec8 CARDGAME_F0x8008eec8
 * 492 bytes at CARDGAME.PRO offset 0xc218 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *              -mno-split-addresses
 *  Variant     o2-g0-nosr-nosplit
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x8008eec8
 *  Symbols     D0x800a58f4=0x800a58f4 func_8001ebf8=0x8001ebf8
 *  Compare     492 bytes from 0x8008eec8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008eec8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Simbolos: func_8001ebf8=0x8001ebf8, D0x800a58f4=0x800a58f4. k e o flag de
 * skip (a0); j e o indice do scan. h e int para o lh.
 */

#include <stdint.h>

extern void func_8001ebf8(void *buf);
extern int16_t D0x800a58f4[];

typedef void (*cardgame_8eec8_cb_t)(int x);

int CARDGAME_F0x8008eec8(unsigned char *p, void *unused, int mode, unsigned int flags)
{
    void *buf[22];
    unsigned char *q;
    unsigned char *r;
    int off;
    int h;
    int ret;
    int i;
    int k;
    int j;
    cardgame_8eec8_cb_t cb;
    unsigned char *tab;
    int16_t *tbl;

    (void)unused;
    ret = 0;
    h = ret;
    func_8001ebf8(buf);
    i = 0;
    k = ret;
    p[0x445] = 0;
    q = p;
    off = -0x54;
    r = q;
    do {
        q[0x446] = 0;
        if (i < 6) {
            if (mode == 0) {
                if ((flags & 0x100) == 0) {
                    k = 1;
                    goto gate;
                }
            } else if ((flags & 0x200) == 0) {
                goto set_skip;
            }
            p[0x445] |= 1;
            if (i >= p[0x72c])
                goto set_skip;
            h = (int)*(int16_t *)(r + 0x72e);
            goto gate;
        }
        if (i >= 12)
            goto set_skip;
        if (mode == 0) {
            if ((flags & 0x200) == 0) {
                k = 1;
                goto gate;
            }
        } else if ((flags & 0x100) == 0) {
            goto set_skip;
        }
        p[0x445] |= 2;
        if (i - 6 >= p[0x79e])
            goto set_skip;
        h = (int)*(int16_t *)(p + off + 0x7a0);
        goto gate;
set_skip:
        k = 1;
gate:
        if (k == 0) {
            cb = (cardgame_8eec8_cb_t)buf[11];
            cb(*(short *)(p + (h << 1) + 0x50) + 1);
            tab = (unsigned char *)buf[0];
            j = 0;
            tbl = D0x800a58f4;
            for (; j < 6; j++, tbl++) {
                if (((flags & *tbl) != 0) && (tab[0] == j + 1)) {
                    q[0x446] = 1;
                    ret = 1;
                    break;
                }
            }
        }
        q++;
        off += 0xe;
        r += 0xe;
        i++;
        k = 0;
    } while (i < 15);
    return ret;
}
