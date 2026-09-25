/*
 * CARDGAME:0x80085fd0 CARDGAME_F0x80085fd0
 * 192 bytes at CARDGAME.PRO offset 0x3320 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80085fd0
 *  Symbols     CARDGAME_F0x80085f38=0x80085f38 F0x8001ebf8=0x8001ebf8
 *  Compare     192 bytes from 0x80085fd0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80085fd0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdff80 addiu sp,-0x80 ; sw s0,0x68(sp) ; move s0,a0 ; sw
 * s3,0x74(sp) ; move s3,a1 ; sw s2,0x70(sp) ; move s2,a2 ; sw s1,0x6c(sp) ; sw
 * ra,0x78(sp)
 *
 * Body: lbu v1,0x2f8(s0) ; li v0,6 ; bne v1,v0 -> else ; move s1,zero (delay,
 * always) ; jal 0x8001ebf8 ; addiu a0,sp,0x10 (delay) ; sll v0,s2,1 ; addu
 * v0,s0,v0 ; lh a0,0x50(v0) ; lw v0,0x3c(sp) ; nop ; jalr v0 ; addiu a0,a0,1
 * (delay) ; lw a0,0x10(sp) ; nop ; lbu v1,3(a0) ; li v0,0x10 ; bne v1,v0 -> ret
 * ; move v0,s1 (delay, always) ; lbu v0,0(a0) ; lbu v1,5(a0) ; addu v0,v0,s3 ;
 * lbu v0,-1(v0) ; nop ; sltu v0,v0,v1 ; j join ; xori s1,v0,1 (delay)
 *
 * Else: move a0,s0 ; jal 0x80085f38 ; move a1,s2 (delay) ; move s1,v0 ; join:
 * move v0,s1 ; ret: epilogue lw ra,0x78(sp) ; lw s3,0x74(sp) ; lw s2,0x70(sp) ;
 * lw s1,0x6c(sp) ; lw s0,0x68(sp) ; jr ra ; addiu sp,+0x80
 *
 * Prev CARDGAME:0x80085f38 size 0x98 ends at 0x80085fd0 exactly - size 0xc0
 * contiguous prologue/epilogue pair.
 *
 * Callers pass a1 as pointer (0x800863b4: a1=s1+0x5a8 into same struct;
 * 0x80087280: a1=s4+0xc; 0x80086de4: a1=s3+0xc) and a2 as halfword index (lh) -
 * so k is a byte table and q[0] a 1-based index: b = k[q[0]-1].
 *
 * Callee EXE:0x8001ebf8 fills stack buffer at sp+0x10 (offsets +0x0 pointer,
 * +0x2c callback); sibling CARDGAME:0x80085f38 handles the flag!=6 path.
 */

extern void F0x8001ebf8(void *);
extern unsigned int CARDGAME_F0x80085f38(void *, int);

unsigned int CARDGAME_F0x80085fd0(void *p, unsigned char *k, int idx) {
    unsigned int buf[22];
    unsigned int r = 0;

    if (*(unsigned char *)((char *)p + 0x2f8) == 6) {
        short h;
        unsigned char *q;
        void (*cb)(int);
        int off;

        F0x8001ebf8(buf);
        off = idx << 1;
        h = *(short *)((char *)p + off + 0x50);
        cb = (void (*)(int))buf[11];
        cb(h + 1);
        q = (unsigned char *)buf[0];
        if (q[3] == 0x10)
            r = k[q[0] - 1] >= q[5];
    } else {
        r = CARDGAME_F0x80085f38(p, idx);
    }
    return r;
}
