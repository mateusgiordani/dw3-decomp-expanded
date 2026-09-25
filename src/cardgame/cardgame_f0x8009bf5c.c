/*
 * CARDGAME:0x8009bf5c CARDGAME_F0x8009bf5c
 * 112 bytes at CARDGAME.PRO offset 0x192ac (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009bf5c
 *  Symbols     CARDGAME_F0x8009d310=0x8009d310 _DAT_80055c48=0x80055c48
 *  Compare     112 bytes from 0x8009bf5c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009bf5c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffe0 addiu sp,-0x20 ; sw s0,0x10(sp) ; move s0,a0 ; sw
 * s1,0x14(sp) ; move s1,a1
 *
 * Setup lui a0,0x4 ; lui v0,0x8005 ; lw v0,0x5c48(v0) ; sw ra,0x18(sp) ; jalr
 * v0 ; ori a0,0x1a (delay, a0=0x4001a)
 *
 * Body sll v0,s1,1 ; addu v0,v0,s1 ; sll v0,v0,3 (v0 = s1*24) ; addu s0,s0,v0
 * sh 0x1000 at +0xe10/+0xe12 ; sb 3 at +0xe23 ; sh 6 at +0xe16/+0xe14 ; restore
 * ; jr ra ; addiu sp,+0x20
 *
 * Prev CARDGAME:0x8009beac (176B) ends exactly at 0x8009bf5c ; next framed
 * CARDGAME:0x8009c054 at +0xf8 (0x88 leaf gap)
 *
 * Xref: DATA ref from CARDGAME_F0x8009d310+0x5c (sw v1,0xeb0(v0) with v1 =
 * 0x800a0000-0x40a4 = 0x8009bf5c): 0x8009bf5c is registered as the callback at
 * table slot +0xeb0 (no direct jal callers).
 *
 * Callee *0x80055c48 is EXE-resident shared vector (same idiom across
 * 0x8009c0dc/0x8009c124/0x8009c1cc/0x8009c214 family).
 */

typedef void (*cardgame_vec_t)(int);

#define CARDGAME_VEC (*(cardgame_vec_t *)0x80055c48)

void CARDGAME_F0x8009bf5c(char *base, int index)
{
    char *entry;

    CARDGAME_VEC(0x4001a);
    entry = base + index * 24;
    *(short *)(entry + 0xE10) = 0x1000;
    *(short *)(entry + 0xE12) = 0x1000;
    entry[0xE23] = 3;
    *(short *)(entry + 0xE16) = 6;
    *(short *)(entry + 0xE14) = 6;
}
