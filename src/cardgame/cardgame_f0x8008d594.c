/* CARDGAME:0x8008d594 (size 704, 0x2c0) */
/* PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xa8e4. */
/* Boundary: sweep reports/handoffs/cardgame-boundary-sweep.md #50 (HIGH): */
/* prologue 27bdffc8 addiu sp,-0x38, epilogue jr ra / addiu sp,+0x38 (frame */
/* -0x38/+0x38); next framed CARDGAME:0x8008d854 at +0x2c0 (contiguous). */
/* Ghidra program CARDGAME (project ddw3-pal-sles-03936, min_address 0x80082cb0 */
/* = verified base, MIPS LE), read-only disasm/decompile/xref/caller+callee */
/* graphs; PAL chunk sha256 b8d3650491365236afdbad1ca4d1c82b64d8b09754c6d6b55a4490107a371271; */
/* cardgame.s is GUIDE only (no 0x8008d594 label), never copied. No state change. */
/* Caller: 1 direct jal at 0x80085cf4 (CARDGAME_F0x80084320, word 0x0c023565): */
/* move a0,s1 / jal / move a1,s0 (delay); return beq-tested vs -1 else stored */
/* as byte. Signature int f(void *, void *), returns -1/0/1. Callees: 9 indirect */
/* jalr (no direct jal): 2x *(EXE 0x8004BBC4) as f(0)->int, 2x *(EXE 0x8004BBD8) */
/* as f(0,0xd)/(0,0xe)->int, 2x *(0x80055C48) as f(0x4001c)/f(0x800450bd), plus */
/* q-table calls +0xec4(q), +0xf24(q,0xf,4,0,0x1000: 4 regs + 1 stack word at */
/* 0x10(sp)), +0xf18(q,0xf), +0xeb4(q). Load-delay nops before each jalr. */
/* Semantics: state machine on p[0x422]. st==0 or >3: return -1. st==1: if */
/* *(short *)(q+0x64)==2 and p[0x498]==0, set p[0x422]=2 and set bit0 of */
/* q[i*0x4c+0x150] for i in 0..11 with (signed)p[i+0x46f]!=0; return -1. st==2: */
/* t=f0(0), u=f1(0,0xd); if ((t>>u)&1) and *(int *)(p+0x438)!=0: f2(0x4001c), */
/* clear bit0 of the 12 bytes, return 1. Else t=f0(0), u=f1(0,0xe); if */
/* ((t>>u)&1): f2(0x800450bd), cb_ec4(q), *(p+0x440)=1, p[0x422]=3, */
/* cb_f24(q,0xf,4,0,0x1000), p[0x499]=2; return -1. st==3: if *(short *)(q+0x64) */
/* ==0 and p[0x498]==0: cb_f18(q,0xf), r=0, cb_eb4(q), clear bit0 of the 12 */
/* bytes; return 0. Else return -1. r lives in s5, st in s3, p in s4, q in s2. */
/* Control flow mirrors the ASM layout with gotos (still plain ISO C): */
/* beq-forward st2 dispatch, slti <3 chain, st3-test between dispatch and the */
/* st1 body, twin joins done2/done, r=1/0 set before their loops so the -O2 */
/* scheduler can hoist them into jalr delay slots, int st (no andi), int x */
/* (single lh reused for the sb). b[0xfd]/b[0x102] address the two EXE words */
/* via the same CSE'd base the ASM uses (0x8004B7D0+0x3f4==0x8004BBC4, */
/* +0x408==0x8004BBD8); (t>>u)&1 keeps the srav shape (srav masks in hw). */

typedef int (*cardgame_d594_f0_t)(void *a);
typedef int (*cardgame_d594_f1_t)(void *a, int b);
typedef void (*cardgame_d594_f2_t)(void *a);
typedef int (*cardgame_d594_c1_t)(void *a);
typedef int (*cardgame_d594_c2_t)(void *a, int b);
typedef int (*cardgame_d594_c5_t)(void *a, int b, int c, int d, int e);

extern unsigned int D_80055C48;
extern unsigned int D_8004B7D0;

int CARDGAME_F0x8008d594(unsigned char *p, unsigned char *q)
{
    unsigned int *b;
    int r = -1;
    int st = p[0x422];
    int x;
    int t;
    int i;
    unsigned char *qq;

    if (st == 2)
        goto st2;
    if (st < 3) {
        if (st == 1)
            goto st1;
        goto done;
    }
st3chk:
    if (st == 3)
        goto st3;
    goto done;
st1:
    x = *(short *)(q + 0x64);
    if (x != 2)
        goto done;
    if (p[0x498] != 0)
        goto done;
    p[0x422] = x;
    for (i = 0, qq = q; i < 12; i++, qq += 0x4c) {
        if (*(signed char *)(p + i + 0x46f) != 0)
            qq[0x150] |= 1;
    }
    goto done;

st2:
    b = &D_8004B7D0;
    t = ((cardgame_d594_f0_t)b[0xfd])(0) >> ((cardgame_d594_f1_t)b[0x102])(0, 0xd);
    t &= 1;
    if (t == 0)
        goto st2b;
    if (*(int *)(p + 0x438) == 0)
        goto done2;
    ((cardgame_d594_f2_t)D_80055C48)((void *)0x4001c);
    r = 1;
    i = 0;
    qq = q;
    do {
        if (*(signed char *)(p + i + 0x46f) != 0)
            qq[0x150] &= 0xfe;
        i++;
        qq += 0x4c;
    } while (i < 12);
    goto done;

st2b:
    t = ((cardgame_d594_f0_t)b[0xfd])(0) >> ((cardgame_d594_f1_t)b[0x102])(0, 0xe);
    t &= 1;
    if (t == 0)
        goto done2;
    ((cardgame_d594_f2_t)D_80055C48)((void *)0x800450bd);
    ((cardgame_d594_c1_t)*(void **)(q + 0xec4))(q);
    *(int *)(p + 0x440) = 1;
    p[0x422] = 3;
    ((cardgame_d594_c5_t)*(void **)(q + 0xf24))(q, 0xf, 4, 0, 0x1000);
    p[0x499] = (unsigned char)st;
    goto done2;

st3:
    if (*(short *)(q + 0x64) != 0)
        goto done;
    if (p[0x498] != 0)
        goto done;
    ((cardgame_d594_c2_t)*(void **)(q + 0xf18))(q, 0xf);
    r = 0;
    ((cardgame_d594_c1_t)*(void **)(q + 0xeb4))(q);
    i = r;
    qq = q;
    do {
        if (*(signed char *)(p + i + 0x46f) != 0)
            qq[0x150] &= 0xfe;
        i++;
        qq += 0x4c;
    } while (i < 12);
    goto done2;

done2:
done:
    return r;
}
