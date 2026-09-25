/*
 * CARDGAME:0x80091858 CARDGAME_F0x80091858
 * 420 bytes at CARDGAME.PRO offset 0xeba8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80091858
 *  Symbols     CARDGAME_F0x8009170c=0x8009170c CARDGAME_F0x80091858=0x80091858
 *              func_8001ebf8=0x8001ebf8
 *  Compare     420 bytes from 0x80091858 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80091858
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * at base 0x80082cb0. The 88-byte local holds a data pointer at +0 and callback
 * at +0x2c. Reusing the halfword temporary for data[1] preserves the PAL
 * load/store allocation. Exact with PsyQ GCC 2.8.1 / ASPSX 2.79, O2/G0 base.
 */

extern void func_8001ebf8(void *buf);
extern void CARDGAME_F0x8009170c(void *a0, void *a1, int a2, int a3);

typedef void (*cardgame_91858_cb0_t)(int x);
typedef void (*cardgame_91858_cb4_t)(void *q, int y, int d, int e);
typedef void (*cardgame_91858_cb3_t)(void *q, int y, int h);
typedef void (*cardgame_91858_cb5_t)(void *q, int y, int d, int e, int f);

typedef struct {
    unsigned char *data;
    unsigned int unknown04[10];
    cardgame_91858_cb0_t cb2c;
    unsigned int unknown30[10];
} CardBuffer91858;

void CARDGAME_F0x80091858(unsigned char *p1, void *p2, int p3, int p4)
{
    CardBuffer91858 buf;
    unsigned char *e;
    unsigned char *row;
    unsigned char *t;
    unsigned int x;
    unsigned int y;
    short h;
    int off;
    int b;

    e = p1 + (p3 * 0x72 + 0x72c);
    x = *e;
    y = x;
    if (p3 != 0)
        y = x + 6;
    func_8001ebf8(&buf);
    off = p4 << 1;
    h = *(short *)((char *)p1 + off + 0x50);
    buf.cb2c(h + 1);
    h = buf.data[1];
    row = e + x * 14;
    *(short *)(row + 8) = h;
    b = buf.data[2];
    *(short *)(row + 4) = 0;
    *(short *)(row + 6) = 0;
    *(short *)(row + 2) = (short)p4;
    row[0xd] = (unsigned char)p3;
    row[0xc] = 2;
    *(short *)(row + 10) = b;
    b = p1[0x308];
    p1[0x308] = (unsigned char)(b + 1);
    row[0xe] = b;
    ((cardgame_91858_cb4_t)*(void **)((char *)p2 + 0xf14))(p2, y, 0xe500, 0x6100);
    ((cardgame_91858_cb3_t)*(void **)((char *)p2 + 0xf3c))(p2, y, p4);
    t = (unsigned char *)p2 + y * 76;
    t[0x14b] = row[8];
    b = row[10];
    *(short *)(t + 0x120) = 0;
    t[0x14c] = b;
    CARDGAME_F0x8009170c(p1, p2, p3, y);
    p1[0x422] = 1;
    ((cardgame_91858_cb5_t)*(void **)((char *)p2 + 0xf24))(p2, y, 0x14, 0x1000, 0x1000);
}
