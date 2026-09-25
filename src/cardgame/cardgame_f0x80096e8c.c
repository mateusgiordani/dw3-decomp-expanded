/*
 * CARDGAME:0x80096e8c CARDGAME_F0x80096e8c
 * 940 bytes at CARDGAME.PRO offset 0x141dc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80096e8c
 *  Symbols     CARDGAME_F0x80096a94=0x80096a94 CARDGAME_F0x80097508=0x80097508
 *              D0x80044f5c=0x80044f5c D0x800a5984=0x800a5984
 *              F0x8001f648=0x8001f648 exe_f0x8001f648=0x8001f648
 *  Compare     940 bytes from 0x80096e8c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80096e8c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdff28 addiu sp,-0xd8 ; epilogue jr ra / 27bd00d8 addiu sp,+0xd8
 *
 * Next CARDGAME:0x80097238 at +0x3ac; ends at 0x80097238.
 *
 * Caller: CARDGAME_F0x80097508 @0x800978c4 (jal, delay _move a2,s1).
 *
 * Callees: CARDGAME 0x80096a94 (card-arg struct at sp+0x10, flag 1); EXE
 * indirect slot at 0x80044f5c (lui 0x8004 / lw 0x4f5c).
 *
 * Data: CARDGAME table at 0x800a5984, 6-byte entries indexed by p[6]*6
 * (halfwords +0/+2 via lhu+lh, bytes +4/+5 via lbu).
 *
 * Stack: single 0xb8-byte frame region (sp+0x10..sp+0xc7) holds the card-arg
 * struct (sp+0x10, 0x11 bytes, consumed by 0x80096a94) and BOTH callback
 * structs (sp+0x28 branch1, sp+0x10 branches 2/3) with live-range overlap;
 * modeled as one byte array with documented offsets (PAL reuses the slots).
 */

#include <stdint.h>

extern void CARDGAME_F0x80096a94(void *arg, int32_t flag);
extern void F0x8001f648(void *buf);
extern void *D0x80044f5c;

typedef void (*CardCb2)(int32_t a0, int32_t a1);
typedef void (*CardCb3)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*CardCb4)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*ExeSlotFn)(int32_t a0);

/* 6-byte table entry at D0x800a5984, indexed by p[6] (v1*6 via sll/addu/sll). */
typedef struct {
    int16_t w0;
    int16_t w1;
    uint8_t b4;
    uint8_t b5;
} CardTbl96e8c;

extern CardTbl96e8c D0x800a5984[];

/* 0xa0-byte callback struct filled by F0x8001f648 (same layout as the
 * 0x80096c20 neighbor: slots at +0x74/+0x7c/+0x80/+0x84/+0x88/+0x8c/+0x90/
 * +0x94/+0x98/+0x9c; this function uses +0x74/+0x7c/+0x84/+0x8c/+0x94). */
typedef struct {
    uint8_t pad[0x74];
    CardCb2 cb74;
    CardCb2 cb78;
    CardCb2 cb7c;
    CardCb2 cb80;
    CardCb4 cb84;
    CardCb2 cb88;
    CardCb3 cb8c;
    CardCb2 cb90;
    CardCb2 cb94;
    CardCb2 cb98;
    CardCb2 cb9c;
} CardCtx96e8c;

/* Card-arg struct built at sp+0x10 for CARDGAME_F0x80096a94 (0x11 bytes):
 * +0x00 p[8]&0xf, +0x02 *p+0x18, +0x04 p[1]+4, +0x06 p[2], +0x08 0x1000,
 * +0x0a *p+tbl.w0, +0x0c p[1]+tbl.w1, +0x0e byte(p+0x17), +0x0f 0, +0x10 1. */
typedef struct {
    uint16_t h0;
    uint16_t h2;
    uint16_t h4;
    uint16_t h6;
    uint16_t h8;
    uint16_t ha;
    uint16_t hc;
    uint8_t be;
    uint8_t bf;
    uint8_t b10;
} CardArg96e8c;

void CARDGAME_F0x80096e8c(int32_t a0, int32_t a1, int16_t *p)
{
    union {
        CardArg96e8c arg;
        CardCtx96e8c ctx;
        struct {
            uint8_t pad[0x18];
            CardCtx96e8c ctx2;
        } s;
    } u;
    CardTbl96e8c *tbl;
    int32_t r;
    int32_t adj;
    uint8_t p17;

    (void)a0;
    (void)a1;
    adj = 0;
    if (p[6] == 2 && ((uint8_t *)p)[0x16] == 1)
        adj = 0x45;
    p17 = ((uint8_t *)p)[0x17];
    if (p17 == 2) {
        if (p[6] == 3) {
            if (p[7] != 0) {
                u.arg.b10 = 1;
                u.arg.be = p17;
                u.arg.bf = 0;
                u.arg.h2 = p[0] + 0x18;
                u.arg.h4 = p[1] + 4;
                u.arg.h0 = ((uint16_t *)p)[8] & 0xf;
                u.arg.ha = p[0] + D0x800a5984[p[6]].w0;
                u.arg.hc = p[1] + D0x800a5984[p[6]].w1;
                u.arg.h6 = p[2];
                u.arg.h8 = 0x1000;
                CARDGAME_F0x80096a94(&u.arg, 1);
                F0x8001f648(&u.s.ctx2);
                (&u.s.ctx2)->cb7c(0x100, 1);
                (&u.s.ctx2)->cb74(0x280, 0);
                (&u.s.ctx2)->cb94(p[0] + D0x800a5984[p[6]].w0, p[1] + D0x800a5984[p[6]].w1);
                (&u.s.ctx2)->cb8c(p[2], 0x1000, 0x1000);
                r = ((ExeSlotFn)D0x80044f5c)(0x25d0002);
                (&u.s.ctx2)->cb84(r, ((((int32_t *)p)[4]) >> 4) * 3 + 0x1d, p[0] + 6, p[1] + 2);
            }
        }
    }
    tbl = D0x800a5984;
    if (tbl[p[6]].b5 == 2) {
        F0x8001f648(&u.ctx);
        (&u.ctx)->cb7c(0x100, 1);
        (&u.ctx)->cb74(0x340, 0);
        if (p[2] != 0x1000) {
            (&u.ctx)->cb94(p[0] + tbl[p[6]].w0, p[1] + tbl[p[6]].w1);
            (&u.ctx)->cb8c(p[2], 0x1000, 0x1000);
        }
        r = ((ExeSlotFn)D0x80044f5c)(0x25d0003);
    } else {
        F0x8001f648(&u.ctx);
        (&u.ctx)->cb7c(0x100, 1);
        (&u.ctx)->cb74(0x280, 0);
        if (p[2] != 0x1000) {
            (&u.ctx)->cb94(p[0] + tbl[p[6]].w0, p[1] + tbl[p[6]].w1);
            (&u.ctx)->cb8c(p[2], 0x1000, 0x1000);
        }
        r = ((ExeSlotFn)D0x80044f5c)(0x25d0002);
    }
    (&u.ctx)->cb84(r, tbl[p[6]].b4, p[0] + adj, p[1]);
}