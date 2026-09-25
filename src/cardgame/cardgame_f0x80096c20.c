/*
 * CARDGAME:0x80096c20 CARDGAME_F0x80096c20
 * 208 bytes at CARDGAME.PRO offset 0x13f70 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80096c20
 *  Symbols     CARDGAME_F0x80096cf0=0x80096cf0 D0x80044f5c=0x80044f5c
 *              D0x800a5978=0x800a5978 F0x8001f648=0x8001f648
 *              exe_f0x8001f648=0x8001f648
 *  Compare     208 bytes from 0x80096c20 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80096c20
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdff40 addiu sp,-0xc0 ; epilogue jr ra / 27bd00c0 addiu sp,+0xc0
 *
 * Next CARDGAME:0x80096cf0 at +0xd0 (sweep #114); ends at 0x80096cf0.
 *
 * Caller: CARDGAME_F0x80096cf0 @0x80096dfc (jal a0=s2/a1=s3/a2=s4/a3=s0, guide
 * .L0x0001414c).
 *
 * Data: CARDGAME table at 0x800a5978 (halfword pairs indexed by a2*4); EXE
 * indirect slot at 0x80044f5c (lui 0x8004 / lw 0x4f5c).
 */

#include <stdint.h>

extern void F0x8001f648(void *buf);
extern int16_t D0x800a5978[];
extern void *D0x80044f5c;

typedef void (*CardCb2)(int32_t a0, int32_t a1);
typedef void (*CardCb3)(int32_t a0, int32_t a1, int32_t a2);
typedef int32_t (*CardCbR)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*ExeSlotFn)(int32_t a0);

typedef struct {
    uint8_t pad[0x74];
    CardCb2 cb74;
    CardCb2 cb78;
    CardCb2 cb7c;
    CardCb2 cb80;
    CardCbR cb84;
    CardCb2 cb88;
    CardCb3 cb8c;
    CardCb2 cb90;
    CardCb2 cb94;
    CardCb2 cb98;
    CardCb2 cb9c;
} CardCtx96c20;

void CARDGAME_F0x80096c20(int32_t a0, int32_t a1, int32_t a2, int16_t *a3)
{
    CardCtx96c20 ctx;
    int16_t *base;
    int16_t *tbl;
    int32_t r;

    (void)a0;
    (void)a1;
    F0x8001f648(&ctx);
    ctx.cb7c(0x100, 1);
    ctx.cb74(0x340, 0);
    base = D0x800a5978;
    tbl = base + a2 * 2;
    ctx.cb94(tbl[0] + 4, tbl[1] + 0x17);
    ctx.cb8c(0x1000, a3[1], 0x1000);
    r = ((ExeSlotFn)D0x80044f5c)(0x25d0003);
    ctx.cb84(r, a3[5] + 6, tbl[0], tbl[1]);
}
