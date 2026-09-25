/*
 * CARDGAME:0x8008722c CARDGAME_F0x8008722c
 * 220 bytes at CARDGAME.PRO offset 0x457c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008722c
 *  Symbols     CARDGAME_F0x80085fd0=0x80085fd0
 *  Compare     220 bytes from 0x8008722c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008722c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Frame -0x30/+0x30; saves s6,s4,ra,s5,s3,s2,s1,s0.
 *
 * Semantics: n = lh(h+10); if (n <= 0) return; i = 0; pc = p; qc = q; hc = h;
 * do { r = CARDGAME_F0x80085fd0(p, h+12, (int)hc[50]); s1[0x151] = (r == 0 &&
 * pc[0x46f] == 0); pc[0x446] = (r != 0 && pc[0x46f] == 0); qc += 0x4c; pc++;
 * hc++; } while (lh(h+10) > ++i).
 *
 * The guard compares the count against the counter, which is zero there, so the
 * counter is referenced once before the loop.
 */

extern unsigned int CARDGAME_F0x80085fd0(void *p, unsigned char *k, int idx);

void CARDGAME_F0x8008722c(unsigned char *p, unsigned char *q, unsigned char *h)
{
    int i = 0;
    short *hc;

    if (*(short *)(h + 10) > i) {
        int one = 1;
        unsigned char *qc = q;
        unsigned char *pc = p;
        unsigned int r;
        int c;
        hc = (short *)h;

        do {
            r = CARDGAME_F0x80085fd0(p, h + 12, (int)hc[50]);
            if (r != 0) {
                c = *(signed char *)(pc + 0x46f);
                if (c != 0)
                    goto l_b0;
                *(unsigned char *)(pc + 0x446) = one;
                goto l_c0;
            } else {
                c = *(signed char *)(pc + 0x46f);
            l_b0:
                *(unsigned char *)(pc + 0x446) = 0;
                if (c != 0)
                    goto l_c0;
                *(unsigned char *)(qc + 0x151) = one;
                goto l_adv;
            l_c0:
                *(unsigned char *)(qc + 0x151) = 0;
            }
        l_adv:
            qc += 0x4c;
            pc++;
            hc++;
        } while (*(short *)(h + 10) > ++i);
    }
}
