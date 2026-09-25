/*
 * CARDGAME:0x8009cda4 CARDGAME_F0x8009cda4
 * 88 bytes at CARDGAME.PRO offset 0x1a0f4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009cda4
 *  Symbols     D_8004B5DC=0x8004b5dc
 *  Compare     88 bytes from 0x8009cda4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009cda4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Frame -0x20/+0x20; saves s1/s0/ra; s1=a0; s0=0x8004B5DC (EXE dispatch base);
 * two indirect jalr via lw v0,36(s0) (slot 0x8004B600), a1=10, a0=s1+0x90 then
 * s1+0xE4.
 *
 * Ghidra CARDGAME (seeded): CARDGAME_F0x8009cda4 size 88 confirmed; 0 direct
 * jal callers (jal 0x0C027369 absent in overlay); prev 0x8009ca3c / next
 * 0x8009ce28.
 */

typedef void (*cardgame_dispatch_fn)(void *a0, int a1);

extern unsigned int D_8004B5DC[];

void CARDGAME_F0x8009cda4(void *arg) {
    ((cardgame_dispatch_fn)D_8004B5DC[9])((char *)arg + 0x90, 10);
    ((cardgame_dispatch_fn)D_8004B5DC[9])((char *)arg + 0xE4, 10);
}
