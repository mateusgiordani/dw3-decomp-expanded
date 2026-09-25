// CARDGAME:0x8008e398 (size 536, 0x218) -- portable C, exact_byte_match
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xb6e8.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, variant base
// (-O2 -G0), --rodata 0x8008351c for the 8-entry switch table (cases 1..8).
// Caller: CARDGAME_F0x80084320 via jal at 0x80085814 (return ignored).
// Next function CARDGAME:0x8008e5b0 at +0x218; no overlap.
//
// Matching-critical shapes (evidence: RTL -da dumps, see
// docs/c-matching-guide/submissions/cardgame-8008e398/strategy-r9-o55.md):
// - The loop is written over the index i. PAL's s16 (p+i, stores at +0x46f),
//   s21 (p+i*14, loads at +0x72e) and s22 ((i-6)*14) are strength-reduced
//   givs created by loop.c, so their inits land after the hoisted literal 1
//   (s19) and their increments follow i++ -- source pointer bivs cannot
//   reproduce that prologue/tail order or keep the 0x46f/0x72e offsets.
// - off is assigned in the second half so it stays a register giv added to
//   p (addu p,off; lh 0x7a0); folding it into the address merges it with
//   the first-half giv (524 B).
// - s = p + sel*8 is computed as a pointer value (addu p,t operand order)
//   and re-assigned before the shared sw load, so the per-iteration sll/addu
//   is not a desirable loop movable (sel stays live in fp).

typedef int (*cardgame_8008e398_cb_t)(void *q, int v);

void CARDGAME_F0x8008e398(unsigned char *p, void *q)
{
    int i;
    int sel;
    unsigned char *s;
    int c;
    int v;
    unsigned char sw;
    cardgame_8008e398_cb_t cb;
    int off;

    sel = *(signed char *)(p + 0x575) - 1;
    s = p + sel * 8;
    c = s[0x584];
    i = 0;
    do {
        p[i + 0x46f] = 0;
        if (i < 6) {
            if (p[0x72c] <= i)
                goto next;
            v = (int)*(short *)(p + i * 14 + 0x72e);
        } else {
            if (i - 6 >= p[0x79e])
                goto next;
            off = (i - 6) * 14;
            v = (int)*(short *)(p + off + 0x7a0);
        }
        s = p + sel * 8;
        sw = s[0x585];
        switch (sw) {
        case 1:
            if (c == 0) {
                if (i < 6)
                    p[i + 0x46f] = 1;
            } else if (i >= 6)
                p[i + 0x46f] = 1;
            break;
        case 2:
            if (c == 0) {
                if (i >= 6)
                    p[i + 0x46f] = 1;
            } else if (i < 6)
                p[i + 0x46f] = 1;
            break;
        case 3:
            p[i + 0x46f] = 1;
            break;
        case 4:
            cb = *(cardgame_8008e398_cb_t *)((char *)q + 0xf40);
            if (cb(q, v) != 1)
                p[i + 0x46f] = 1;
            break;
        case 5:
            cb = *(cardgame_8008e398_cb_t *)((char *)q + 0xf40);
            if (cb(q, v) != 2)
                p[i + 0x46f] = 1;
            break;
        case 6:
            cb = *(cardgame_8008e398_cb_t *)((char *)q + 0xf40);
            if (cb(q, v) == 3)
                p[i + 0x46f] = 1;
            break;
        case 7:
            cb = *(cardgame_8008e398_cb_t *)((char *)q + 0xf40);
            if (cb(q, v) != 4)
                p[i + 0x46f] = 1;
            break;
        case 8:
            cb = *(cardgame_8008e398_cb_t *)((char *)q + 0xf40);
            if (cb(q, v) == 6)
                p[i + 0x46f] = 1;
            break;
        default:
            break;
        }
next:
        i++;
    } while (i < 12);
}
