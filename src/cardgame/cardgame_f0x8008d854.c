/*
 * CARDGAME:0x8008d854 CARDGAME_F0x8008d854
 * 228 bytes at CARDGAME.PRO offset 0xaba4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008d854
 *  Symbols     func_8001ebf8=0x8001ebf8
 *  Compare     228 bytes from 0x8008d854 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008d854
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Next framed CARDGAME:0x8008d938 at +0xe4 (contiguous), prev word 27bd0038
 * (prev function epilogue addiu sp,+0x38); no overlap.
 *
 * No Ghidra state change.
 *
 * Callers: 2 direct jal (PAL bin scan for 0x0c023615 at file-off
 * 0x10648/0x10684, vaddr 0x800932f8/0x80093334): f(s2-struct, s3, 0-or-1, lh)
 * -> int (beqz-tested).
 *
 * Callees: jal EXE:0x8001ebf8 (same helper as CARDGAME:0x8009cf60 family;
 * 0x0c007afe occurs 31 times in cardgame.bin) plus 2 indirect jalr: one via
 * stack slot buf+0x2c, one via q+0xea0. Load-delay nops before each jalr;
 * addiu/move in delay slots.
 *
 * Matching-critical source shapes (all portable C, no register variables): e is
 * computed before the EXE call and parenthesized as p + (idx*200 + 0x59c) so
 * the shift chain ends addiu v0,0x59c / addu s3,s1,v0 (delay); p is reused for
 * the p+2h and e+d pointers (lh off s1, no extra saved reg); early exit is
 * nested if (d < 5) with int r = 0 so the beqz delay slot carries move s0,zero
 * and the shared tail returns move v0,s0; r = 1 sits after the cb1 call in
 * source and the scheduler hoists it to move a0,s4 / addiu s0,1 / lw
 * v0,0xea0(a0) order. qq reuses q for the load and the call.
 *
 * Semantic: EXE helper fills stack buffer at sp+0x10; e = p + idx*200 + 0x59c;
 * x = *(short *)(p + h*2 + 0x50); cb0 = *(buf+0x2c); cb0(x + 1); b = **buf
 * (byte); d = b - 1; if (d >= 5, signed) return 0; s = e + d; saturating
 * increment of s[0xc] at 99 (0x63, unsigned compare); cb1 = *(q+0xea0); cb1(q,
 * idx, d, s[0xc]); return 1.
 *
 * Buffer void *buf[22] (88 B: vars=88 + outgoing 16 + 6 saved regs 24 -> frame
 * 0x80): only words 0 (result pointer) and 11 (callback) are read here; the
 * rest is EXE-helper-filled and untouched, so no struct is invented.
 *
 * Conservative names: func_8001ebf8 (EXE helper, semantics out of scope), no
 * globals, no overlay table, no invented struct. lh is signed (short), lbu is
 * unsigned, saturation compare is unsigned (sltiu), range check is signed
 * (slti).
 */

extern void func_8001ebf8(void *buf);

typedef void (*cardgame_8008d854_cb0_t)(int x);
typedef void (*cardgame_8008d854_cb1_t)(void *q, int idx, int d, int cnt);

int CARDGAME_F0x8008d854(unsigned char *p, void *q, int idx, int h)
{
    void *buf[22];
    unsigned char *e;
    short x;
    unsigned char b;
    int d;
    unsigned int cnt;
    int r = 0;
    void *qq;
    cardgame_8008d854_cb0_t cb0;
    cardgame_8008d854_cb1_t cb1;

    e = p + (idx * 200 + 0x59c);
    func_8001ebf8(buf);
    p = p + h * 2;
    x = *(short *)(p + 0x50);
    cb0 = *(cardgame_8008d854_cb0_t *)((char *)buf + 0x2c);
    cb0(x + 1);
    b = *(unsigned char *)*(void **)buf;
    d = b - 1;
    if (d < 5) {
        p = e + d;
        cnt = p[0xc];
        if (cnt < 0x63)
            p[0xc] = (unsigned char)(cnt + 1);
        qq = q;
        cb1 = *(cardgame_8008d854_cb1_t *)((char *)qq + 0xea0);
        cb1(qq, idx, d, p[0xc]);
        r = 1;
    }
    return r;
}
