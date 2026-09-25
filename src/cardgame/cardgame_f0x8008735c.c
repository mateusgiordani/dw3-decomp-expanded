/*
 * CARDGAME:0x8008735c CARDGAME_F0x8008735c
 * 252 bytes at CARDGAME.PRO offset 0x46ac (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008735c
 *  Symbols     func_8001ebf8=0x8001ebf8
 *  Compare     252 bytes from 0x8008735c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008735c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Frame -0x80/+0x80; saves s1/s4/s2/s3/s0/ra at 0x6c/0x78/0x70/0x74/0x68/0x7c;
 * 5th arg (a4) via 0x90(sp).
 *
 * Body: EXE helper func_8001ebf8(sp+0x10); fp at buf+0x2c is called with the
 * signed halfword loaded at a0 + a4*2 + 0x50, plus 1; if (*b < 6) adjust
 * a2[*b-1+0xc] by +-b[5] on a3 flag, then indirect (*(a1+0xea0))(a1, a2[0x11],
 * *b-1, updated). Callers: 0x800874ac, 0x8008753c, 0x8008c688.
 *
 * Branch-local p/v/d/i keep the merged sb before the callback argument move.
 * Reading b[5] after p[0x0c] gives b a conflict with $3 in native .greg,
 * selecting PAL's $4 naturally.
 */

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
