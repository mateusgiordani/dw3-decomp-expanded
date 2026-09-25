/*
 * CARDGAME:0x8009d6e0 CARDGAME_F0x8009d6e0
 * 540 bytes at CARDGAME.PRO offset 0x1aa30 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009d6e0, jump table (.rodata) at 0x800837a0
 *  Symbols     D0x800a5c2c=0x800a5c2c DAT_80044f4c=0x80044f4c
 *  Compare     540 bytes from 0x8009d6e0 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009d6e0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL body 540 bytes; jump table 28 bytes at 0x800837a0.
 *
 * Framed function: prologue addiu sp,-0x18 ; sw s0,0x10(sp) ; move s0,a0 ; lui
 * v0,0x8004 ; lw v0,0x4f4c(v0) ; sw ra,0x14(sp) ; jalr v0 ; li a0,0x7a4
 * (delay).
 *
 * Epilogue lw ra,0x14(sp) ; lw s0,0x10(sp) ; jr ra ; addiu sp,+0x18.
 *
 * Prev function ends jr ra/addiu sp,+0x18 at 0x8009d6d8/0x8009d6dc (contiguous,
 * no overlap); next function starts 0x8009d8fc (move t4,a0, frameless).
 *
 * No Ghidra state change.
 *
 * Caller: CARDGAME FUN_800a2f00+0x8c jal 0x8009d6e0 (a0=s2 struct, a1=s1 dead:
 * a1 is cleared before first use, so the function takes one param).
 *
 * Callees: one indirect jalr via EXE slot DAT_80044f4c (allocator idiom, same
 * as sibling CARDGAME:0x800973e4) with size 0x7a4; result kept at s+0x2f0.
 *
 * Data: jump table at 0x800837a0 (7 words, bin-verified
 * 0x8009d7b4..0x8009d7e4), halfword cost table at 0x800a5c2c (bin-verified).
 *
 * Record array: rec = alloc + idx*208 - 208 (idx = s[0x2e8], shift chain
 * sll1/addu/sll2/addu/sll4 then addiu -0xd0 then addu); 40 entries of 4 B
 * (h0:2, b2:1, b3:1). Per-slot s layout: [0x30a+i*2]=i, [0x30b+i*2]=b2,
 * [0x35c+i*4]=b3, [0x35d+i*4]=(h0>>15)&1, [0x35e+i*4]=i+K(b3) via jump table
 * (K: 1->0x190, 2->0x12c, 3->0x1f4, 4->0xc8, 5->0x258, 6->0x64, 7->0x2bc; b3==0
 * or >7 stores nothing). Loop2 scans 27 halfwords at rec+0xa0: zero halfword
 * writes the 15-byte default row at s+k and exits; else
 * s[0x400+k]=(byte)(half-1).
 *
 * Exact with pinned PsyQ GCC 2.8.1 / ASPSX 2.79, O2 base (strength reduction
 * on).
 *
 * Indexed loop-1 accesses let the compiler initialize its derived cursors after
 * the jump-table address. In loop 2, k remains live through the zero-byte
 * address; its transition from row index to byte offset preserves PAL's
 * temporary copy.
 *
 * See the r10 strategy for controls and native RTL evidence.
 */

#include "common/types.h"

extern int32_t (*DAT_80044f4c)(int32_t arg);
extern int16_t D0x800a5c2c[];

void CARDGAME_F0x8009d6e0(unsigned char *s)
{
    int32_t base;
    unsigned int idx;
    int off;
    unsigned char *rec;
    unsigned char *q2;
    unsigned char *p;
    unsigned char *w;
    int i;
    int k;
    unsigned int v;
    int t;
    int hs;
    unsigned int u;
    unsigned int h;

    base = DAT_80044f4c(0x7a4);
    i = 0;
    idx = s[0x2e8];
    *(int32_t *)(s + 0x2f0) = base;
    off = (int)idx * 208 - 208;
    rec = (unsigned char *)(base + off);
    s[0x2e9] = rec[0xc8];
    *(int32_t *)(s + 0x2ec) = D0x800a5c2c[*(int32_t *)(rec + 0xcc)];
    do {
        s[i * 2 + 0x30a] = (unsigned char)i;
        s[i * 2 + 0x30b] = rec[i * 4 + 2];
        s[i * 4 + 0x35c] = rec[i * 4 + 3];
        hs = *(int16_t *)(rec + i * 4);
        u = (unsigned int)hs;
        s[i * 4 + 0x35d] = (unsigned char)((u >> 15) & 1);
        v = s[i * 4 + 0x35c] - 1;
        switch (v) {
        case 0:
            t = i + 0x190;
            break;
        case 1:
            t = i + 0x12c;
            break;
        case 2:
            t = i + 0x1f4;
            break;
        case 3:
            t = i + 0xc8;
            break;
        case 4:
            t = i + 0x258;
            break;
        case 5:
            t = i + 0x64;
            break;
        case 6:
            t = i + 0x2bc;
            break;
        default:
            goto next;
        }
        *(int16_t *)(s + i * 4 + 0x35e) = (int16_t)t;
next:
        i++;
    } while (i < 0x28);
    k = 0;
    p = s;
    q2 = rec;
top2:
    if (*(int16_t *)(q2 + 0xa0) != 0)
        goto nz2;
    p[0x400] = 7;
    w = (unsigned char *)(k + (int)s);
    w[0x401] = 8;
    w[0x402] = 0x18;
    w[0x403] = 0x1f;
    w[0x404] = 0x20;
    /* The scan index is no longer needed on this exit; reuse it as a byte offset. */
    k += 0x405;
    *(unsigned char *)(k + (int)s) = 0;
    w[0x406] = 6;
    w[0x407] = 0xc;
    w[0x408] = 0x12;
    w[0x409] = 0x1b;
    w[0x40a] = 0x1e;
    w[0x40b] = 0x21;
    w[0x40c] = 0x25;
    w[0x40d] = 0x26;
    w[0x40e] = 0xff;
    goto done2;
nz2:
    p[0x400] = (unsigned char)(q2[0xa0] - 1);
    q2 += 2;
    k++;
    p++;
    if (k < 0x1b)
        goto top2;
done2:
    ;
    k = 0;
    q2 = rec;
    p = s;
    do {
        h = *(uint16_t *)q2;
        q2 += 4;
        k++;
        *(uint16_t *)(p + 0x248) = (uint16_t)((h & 0xfff) - 1);
        p += 2;
    } while (k < 0x28);
}
