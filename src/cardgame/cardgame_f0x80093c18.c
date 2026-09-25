/*
 * CARDGAME:0x80093c18 CARDGAME_F0x80093c18
 * 148 bytes at CARDGAME.PRO offset 0x10f68 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80093c18
 *  Symbols     CARDGAME_F0x80084320=0x80084320
 *  Compare     148 bytes from 0x80093c18 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80093c18
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue: addiu sp,-0x28 ; sw s1,0x1c(sp) ; addu s1,a0 ; sw s0,0x18(sp) ;
 * addu s0,a1 ; addu a0,s0 ; addiu a1,0x11 ; ori a2,0x8300 ; sw ra,0x20(sp)
 *
 * Body: 3 indirect jalr dispatches via s0-relative table slots
 * +0xf14/+0xf3c/+0xf24 (call 2 passes lbu(s1+0x304)-1 in the jalr delay slot;
 * call 3 is a 5-arg call spilling 0x1000 at sp+0x10 after sh zero,0x62c(a0));
 * final sb 1,0x422(s1)
 *
 * Epilogue: lw ra/s1/s0 ; jr ra ; addiu sp,+0x28
 *
 * Next function CARDGAME:0x80093cac at +0x94 (contiguous, no gap).
 *
 * Xref: 1 caller CARDGAME_F0x80084320+0xd4 (UNCONDITIONAL_CALL at 0x800843f4).
 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef void (*cb4_t)(void *, int, unsigned int, unsigned int);
typedef void (*cb3_t)(void *, int, unsigned int);
typedef void (*cb5_t)(void *, int, unsigned int, unsigned int, unsigned int);
void CARDGAME_F0x80093c18(void *a0, void *a1)
{
    void *s1 = a0;
    void *s0 = a1;
    ((cb4_t)*(cb4_t *)((char *)s0 + 0xF14))(s0, 17, 0x8300, 0x6100);
    ((cb3_t)*(cb3_t *)((char *)s0 + 0xF3C))(s0, 17, (unsigned int)(*(u8 *)((char *)s1 + 0x304) - 1));
    *(u16 *)((char *)s0 + 0x62C) = 0;
    ((cb5_t)*(cb5_t *)((char *)s0 + 0xF24))(s0, 17, 10, 0x1000, 0x1000);
    *(u8 *)((char *)s1 + 0x422) = 1;
}
