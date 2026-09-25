/*
 * CARDGAME:0x800961f0 CARDGAME_F0x800961f0
 * 552 bytes at CARDGAME.PRO offset 0x13540 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800961f0
 *  Symbols     CARDGAME_F0x80096418=0x80096418 DAT_80044f5c=0x80044f5c
 *              DAT_8004DF9C=0x8004df9c DAT_800836f0=0x800836f0
 *              EXE_F0x8001f648=0x8001f648
 *  Compare     552 bytes from 0x800961f0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800961f0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue addiu sp,-0x170 (saves s0/s1/ra), epilogue jr ra /
 * addiu sp,+0x170.
 *
 * Guard: halfwords at obj+0x5c/+0x5e must both be nonzero, else return (both
 * beq to epilogue).
 *
 * Layout: 16-byte halfword table copy at sp+0x10 (from CARDGAME 0x800836f0,
 * lwl/lwr block copy), two 160-byte callback tables at sp+0x20/sp+0xc0 (filled
 * by EXE 0x8001f648), func pointers at buf words 29/31/33/35/37
 * (+0x74/+0x7c/+0x84/+0x8c/+0x94, stride 8) plus word 34 (+0x88) for the
 * conditional middle call; s1 holds the sp+0x10 table base for the indexed lh.
 *
 * Body order (PAL order): fill b1; b1.w37(h58,h5a); b1.w35(h5c,h5e,0x1000);
 * b1.w31(0x100,1); b1.w29(0x280,0); svc=DAT_80044f5c(0x25D0002);
 * b1.w33(svc,0x45,h58,h5a); fill b2; b2.w37(h58,h5a); if (byte+0x67): v=w54>>1
 * (sra); clamp 7; b2.w34(tbl[v]); w54 += DAT_8004DF9C();
 * b2.w35(h5c,h5e,0x1000); b2.w31(0x100,1); b2.w29(0x280,0);
 * svc=DAT_80044f5c(0x25D0002); b2.w33(svc,0x46,h58,h5a). Note 0x45 first phase,
 * 0x46 tail.
 *
 * Next function at 0x80096418 (+0x228) confirms size; no Ghidra mutation.
 *
 * Caller: CARDGAME_F0x80096418 via 0x800967d0 UNCONDITIONAL_CALL (single
 * caller).
 *
 * Callees: direct EXE 0x8001f648 x2; indirect stack slots
 * sp+0x94/0x9c/0xa4/0xac/0xb4 and sp+0x134/0x13c/0x144/0x148/0x14c/0x154; EXE
 * vectors *0x80044f5c x2, *0x8004DF9C x1.
 *
 * Portable C recovery (campaign cardgame-human-v1 rev 2): no __asm__, no words,
 * no register variables. Raw byte offsets for obj fields: int16_t for lh
 * halves, int32_t for the lw accumulator, uint8_t for the lbu flag. Signedness
 * of the table halves follows lh (int16_t); tbl struct align 2 matches the
 * lwl/lwr copy.
 */

#include <stdint.h>

extern void EXE_F0x8001f648(void *buf);
extern uint32_t (*DAT_80044f5c)(uint32_t);
extern int32_t (*DAT_8004DF9C)(void);

typedef struct {
    int16_t h[8];
} cardgame_f0_tbl_t;

extern cardgame_f0_tbl_t DAT_800836f0;

typedef void (*cardgame_cb1_t)(int32_t a0);
typedef void (*cardgame_cb2_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_cb3_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*cardgame_cb4_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);

typedef struct {
    uint32_t w[40];
} cardgame_f0_cb_t;

void CARDGAME_F0x800961f0(uint8_t *obj) {
    cardgame_f0_tbl_t tbl;
    cardgame_f0_cb_t b1;
    cardgame_f0_cb_t b2;
    uint32_t svc;
    int32_t v;

    tbl = DAT_800836f0;
    if (*(int16_t *)(obj + 0x5c) == 0 || *(int16_t *)(obj + 0x5e) == 0)
        return;
    EXE_F0x8001f648(b1.w);
    ((cardgame_cb2_t)b1.w[37])(*(int16_t *)(obj + 0x58), *(int16_t *)(obj + 0x5a));
    ((cardgame_cb3_t)b1.w[35])(*(int16_t *)(obj + 0x5c), *(int16_t *)(obj + 0x5e), 0x1000);
    ((cardgame_cb2_t)b1.w[31])(0x100, 1);
    ((cardgame_cb2_t)b1.w[29])(0x280, 0);
    svc = DAT_80044f5c(0x25D0002u);
    ((cardgame_cb4_t)b1.w[33])(svc, 0x45, *(int16_t *)(obj + 0x58), *(int16_t *)(obj + 0x5a));
    EXE_F0x8001f648(b2.w);
    ((cardgame_cb2_t)b2.w[37])(*(int16_t *)(obj + 0x58), *(int16_t *)(obj + 0x5a));
    if (*(obj + 0x67) != 0) {
        v = *(int32_t *)(obj + 0x54) >> 1;
        if (v > 6)
            v = 7;
        ((cardgame_cb1_t)b2.w[34])(tbl.h[v]);
        *(int32_t *)(obj + 0x54) = *(int32_t *)(obj + 0x54) + DAT_8004DF9C();
    }
    ((cardgame_cb3_t)b2.w[35])(*(int16_t *)(obj + 0x5c), *(int16_t *)(obj + 0x5e), 0x1000);
    ((cardgame_cb2_t)b2.w[31])(0x100, 1);
    ((cardgame_cb2_t)b2.w[29])(0x280, 0);
    svc = DAT_80044f5c(0x25D0002u);
    ((cardgame_cb4_t)b2.w[33])(svc, 0x46, *(int16_t *)(obj + 0x58), *(int16_t *)(obj + 0x5a));
}
