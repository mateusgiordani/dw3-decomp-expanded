/*
 * CARDGAME:0x80096e24 CARDGAME_F0x80096e24
 * 104 bytes at CARDGAME.PRO offset 0x14174 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80096e24
 *  Symbols     CARDGAME_F0x80096cf0=0x80096cf0 CARDGAME_F0x8009bba8=0x8009bba8
 *  Compare     104 bytes from 0x80096e24 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80096e24
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue 27bdffd8 addiu sp,-0x28, epilogue jr ra / addiu
 * sp,+0x28.
 *
 * Loop wrapper: 3 iterations calling CARDGAME_F0x80096cf0(a0, a1, index, a0 +
 * off), off steps 0xdc0, 0xdcc, 0xdd8 (stride 12, 12-byte entries).
 */

extern void CARDGAME_F0x80096cf0(int a0, int a1, int index, int arg);

void CARDGAME_F0x80096e24(int a0, int a1)
{
    int index;
    int off;

    index = 0;
    off = 0xdc0;
    do {
        CARDGAME_F0x80096cf0(a0, a1, index, a0 + off);
        index += 1;
        off += 12;
    } while (index < 3);
}
