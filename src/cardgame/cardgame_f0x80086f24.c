/*
 * CARDGAME:0x80086f24 CARDGAME_F0x80086f24
 * 384 bytes at CARDGAME.PRO offset 0x4274 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80086f24
 *  Symbols     CARDGAME_F0x80087b80=0x80087b80
 *  Compare     384 bytes from 0x80086f24 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80086f24
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffd0 addiu sp,-0x30 ; sw s1,0x1c(sp) ; move s1,a0 ; sw
 * s0,0x18(sp) ; move s0,a1 ; sw s2,0x20(sp) ; move s2,a2 ; sw s3,0x24(sp) ;
 * move s3,a3 ; sw ra,0x28(sp)
 *
 * Body: indirect EXE call (*0x80055c48)(0x4001b) ; r=(**(p2+0xed8))(p3,idx) ;
 * (**(p2+0xf0c))(p2,idx,5,r+0x1800,0x6100 stacked) ; clear bit0 at
 * p2+idx*76+0x150 ; sh 0 at p2+idx*76+0x12e ; idx+=step ;
 * r=(**(p2+0xed8))(p3,idx) ; (**(p2+0xf0c))(p2,idx,1,r+0x1800,0x5c00 stacked) ;
 * set bit0 ; sh 1
 *
 * Epilogue jr ra + 27bd0030 addiu sp,+0x30 at 0x8008709c/0x800870a0; next
 * CARDGAME:0x800870a4 at +0x180 confirms size 0x180 contiguous
 * (prologue/epilogue pair).
 *
 * Callers pass (s4,s5,s6,+-1): a3 is a signed step; second ed8 call reuses live
 * a1=new idx (Ghidra decompile shows 1 arg, disasm proves 2).
 *
 * Matching notes: explicit int off=idx*76 temp keeps PLUS base-first (addu
 * v0,s0,v0); final p2+=off reuses dead p2 (addu s0,s0,v0) freeing v0 for li 1.
 */

#include <stdint.h>

typedef int32_t (*cardgame_lookup_t)(int32_t, int32_t);
typedef void (*cardgame_apply_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_exe_cb_t)(uint32_t);

void CARDGAME_F0x80086f24(int32_t p1, int32_t p2, int32_t p3, int32_t step)
{
    int32_t r;
    int32_t off;

    ((cardgame_exe_cb_t)*(uint32_t *)0x80055c48)(0x4001b);
    r = ((cardgame_lookup_t)*(uint32_t *)(p2 + 0xed8))(p3, *(int32_t *)(p1 + 0x43c));
    ((cardgame_apply_t)*(uint32_t *)(p2 + 0xf0c))(p2, *(int32_t *)(p1 + 0x43c), 5, r + 0x1800, 0x6100);
    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(uint8_t *)(p2 + off + 0x150) &= 0xfe;
    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(int16_t *)(p2 + off + 0x12e) = 0;
    *(int32_t *)(p1 + 0x43c) += step;
    r = ((cardgame_lookup_t)*(uint32_t *)(p2 + 0xed8))(p3, *(int32_t *)(p1 + 0x43c));
    ((cardgame_apply_t)*(uint32_t *)(p2 + 0xf0c))(p2, *(int32_t *)(p1 + 0x43c), 1, r + 0x1800, 0x5c00);
    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(uint8_t *)(p2 + off + 0x150) |= 1;
    off = *(int32_t *)(p1 + 0x43c) * 76;
    p2 += off;
    *(int16_t *)(p2 + 0x12e) = 1;
}
