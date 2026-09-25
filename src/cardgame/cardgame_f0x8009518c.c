/*
 * CARDGAME:0x8009518c CARDGAME_F0x8009518c
 * 124 bytes at CARDGAME.PRO offset 0x124dc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009518c
 *  Symbols     (none)
 *  Compare     124 bytes from 0x8009518c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009518c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * PAL bytes authoritative.
 *
 * Match recipe (one hypothesis per change, all base
 * psyq-gcc-2.8.1-sn32-4.0.0010/aspsx-2.79 -O2 -G0): H1 goto-funnel (47->46
 * diffs): two forward gotos give PAL beq/beq layout targets; dual-view count
 * loads (lb index temp c, hoisted lbu decrement source d) reproduce the
 * preloaded lbu v1. H2 base-first address temporary (46->44): tab=(u8*)ctx+off
 * then tab[0x584] emits PAL addu $2,$16,$2 (base-first) instead of index-first;
 * first 44 bytes exact. H3 PAL-layout block order (44->10): call body, then
 * ret2, then shared keeps branch2 as beq a2,0->ret2 with nop delay and puts li
 * ret in the j delay slot. H4 drop ret temp, duplicated constant stores (10->0,
 * EXACT): call path stores 2 to ctx[0x499] then 1 to ctx[0x422] and returns;
 * ret2 stores 2 to ctx[0x422]. GCC cross-jumps the common sb into a single
 * shared store with j-over-li (delay li v0,1), nop in jalr delay, everything in
 * $2.
 *
 * Semantics (conservative, uncertainties noted): - ctx (a0, saved to s0) is a
 * byte-addressed context; 0x575 is a small count, signed for the index calc
 * (PAL lb) and unsigned for the decrement source (PAL lbu) -- both views kept.
 * Table at ctx+0x584 indexed as ((count-1)<<3), i.e. 8-byte entries, flag byte
 * at entry offset 0 (struct layout UNKNOWN -- stride/field roles unconfirmed).
 * - If table flag != 0 and a2 == 0: store 2 to ctx[0x422] and skip the indirect
 * call. Otherwise: ctx[0x575]--, call *(void(**)(void*))((char*)a1+0xEC4) with
 * a1, store 2 to ctx[0x499], store 1 to ctx[0x422]. - Declared void because
 * both known callers ignore v0; PAL leaves 1/2 in v0 as scratch. Whether the
 * original returned int (1/2) is UNCERTAIN (not tested: void is exact). -
 * Indirect callee arity UNCERTAIN: at least (void*) with a0=a1; a1/a2/a3 regs
 * pass through. - Field roles for 0x422/0x499/0x575/0x584/0xEC4 are offsets
 * only, no struct invented.
 */

void CARDGAME_F0x8009518c(signed char *ctx, void *a1, int a2) {
    void (*fn)(void *);
    int c = ctx[0x575];
    int off = (c - 1) << 3;
    unsigned char *tab = (unsigned char *)ctx + off;
    int flag = tab[0x584];
    unsigned char d = ((unsigned char *)ctx)[0x575];
    if (flag == 0)
        goto call;
    if (a2 == 0)
        goto ret2;
call:
    ctx[0x575] = d - 1;
    fn = *(void (**)(void *))((char *)a1 + 0xEC4);
    fn(a1);
    ctx[0x499] = 2;
    ctx[0x422] = 1;
    return;
ret2:
    ctx[0x422] = 2;
}
