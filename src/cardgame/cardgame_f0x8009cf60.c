/*
 * CARDGAME:0x8009cf60 CARDGAME_F0x8009cf60
 * 92 bytes at CARDGAME.PRO offset 0x1a2b0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009cf60
 *  Symbols     func_8001ebf8=0x8001ebf8
 *  Compare     92 bytes from 0x8009cf60 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009cf60
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdff88 addiu sp,-0x78 ; sw s1,0x6c(sp) ; addu s1,a0 ; sw
 * s0,0x68(sp) ; addu s0,a1 ; sw ra,0x70(sp) ; jal 0x8001ebf8 ; addiu a0,sp,0x10
 *
 * Body: lw v0,0x50(s1) ; sll s0,s0,1 ; addu s0,s0,v0 ; lh a0,0(s0) ; lw
 * v0,0x3c(sp) ; nop ; jalr ra,v0 ; addiu a0,a0,1 ; lw v0,0x10(sp) ; lw
 * ra,0x70(sp) ; lw s1,0x6c(sp) ; lw s0,0x68(sp) ; lbu v0,0(v0) ; jr ra ; addiu
 * sp,+0x78
 *
 * Next function CARDGAME:0x8009cfbc at +0x5c, prev CARDGAME:0x8009ce94 ends
 * exactly at 0x8009cf60; size 0x5c is self-contained (prologue/epilogue pair).
 *
 * Callers 0 in overlay (no 0x0c0273d8 jal to here); callee jal EXE:0x8001ebf8
 * plus indirect jalr via stack slot; frame -0x78/+0x78 verified.
 *
 * Semantic: EXE helper fills stack buffer at sp+0x10; base=*(a0+0x50);
 * h=base[a1] signed halfword; cb=*(buf+0x2c); cb(h+1); return byte at *buf.
 *
 * Buffer void *buf[22] (88 B, vars=88 -> frame 0x78): only words 0 (result
 * pointer) and 11 (callback) are read here; the rest is written by the EXE
 * helper and untouched.
 */

extern void func_8001ebf8(void *buf);

unsigned char CARDGAME_F0x8009cf60(void *a0, int a1)
{
    void *buf[22];
    short *base;
    short h;
    void (*cb)(int);

    func_8001ebf8(buf);
    base = *(short **)((char *)a0 + 0x50);
    h = base[a1];
    cb = *(void (**)(int))((char *)buf + 0x2c);
    cb(h + 1);
    return *(unsigned char *)*(void **)buf;
}
