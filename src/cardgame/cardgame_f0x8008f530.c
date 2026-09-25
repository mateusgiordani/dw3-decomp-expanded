/*
 * CARDGAME:0x8008f530 CARDGAME_F0x8008f530
 * 48 bytes at CARDGAME.PRO offset 0xc880 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008f530
 *  Symbols     func_8008f2d8=0x8008f2d8
 *  Compare     48 bytes from 0x8008f530 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008f530
 */

extern void func_8008f2d8(void *ctx, int a1, int a2, int a3);

void CARDGAME_F0x8008f530(int *ctx, int a1) {
    func_8008f2d8(ctx, a1, -0x80, 0x10);
    ctx[0x438 / 4] = 0;
}
