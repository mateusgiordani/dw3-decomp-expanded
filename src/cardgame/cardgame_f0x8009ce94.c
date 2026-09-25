/*
 * CARDGAME:0x8009ce94 CARDGAME_F0x8009ce94
 * 204 bytes at CARDGAME.PRO offset 0x1a1e4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009ce94
 *  Symbols     CARDGAME_F0x8009c510=0x8009c510 CARDGAME_F0x8009c9a0=0x8009c9a0
 *              CARDGAME_F0x8009d310=0x8009d310 F0x8009c510=0x8009c510
 *              F0x8009c9a0=0x8009c9a0 FUN_80098864=0x80098864
 *  Compare     204 bytes from 0x8009ce94 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009ce94
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffc8 addiu sp,-0x38 ; sw s3,0x24(sp) ; move s3,a0 ; sw
 * s0,0x18(sp) ; clear s0 ; sw s6,0x30(sp) ; move s6,a3 ; sll a0,a2,0x10 ; sw
 * s2,0x20(sp) ; lw s2,0x48(sp) ; sra v1,a0,0x10 ; sw ra,0x34(sp) ; sw
 * s5,0x2c(sp) ; sw s4,0x28(sp) ; blez v1,+0x1a delay sw s1,0x1c(sp)
 *
 * Body: move s5,a0 ; sll v0,a1,0x10 ; sra s1,v0,0x10 ; move s4,v1 ; loop: sra
 * a0,s5,0x10 ; jal 0x80098864 delay move a1,s0 ; bne s1,zero,+8 delay addu
 * a3,s6,v0 ; then: move a0,s3 ; move a1,s0 ; move a2,a3 ; jal 0x8009c510 delay
 * move a3,s2 ; j latch+ delay addiu s0,s0,1 ; else: sw s2,0x10(sp) ; move a0,s3
 * ; move a1,s0 ; jal 0x8009c9a0 delay move a2,s1 ; addiu s0,s0,1 ; latch: slt
 * v0,s0,s4 ; bne v0,zero,loop delay sra a0,s5,0x10
 *
 * Epilogue: lw ra,0x34(sp) ; lw s6,0x30(sp) ; lw s5,0x2c(sp) ; lw s4,0x28(sp) ;
 * lw s3,0x24(sp) ; lw s2,0x20(sp) ; lw s1,0x1c(sp) ; lw s0,0x18(sp) ; jr ra ;
 * addiu sp,+0x38
 *
 * Next function CARDGAME:0x8009cf60 at +0xcc (addiu sp,-0x78) confirms size
 * 0xcc is self-contained (prologue/epilogue pair).
 *
 * Caller: DATA XREF from CARDGAME_F0x8009d310 (0x8009d4b0 installs 0x8009ce94
 * into table slot 0xf04(v0)); no direct jal callers.
 *
 * Callees: jal 0x80098864 (int,int->int), jal 0x8009c510 (4 args), jal
 * 0x8009c9a0 (5 args, stack slot sp+0x10).
 */

extern int FUN_80098864(int a, int b);
extern int CARDGAME_F0x8009c510(int a, int b, int c, int d);
extern void CARDGAME_F0x8009c9a0(int a, int b, int c, int d, int e);
void CARDGAME_F0x8009ce94(int arg0, short arg1, short arg2, int arg3, int arg4)
{
    int i = 0;
    int t;
    if (arg2 <= 0)
        return;
    do {
        t = arg3 + FUN_80098864(arg2, i);
        if (arg1 == 0)
            CARDGAME_F0x8009c510(arg0, i, t, arg4);
        else
            CARDGAME_F0x8009c9a0(arg0, i, arg1, t, arg4);
        i++;
    } while (i < arg2);
}
