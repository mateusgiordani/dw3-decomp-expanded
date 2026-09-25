/*
 * STCRDDEK:0x8008a21c STCRDDEK_func_8008a21c
 * 188 bytes at STCRDDEK.PRO offset 0x756c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008a21c
 *  Symbols     stcrddek_tbl_80044B38=0x80044b38
 *              stcrddek_word_8005CCA8=0x8005cca8
 *  Compare     188 bytes from 0x8008a21c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x8008a21c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Body: probe EXE helper through table word EXE 0x80044F3C (kept as base
 * 0x80044B38 + field 0x404 in s1, reloaded before every indirect jalr v0) with
 * base value from EXE global 0x8005CCA8 (hi 0x8006 kept in s0, value reloaded
 * before every call). Returns 1 on first nonzero probe; else result of
 * probe(0x771) != 0. Delay-slot li v0,1 after each bne is dead on the
 * fall-through path (overwritten by next call result).
 */

typedef struct {
    unsigned char pad[0x404];
    int (*fn)(int);
} stcrddek_tbl_80044B38_t;

/* EXE-resident objects (resolved via pipeline --symbol, not committed): */
extern stcrddek_tbl_80044B38_t stcrddek_tbl_80044B38;
extern int stcrddek_word_8005CCA8;

int STCRDDEK_func_8008a21c(void)
{
    if (stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x16) != 0)
        return 1;
    if (stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x1D) != 0)
        return 1;
    if (stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x32) != 0)
        return 1;
    if (stcrddek_tbl_80044B38.fn(stcrddek_word_8005CCA8 + 0x86) != 0)
        return 1;
    return stcrddek_tbl_80044B38.fn(0x771) != 0;
}
