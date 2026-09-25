/* CARDGAME:0x8008735c (size 252, 0xfc) */
/* PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x46ac (RAW, no header) */
/* Boundary: prologue addiu sp,-0x80 at 0x8008735c, epilogue jr ra / addiu sp,+0x80 at 0x80087450/54; */
/* next function CARDGAME:0x80087458 prologue addiu sp,-0x28 at +0xfc, contiguous, no inner prologue. */
/* Frame -0x80/+0x80; saves s1/s4/s2/s3/s0/ra at 0x6c/0x78/0x70/0x74/0x68/0x7c; 5th arg (a4) via 0x90(sp). */
/* Body: EXE helper func_8001ebf8(sp+0x10); fp at buf+0x2c is called with the signed halfword */
/* loaded at a0 + a4*2 + 0x50, plus 1; if (*b < 6) adjust a2[*b-1+0xc] by +-b[5] on a3 flag, */
/* then indirect (*(a1+0xea0))(a1, a2[0x11], *b-1, updated). Callers: 0x800874ac, 0x8008753c, 0x8008c688. */
/* Ghidra program CARDGAME read-only (never import); cardgame.s GUIDE only. */
/* Match: portable C on pinned base psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 -mips1 -msoft-float. */
/* Exact 252-byte result: docs/c-matching-guide/submissions/cardgame-8008735c/manifest-r5.json. */
/* Branch-local p/v/d/i keep the merged sb before the callback argument move. Reading b[5] */
/* after p[0x0c] gives b a conflict with $3 in native .greg, selecting PAL's $4 naturally. */

extern void func_8001ebf8(void *buf);

typedef struct {
    unsigned char *p;      /* +0x00 */
    unsigned char pad[40]; /* +0x04 */
    void (*fp)(int);       /* +0x2C -> sp+0x3C */
    unsigned char rest[36];/* +0x30 (total 0x54) */
} Tmp8008735c;

typedef void (*CardCb8008735c)(void *a0, int a1, int a2, int a3);

void CARDGAME_F0x8008735c(short *a0, void *a1, unsigned char *a2, int a3, int a4)
{
    Tmp8008735c tmp;
    unsigned char *b;
    unsigned char *b2;
    unsigned char *p2;
    int j;
    CardCb8008735c q2;

    func_8001ebf8(&tmp);
    a0 = (short *)((unsigned char *)a0 + a4 * 2);
    tmp.fp((int)(*(short *)((unsigned char *)a0 + 0x50)) + 1);
    b = tmp.p;
    if (*b < 6) {
        if (a3 != 0) {
            unsigned char *p;
            unsigned char v;
            unsigned char d;
            int i;
            i = *b - 1;
            p = a2 + i;
            v = p[0x0c];
            d = b[5];
            v = (unsigned char)(v + d);
            p[0x0c] = v;
        } else {
            unsigned char *p;
            unsigned char v;
            unsigned char d;
            int i;
            i = *b - 1;
            p = a2 + i;
            v = p[0x0c];
            d = b[5];
            v = (unsigned char)(v - d);
            p[0x0c] = v;
        }
        b2 = tmp.p;
        j = *b2 - 1;
        p2 = a2 + j;
        q2 = *(CardCb8008735c *)((unsigned char *)a1 + 0xea0);
        q2(a1, a2[0x11], j, p2[0x0c]);
    }
}
