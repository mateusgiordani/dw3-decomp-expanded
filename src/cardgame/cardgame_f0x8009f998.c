/*
 * CARDGAME:0x8009f998 CARDGAME_F0x8009f998
 * 384 bytes at CARDGAME.PRO offset 0x1cce8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009f998
 *  Symbols     CARDGAME_F0x80083e34=0x80083e34
 *  Compare     384 bytes from 0x8009f998 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009f998
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * base 0x80082cb0, file-off 0x1cce8.
 *
 * a1 de entrada morre; o li 3 nasce no join. void: o jal deixa v0 sem andi.
 *
 * Deslocamento (<<) em vez de * emite addu com a base no primeiro operando.
 *
 * d gravado duas vezes perde o boost do sched1; nj = j + 1 e um pseudo novo e
 * fica depois do addu do stride. A cauda recomputa *200 num temporario novo
 * para o addu final sair com a base primeiro.
 */

extern char CARDGAME_F0x80083e34(int v, int op, int w);

void CARDGAME_F0x8009f998(unsigned char *p)
{
    int i;
    int n;
    int stride;
    int j;
    int step;
    unsigned char *q;
    unsigned char *r;
    unsigned char *rb;
    short h1;
    unsigned short u;
    int s4;
    short c;
    int t;

    for (i = 0; i < 10; i++) {
        if (*(signed char *)(p + i + 0x46f) != 0)
            break;
    }
    n = *(p + 0x579);
    stride = ((((n << 1) + n) << 3) + n) << 3;
    q = p + (i * 2 + stride);
    h1 = *(short *)(q + 0x600);
    s4 = *(short *)(p + (h1 << 1) + 0x50);
    *(p + (*(signed char *)(p + 0x575) << 3) + 0x584) = n;
    u = *(unsigned short *)(q + 0x600);
    *(unsigned short *)(p + (*(signed char *)(p + 0x575) << 3) + 0x580) = u;
    *(p + (*(signed char *)(p + 0x575) << 3) + 0x585) =
        CARDGAME_F0x80083e34(*(short *)(p + ((int)(short)u << 1) + 0x50), 3, 0);
    r = p + stride;
    c = *(short *)(r + 0x5a6);
    j = i;
    if (j >= c - 1)
        goto loop_end;
    step = stride;
    rb = r;
loop_start:
    {
        int d;
        int nj;
        unsigned short vsrc;
        d = j * 2;
        d = d + step;
        nj = j + 1;
        vsrc = *(unsigned short *)(p + (nj * 2 + step) + 0x600);
        *(short *)(p + d + 0x600) = vsrc;
        j = nj;
        if (j < *(short *)(rb + 0x5a6) - 1)
            goto loop_start;
    }
loop_end:
    t = ((((n << 1) + n) << 3) + n) << 3;
    *(unsigned short *)(p + t + 0x5a6) = *(unsigned short *)(p + t + 0x5a6) - 1;
    CARDGAME_F0x80083e34(s4, 0, 0);
}
