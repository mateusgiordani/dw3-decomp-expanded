/*
 * CARDGAME:0x80095e58 CARDGAME_F0x80095e58
 * 348 bytes at CARDGAME.PRO offset 0x131a8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80095e58
 *  Symbols     DAT_80044f5c=0x80044f5c DAT_8004df9c=0x8004df9c
 *              DAT_8005df9c=0x8005df9c EXE_F0x8001f648=0x8001f648
 *  Compare     348 bytes from 0x80095e58 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80095e58
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue addiu sp,-0xb8 ; sw s0,0xb0(sp) ; move s0,a0 ; sw
 * ra,0xb4(sp) ; epilogue lw ra/lw s0 ; jr ra ; addiu sp,+0xb8.
 *
 * Prev CARDGAME:0x80095e2c size 44 (0x2c) ends exactly at 0x80095e58 (gap 0);
 * next CARDGAME:0x80095fb4 at +0x15c (gap 0) confirms size 0x15c contiguous.
 *
 * CORRECTION (continuation): trailing getter is *(0x8004df9c) — raw words
 * 3c028005/8c42df9c give 0x80050000-0x2064 = 0x8004df9c; prior DAT_8005df9c was
 * a one-digit typo (verified against PAL bytes, both EXE vectors now
 * 0x8004xxxx-consistent: 0x80044f5c / 0x8004df9c).
 *
 * Callee: 1 direct jal EXE 0x8001f648(a0=sp+0x10 local table); 6 stack-table
 * indirect jalr (lw v0,off(sp)+nop+jalr, MIPS-I load delay) at sp+0xa4/0x9c/
 * 0x98/0x8c/0x84/0x94 (buf words 37/35/34/31/29/33 from sp+0x10); 1 EXE-vector
 * jalr via *(0x80044f5c) with a0=0x025d0002; final *(s0+0x50) +=
 * (*0x8004df9c)().
 *
 * Body order: init(buf) ; f94(h54,h56+0x13) ; f8c(h58,h5a,0x1000) ; f88(mod/min
 * select) ; f7c(0x100,1) ; f74(0x280,0) ; svc=DAT(0x025d0002) ;
 * f84(svc,0x47,h54,h56) ; w50 += DAT_8004df9c().
 *
 * Mod/min select: byte+0x63 == 0 -> v=(w50>>2)%16 (bgez/bias/sra/sll/subu
 * signed-modulo idiom); else v=(w50>>1), v=min(v,7) via slti/bne/li.
 *
 * v is deliberately block-scoped per branch with the slot call inside each
 * branch: this keeps both lw v0,0x98(sp) copies and computes the else value
 * directly in a0 (no move), which is what exact-matches.
 */

#include "common/types.h"

typedef void (*cardgame_cb1_t)(int32_t a0);
typedef void (*cardgame_cb2_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_cb3_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*cardgame_cb4_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*cardgame_svc_t)(uint32_t arg);
typedef int32_t (*cardgame_get_t)(void);

void EXE_F0x8001f648(void *buf);
extern cardgame_svc_t DAT_80044f5c;
extern cardgame_get_t DAT_8004df9c;

typedef struct {
    uint32_t w[40];
} cardgame_95e58_buf_t;

void CARDGAME_F0x80095e58(uint8_t *p)
{
    cardgame_95e58_buf_t buf;
    int32_t svc;

    if (*(int16_t *)(p + 0x58) == 0) {
        return;
    }
    if (*(int16_t *)(p + 0x5a) == 0) {
        return;
    }
    EXE_F0x8001f648(buf.w);
    {
        int32_t a0 = *(int16_t *)(p + 0x54);
        int32_t a1 = *(int16_t *)(p + 0x56) + 0x13;
        ((cardgame_cb2_t)buf.w[37])(a0, a1);
    }
    {
        int32_t a0 = *(int16_t *)(p + 0x58);
        int32_t a1 = *(int16_t *)(p + 0x5a);
        ((cardgame_cb3_t)buf.w[35])(a0, a1, 0x1000);
    }
    if (*(p + 0x63) == 0) {
        int32_t v = *(int32_t *)(p + 0x50) >> 2;
        v = v % 16;
        ((cardgame_cb1_t)buf.w[34])(v);
    } else {
        int32_t v = *(int32_t *)(p + 0x50) >> 1;
        if (v >= 7) {
            v = 7;
        }
        ((cardgame_cb1_t)buf.w[34])(v);
    }
    ((cardgame_cb2_t)buf.w[31])(0x100, 1);
    ((cardgame_cb2_t)buf.w[29])(0x280, 0);
    svc = DAT_80044f5c(0x025D0002u);
    {
        int32_t a2 = *(int16_t *)(p + 0x54);
        int32_t a3 = *(int16_t *)(p + 0x56);
        ((cardgame_cb4_t)buf.w[33])(svc, 0x47, a2, a3);
    }
    *(int32_t *)(p + 0x50) += DAT_8004df9c();
}