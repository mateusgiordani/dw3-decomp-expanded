/*
 * CARDGAME:0x80097958 CARDGAME_F0x80097958
 * 204 bytes at CARDGAME.PRO offset 0x14ca8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80097958
 *  Symbols     CARDGAME_F0x80097acc=0x80097acc DAT_80044f5c=0x80044f5c
 *              EXE_F0x8001f648=0x8001f648
 *  Compare     204 bytes from 0x80097958 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80097958
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function; prologue addiu sp,-0xc0 ; sw s0,0xb0(sp) ; move s0,a2 ; sw
 * s1,0xb4(sp) ; move s1,a3 ; sw s2,0xb8(sp) ; lw s2,0xd0(sp) ; sw ra,0xbc(sp) ;
 * epilogue lw ra/s2/s1/s0 ; jr ra ; addiu sp,+0xc0
 *
 * Next function CARDGAME:0x80097a24 at +0xcc (same -0xc0 prologue shape, gap 0)
 * confirms size 0xcc contiguous.
 *
 * Callers: 1 direct jal from CARDGAME_F0x80097acc at 0x80098454 (delay _sra
 * a2,a2,0x10).
 *
 * Callees: 1 direct jal EXE 0x8001f648(a0=sp+0x10 local table); 5 stack-table
 * indirect jalr (lw v0,off(sp)+nop+jalr, MIPS-I load delay); 1 EXE-vector jalr
 * via *(0x80044f5c) with a0=0x025d0002.
 *
 * Stack table stride 8: func pointers at sp+0x84/0x8c/0x94/0x9c/0xa4 (buf words
 * 29/31/33/35/37 from sp+0x10), even words are paired data.
 *
 * Frame is 0xc0 = 16 (o32 home area at sp+0x00) + 160 (table w[40] at sp+0x10)
 * + 16 (s0/s1/s2/ra saves); a 176-byte table over-allocates to 0xd0.
 *
 * Body order: init(buf) ; f8c(0x100,1) ; f84(0x280,0) ; fa4(0,0x78) ;
 * f9c((int16)a2,0x1000,0x1000) ; tmp=DAT_80044f5c(0x025d0002) ;
 * f94(tmp,0x1a,a3,stackarg).
 */

#include "common/types.h"

typedef void (*cardgame_cb2_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_cb3_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*cardgame_cb4_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*cardgame_svc_t)(uint32_t arg);

void EXE_F0x8001f648(void *buf);
extern cardgame_svc_t DAT_80044f5c;

typedef struct {
    uint32_t w[40];
} cardgame_97958_buf_t;

void CARDGAME_F0x80097958(int32_t p0, int32_t p1, int32_t p2, int32_t p3, int32_t p4)
{
    cardgame_97958_buf_t buf;
    int32_t svc;

    (void)p0;
    (void)p1;
    EXE_F0x8001f648(buf.w);
    ((cardgame_cb2_t)buf.w[31])(0x100, 1);
    ((cardgame_cb2_t)buf.w[29])(0x280, 0);
    ((cardgame_cb2_t)buf.w[37])(0, 0x78);
    ((cardgame_cb3_t)buf.w[35])((int16_t)p2, 0x1000, 0x1000);
    svc = DAT_80044f5c(0x025D0002u);
    ((cardgame_cb4_t)buf.w[33])(svc, 0x1a, p3, p4);
}
