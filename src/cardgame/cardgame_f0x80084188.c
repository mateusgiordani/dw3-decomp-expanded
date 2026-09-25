/*
 * CARDGAME:0x80084188 CARDGAME_F0x80084188
 * 180 bytes at CARDGAME.PRO offset 0x14d8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80084188
 *  Symbols     (none)
 *  Compare     180 bytes from 0x80084188 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80084188
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * CARDGAME_F0x80084320 at 0x80085cdc) + callees (none; indirect jalr v0).
 *
 * No Ghidra state change.
 *
 * Semantics: 12-slot byte flags at p+0x46f[i] cleared each pass; for i>=6 call
 * the 0x820 callback as fn(p, 1, 1 << (i-6)); track strictly-greatest return in
 * best/best_idx, clearing the old best slot, storing i at p+0x440 (int) and
 * setting the new best slot to 1.
 *
 * Matching note: base-first pointer additions (*(p + i + 0x46f)) are required;
 * plain p[i + 0x46f] subscripts let gcc 2.8.1 emit addu index,base (3 words
 * differ at file-offs 0x150e/0x1542/0x1566) while all other codegen matches.
 */

typedef int (*cardgame_84188_fn_t)(unsigned char *, int, int);

void CARDGAME_F0x80084188(unsigned char *p)
{
    int best = 0;
    int best_idx = 6;
    int i = 0;
    int v;

    do {
        *(p + i + 0x46f) = 0;
        if (i >= 6) {
            v = ((cardgame_84188_fn_t)*(void **)(p + 0x820))(p, 1, 1 << (i - 6));
            if (best < v) {
                *(p + best_idx + 0x46f) = 0;
                *(int *)(p + 0x440) = i;
                *(p + i + 0x46f) = 1;
                best = v;
                best_idx = i;
            }
        }
        i++;
    } while (i < 12);
}
