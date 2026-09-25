// CARDGAME:0x8008722c (size 220, 0xdc)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x457c
// Boundary: prologue 27bdffd0 (sp,-0x30) at 0x8008722c, epilogue jr ra +
// 27bd0030 at 0x80087300/04; prev CARDGAME:0x800870a4 ends 0x80087224,
// next CARDGAME:0x8008735c prologue at +0xdc: contiguous, no gap.
// Frame -0x30/+0x30; saves s6,s4,ra,s5,s3,s2,s1,s0.
// Semantics: n = lh(h+10); if (n <= 0) return; i = 0; pc = p; qc = q;
// hc = h; do { r = CARDGAME_F0x80085fd0(p, h+12, (int)hc[50]);
// s1[0x151] = (r == 0 && pc[0x46f] == 0); pc[0x446] = (r != 0 &&
// pc[0x46f] == 0); qc += 0x4c; pc++; hc++; } while (lh(h+10) > ++i).
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: 55 words
// word-equal to PAL @0x457c; single caller CARDGAME_F0x80087edc at 0x80088678
// (a0=s3, a1=s4, a2=s5); single callee CARDGAME_F0x80085fd0 at 0x80087280.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, -O2 -G0, variant
// o2-g0-no-strength-reduce (PAL keeps raw byte cursors with full-offset
// loads/stores; pair pinned by the matching sibling cardgame_f0x80085fd0).
// The guard compares the count against the counter, which is zero there, so
// the counter is referenced once before the loop. That eighth reference is
// what puts it in s2 and the halfword cursor in s3, as in PAL: see
// submissions/cardgame-8008722c/strategy-r6.md for the .lreg priorities.
// Revision history in that strategy and in the earlier strategy-r5.md.
// Status: C_MATCHING (220/220, 55/55 PAL words, zero differences).

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
