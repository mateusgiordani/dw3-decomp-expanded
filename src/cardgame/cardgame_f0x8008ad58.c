/*
 * CARDGAME:0x8008ad58 CARDGAME_F0x8008ad58
 * 32 bytes at CARDGAME.PRO offset 0x80a8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008ad58
 *  Symbols     func_80089ed4=0x80089ed4
 *  Compare     32 bytes from 0x8008ad58 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008ad58
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffe8 addiu sp,-0x18 ; sw ra,0x10(sp) ; jal 0x80089ed4 ; ...
 *
 * Next function CARDGAME:0x8008ad78 at +0x20, confirms size.
 */

extern void func_80089ed4(void);

void CARDGAME_F0x8008ad58(void) {
    func_80089ed4();
}
