/*
 * CARDGAME:0x8009c904 CARDGAME_F0x8009c904
 * 52 bytes at CARDGAME.PRO offset 0x19c54 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009c904
 *  Symbols     func_8009c898=0x8009c898
 *  Compare     52 bytes from 0x8009c904 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009c904
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffe0 addiu sp,-0x20 ; jal 0x8009c898 ; epilogue 27bd0020
 *
 * Next function CARDGAME:0x8009c938 at +0x34, confirms size.
 */

extern void func_8009c898(void *a0, int a1, int a2, int a3, int a4, int a5);

void CARDGAME_F0x8009c904(void *a0, int a1, int a2, int a3) {
    func_8009c898(a0, a1, 0, a2, a3, 1);
}
