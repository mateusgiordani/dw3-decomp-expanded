/*
 * CARDGAME:0x8008f978 CARDGAME_F0x8008f978
 * 292 bytes at CARDGAME.PRO offset 0xccc8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008f978
 *  Symbols     (none)
 *  Compare     292 bytes from 0x8008f978 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008f978
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue addiu sp,-0x30 ; sw s1,0x1c(sp) ; move s1,a0 ; sw s2,0x20(sp) ; move
 * s2,a1 ; sw s0,0x18(sp) ; move s0,a2 ; sw s3,0x24(sp) ; move s3,a3
 *
 * Body: indirect call *(a1+0xf14)(a1,0x11,0xe500,0x6100) ; v=0 (clear a2) ; sh
 * zero,0x62c(a1) in beq delay slot ; switch (a3): case 0 ->
 * v=*(short*)(a0+row*2+a2*200+0x600), *(a0+0x438)=v, break ; case 1 ->
 * v=*(short*)(a0+row*2+a2*200+0x5b0), *(a0+0x438)=v (fallthrough join) ;
 * indirect call *(a1+0xf3c)(a1,0x11,v) ; indirect call
 * *(a1+0xf24)(a1,0x11,8,0x1000,0x1000) [5th arg on stack] ; *(a0+0x424)=0 ;
 * *(char*)(a0+0x422)=1
 *
 * Epilogue lw ra,0x28(sp) ; lw s3,0x24(sp) ; lw s2,0x20(sp) ; lw s1,0x1c(sp) ;
 * lw s0,0x18(sp) ; jr ra ; addiu sp,+0x30
 *
 * Callers: 2x jal from CARDGAME:0x80084320 (0x800845c0 with a3=0, 0x800845dc
 * with a3=1).
 *
 * No direct jal callees; 3 indirect jalr via table slots 0xf14/0xf3c/0xf24
 * (owning table unknown).
 *
 * Next function CARDGAME:0x8008fa9c at +0x124 (contiguous, no gap).
 *
 * - case-local declaration order (row-derived off before chain t) decides the
 * v0/v1 coloring of the address accumulation.
 */

typedef void (*cardgame_fn4_t)(void *, int, int, int);
typedef void (*cardgame_fn3_t)(void *, int, int);
typedef void (*cardgame_fn5_t)(void *, int, int, int, int);

void CARDGAME_F0x8008f978(void *a0, void *a1, int a2, int a3) {
    int v;

    (*(cardgame_fn4_t *)((char *)a1 + 0xf14))(a1, 0x11, 0xe500, 0x6100);
    v = 0;
    *(short *)((char *)a1 + 0x62c) = 0;
    switch (a3) {
    case 0: {
        int off = (*(int *)((char *)a0 + 0x440)) * 2;
        int t = a2 * 200;
        off += t;
        v = *(short *)((char *)a0 + off + 0x600);
        *(int *)((char *)a0 + 0x438) = v;
        break;
    }
    case 1: {
        int off = (*(int *)((char *)a0 + 0x440)) * 2;
        int t = a2 * 200;
        off += t;
        v = *(short *)((char *)a0 + off + 0x5b0);
        *(int *)((char *)a0 + 0x438) = v;
        break;
    }
    }
    (*(cardgame_fn3_t *)((char *)a1 + 0xf3c))(a1, 0x11, v);
    (*(cardgame_fn5_t *)((char *)a1 + 0xf24))(a1, 0x11, 8, 0x1000, 0x1000);
    *(int *)((char *)a0 + 0x424) = 0;
    ((char *)a0)[0x422] = 1;
}
