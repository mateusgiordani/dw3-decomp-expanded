/*
 * CARDGAME:0x80086eb0 CARDGAME_F0x80086eb0
 * 116 bytes at CARDGAME.PRO offset 0x4200 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80086eb0
 *  Symbols     CARDGAME_F0x80086a18=0x80086a18
 *  Compare     116 bytes from 0x80086eb0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80086eb0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL words decoded directly (file off = vaddr - base).
 *
 * Signature: 4 args (a0 pointer s0, a1 unused, a2 signed count, a3 byte s1).
 *
 * a1 ($5) is never read; a2 drives blez; a3 is preserved across the jal in s1.
 *
 * Semantics: if (a2 <= 0) clear one byte at +0x49E, else clear 40 bytes at
 * +0x49E..+0x4C5 descending (i 39..0, pointer sliding); then call the
 * 0x80086a18 worker on a0 and set +0x423=1, +0x499=a3, +0x43C=0, +0x440=-1.
 *
 * The explicit goto pins blez/j/nop/single order: plain if/else lets gcc 2.8.1
 * invert to bgtz with the single sb folded into j's delay slot (112 B object);
 * the goto form keeps fallthrough loop + j/nop + single (116 B, exact).
 */

extern void CARDGAME_F0x80086a18(unsigned char *a0);

void CARDGAME_F0x80086eb0(unsigned char *a0, int a1, int a2, unsigned char a3)
{
    (void)a1;
    if (a2 <= 0)
        goto single;
    {
        int i = 39;
        unsigned char *p = a0 + 39;
        do {
            p[0x49E] = 0;
            i--;
            p--;
        } while (i >= 0);
    }
    goto cont;
single:
    a0[0x49E] = 0;
cont:
    CARDGAME_F0x80086a18(a0);
    a0[0x423] = 1;
    a0[0x499] = a3;
    ((int *)(a0 + 0x43C))[0] = 0;
    ((int *)(a0 + 0x440))[0] = -1;
}
