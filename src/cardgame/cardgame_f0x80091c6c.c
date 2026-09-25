/*
 * CARDGAME:0x80091c6c CARDGAME_F0x80091c6c
 * 580 bytes at CARDGAME.PRO offset 0xefbc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80091c6c
 *  Symbols     D_8005CCB0=0x8005ccb0 D_800A5958=0x800a5958
 *  Compare     580 bytes from 0x80091c6c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80091c6c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL SHA-256 (580 B @ 0xefbc):
 * e539a944c39f5804ec9c8cd1d196f2660e4628acc91dc3f051aa57257b106f47
 *
 * Prologue 27bdffd0 addiu sp,-0x30 ; sw s2,0x20(sp) ; move s2,a0 ; move t2,a1
 * sw s3,0x24(sp) ; shift-chain kind*0x72 ((x*8-x)*8+x)*2 ; addiu v0,0x72c addu
 * t1,s2,v0 (p, kept in t1 whole function) ; sw ra/s1/s0 ; lbu t0,(t1)
 *
 * Per n-arm full table reads D_800A5958[D_8005CCB0].wX (idx reloaded at
 * 0x80091cc8/cf8/d30/d54: the idx value dies in address computation so each of
 * the 4 reads reloads; no hoisted idx variable). Per-arm u*41 prefix
 * (sll,addu,sll + addu), shared sll-8, sc in a3. n-test#2 bne a2,zero splits
 * tag loads (w1 @ +4 / w3 @ +0xc). switch-like dispatch on s[0x422]: ==2 first
 * (delay slti st<3), >=3 split, ==1, ==3, ==4; no-match edges jump to epilogue
 * with v0=0 from delay slots. st==2: card chain b*0x4c (immediate sll chain),
 * flag lbu, 5-arg indirect call via t+0xf08 (a0=t,a1=b moves; a2=0x14; a3=sc;
 * tag spilled), stores. st==1: 2-arg call via t+0xf1c with a0/a1 moves. st==3:
 * flag clear, *p+=1 via t1 reload. st==4: r=1. Return r.
 *
 * Next function CARDGAME:0x80091eb0 at +0x244 (contiguous).
 *
 * The b += 6 adjust is its own "if (n != 0)" ahead of the n-select (jump
 * threading gives PAL beq + j/addiu), cases are in PAL block order 1, 2, 3, and
 * r = 0 precedes b = *p (prologue s3 save order).
 *
 * Details: strategy-r9-o55/attempts-r9.
 *
 * 16-byte table entry at 0x800a5958: stride confirmed by 4 access sites (n==0
 * uses +0/+4, n!=0 uses +8/+12 of entry idx=[0x8005ccb0]).
 */

typedef struct {
    int w0;
    int w1;
    int w2;
    int w3;
} cg_tbl16_t;

extern cg_tbl16_t D_800A5958[];
extern int D_8005CCB0;

int CARDGAME_F0x80091c6c(unsigned char *s, unsigned char *t, int n) {
    unsigned char *p;
    unsigned int b;
    int tag;
    int sc;
    unsigned char st;
    int r;

    r = 0;
    p = s + (n * 0x72 + 0x72c);
    b = *p;

    if (n != 0)
        b = b + 6;
    if (n == 0)
        sc = D_800A5958[D_8005CCB0].w0 + *p * 0x2900;
    else
        sc = D_800A5958[D_8005CCB0].w2 + *p * 0x2900;
    if (n == 0) {
        tag = D_800A5958[D_8005CCB0].w1;
    } else {
        tag = D_800A5958[D_8005CCB0].w3;
    }
    st = s[0x422];
    switch (st) {
    case 1: {
        unsigned char *c = t + b * 0x4c;

        if (c[0x14a] == 1) {
            ((void (*)(unsigned char *, unsigned int))*(void **)(t + 0xf1c))(t, b);
            s[0x422] = 2;
        }
        break;
    }
    case 2: {
        unsigned char *c = t + b * 0x4c;

        if (c[0x14a] == 1) {
            ((void (*)(unsigned char *, unsigned int, int, int, int))*(void **)(t + 0xf08))(t, b, 0x14, sc, tag);
            *(short *)(c + 0x12e) = 1;
            s[0x422] = 3;
        }
        break;
    }
    case 3: {
        unsigned char *c = t + b * 0x4c;

        if (c[0x14a] == 1) {
            s[0x422] = 4;
            *(short *)(c + 0x12e) = 0;
            *p = *p + 1;
        }
        break;
    }
    case 4:
        r = 1;
        break;
    default:
        break;
    }
    return r;
}
