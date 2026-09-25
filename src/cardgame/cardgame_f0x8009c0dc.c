/*
 * CARDGAME:0x8009c0dc CARDGAME_F0x8009c0dc
 * 72 bytes at CARDGAME.PRO offset 0x1942c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009c0dc
 *  Symbols     CARDGAME_F0x8009d310=0x8009d310 _DAT_80055c48=0x80055c48
 *  Compare     72 bytes from 0x8009c0dc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009c0dc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffe8 addiu sp,-0x18 ; sw s0,0x10(sp) ; move s0,a0 ; lui a0,4 ;
 * lui v0,0x8005 ; lw v0,0x5c48(v0) ; sw ra,0x14(sp) ; jalr v0 ; ori a0,0x1a
 * delay
 *
 * Body: addiu v0,6 ; sh v0,0xdf2(s0) ; sh v0,0xdf0(s0) ; addiu v0,5 ; sb
 * v0,0xdfa(s0) ; lw ra,0x14(sp) ; lw s0,0x10(sp) ; jr ra ; addiu sp,+0x18
 *
 * Next function CARDGAME:0x8009c124 at +0x48 confirms size 0x48 is
 * self-contained.
 *
 * Ghidra: CARDGAME_F0x8009c0dc decompiled as (*_DAT_80055c48)(0x4001a);
 * *(short*)(param+0xdf2)=6; *(short*)(param+0xdf0)=6; *(char*)(param+0xdfa)=5;
 *
 * Caller: DATA XREF from CARDGAME_F0x8009d310 (0x8009d39c); no direct jal
 * callers (sweep 0)
 *
 * Callee: indirect jalr via v0 loaded from *0x80055c48 (EXE global)
 */

void CARDGAME_F0x8009c0dc(void *arg) {
    (*(void (**)(int))0x80055c48)(0x4001a);
    *(short *)((char *)arg + 0xdf2) = 6;
    *(short *)((char *)arg + 0xdf0) = 6;
    *(char *)((char *)arg + 0xdfa) = 5;
}
