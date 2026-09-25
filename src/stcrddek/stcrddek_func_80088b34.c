/*
 * STCRDDEK:0x80088b34 STCRDDEK_func_80088b34
 * 496 bytes at STCRDDEK.PRO offset 0x5e84 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80088b34
 *  Symbols     FUN_8001ae38=0x8001ae38 FUN_8001b364=0x8001b364
 *  Compare     496 bytes from 0x80088b34 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80088b34
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue addiu sp,-0x38 with saves a1/0x3c, s3/s8/ra/s7/s6/s5/s4/s2/s1/s0,
 * a0/0x38; epilogue restores + jr ra.
 *
 * Body: outer 3x (per-row header + inner 6x), phase2 2x, phase3 alloc + virtual
 * dispatch loop.
 *
 * Caller: UNCONDITIONAL_CALL from STCRDDEK_func_80089e98 at 0x80089f30 (jal,
 * args arg0=s0, arg1=s1).
 *
 * Callees: 4x jal 0x8001ae38, 1x jal 0x8001b364, 2x indirect jalr (+0x80 with
 * (ptr,0), +0x15c with (obj,mem54-2)).
 *
 * The do/while(0) inside the goto inner loop only adds loop depth to flow's
 * reference weights (h: 4 -> 5 refs, priority above row); loop.c can hoist
 * nothing out of the goto loop.
 *
 * The empty do/while(0) after phase 2 becomes mid-block behind combine's (use)
 * from the known-zero guard, making the a1 load of FUN_8001b364 a sched
 * barrier.
 */

extern int FUN_8001ae38(int a0, int a1, int a2, int a3);
extern int FUN_8001b364(int a0, int a1, int a2, int a3);
void STCRDDEK_func_80088b34(int arg0, int *arg1)
{
    int outer;
    int off;
    int hiA;
    int hiB;
    int *row;
    int inner;
    int cur;
    int h;
    int p;
    int k;
    int dy = 0;
    int v0;
    int y;
    int *q;
    v0 = FUN_8001ae38(*(short *)(arg0 + 0x50), k = 1, 0x97, 0x22);
    outer = 0;
    off = outer;
    row = arg1;
    row[1] = v0;
    hiA = 0x640000;
    hiB = 0x530000;
    do {
        row[2] = FUN_8001ae38(*(short *)(arg0 + 0x50), k = 1, 0x1c, hiB >> 16);
        inner = 0;
        h = hiA;
        p = off;
        cur = 0x3b0000;
    inl:
        do {
            int v = FUN_8001ae38(*(short *)(arg0 + 0x50), k = 1, cur >> 16, h >> 16);
            *(int *)((char *)arg1 + p + 0xc) = v;
            p += 4;
            inner += k;
            cur += 0x230000;
        } while (0);
        if (inner < 6)
            goto inl;
        off += 0x1c;
        dy = 0x2d0000;
        hiA += dy;
        hiB += dy;
        outer++;
        row += 7;
    } while (outer < 3);
    outer = 0;
    q = arg1;
    y = 0x230000;
ph2:
    {
        q[0x17] = FUN_8001ae38(*(short *)(arg0 + 0x50), k = 1, 0xa7, y >> 16);
        outer += k;
        q++;
        y += 0xe0000;
    }
    if (outer < 2)
        goto ph2;
    do {
    } while (0);
    {
        int *s;
        int v;
        v = FUN_8001b364(*(short *)(arg0 + 0x50), *(int *)(arg0 + 0x54) - 2, 0x9a, 0x23);
        outer = 0;
        arg1[0x19] = v;
        (*(void (*)(int, int))(*(int *)(v + 0x80)))(v, outer);
        s = arg1 + 1;
        if (outer < *(int *)(arg0 + 0x20) - 3) {
            do {
                int obj = *s;
                (*(void (*)(int, int))(*(int *)(obj + 0x15c)))(obj, *(int *)(arg0 + 0x54) - 2);
                outer++;
                s++;
            } while (outer < *(int *)(arg0 + 0x20) - 3);
        }
    }
}
