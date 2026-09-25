/*
 * CARDGAME:0x800973e4 CARDGAME_F0x800973e4
 * 292 bytes at CARDGAME.PRO offset 0x14734 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800973e4
 *  Symbols     CARDGAME_F0x80097508=0x80097508 DAT_80044f4c=0x80044f4c
 *              func_0x8001fcc0=0x8001fcc0
 *  Compare     292 bytes from 0x800973e4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800973e4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue addiu sp,-0x30, saves s2/s1/s0/ra; epilogue jr
 * ra/addiu sp,+0x30.
 *
 * Prev function ends jr ra/addiu sp,+0x20 at 0x800973dc; next starts addiu
 * sp,-0x38 at 0x80097508.
 *
 * Layout: beq-to-null fall-into-body (body inline, null block out-of-line at
 * 0x800974dc).
 *
 * Selector *(p3+0x10): nonzero -> full sequence; zero -> (*(p4+0x144))(p4,0).
 *
 * Body: u=(*DAT_80044f4c)(p5); (*(p4+0x114))(p4,u,*(p3+0x10));
 * func_0x8001fcc0(sub at sp+0x10);
 * s1=(*fn14)(p4+0x5c,*(p4+0x50),(short)*(p4+0xb4)); (*(p4+0x138))(p4, sel==0x1f
 * ? 3 : 0); (*(p4+0x144))(p4,1);
 * (*(p4+0x134))(p4,(short)(0xa0-s1/2),(short)(*(ushort*)(p3+2)+4)).
 *
 * The 0x138 call is written per-branch (repeated table load) so the scheduler
 * places move a0,s0 in the bne delay slot and leaves the shared jalr delay as
 * nop (PAL-evident).
 *
 * Callee EXE 0x8001fcc0 verified (program EXE prologue addiu sp,-0x18); writes
 * buf slots +0/+4/+8.
 *
 * Frame 0x30 = saves 16 + vars 16 (init block at sp+0x10, fn at sp+0x14) +
 * outgoing args 16; slot contents beyond the three EXE-written words are
 * unobserved and kept as explicit pads.
 */

#include "common/types.h"

extern void func_0x8001fcc0(void *buf);
extern int32_t (*DAT_80044f4c)(int32_t arg);

typedef int32_t (*init_fn_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*tbl2_t)(int32_t a0, int32_t a1);
typedef void (*tbl3_t)(int32_t a0, int32_t a1, int32_t a2);

typedef struct {
    int32_t w10;
    init_fn_t fn14;
    int32_t w18;
    int32_t pad1c;
} init_blk_t;

void CARDGAME_F0x800973e4(int32_t p1, int32_t p2, int32_t p3, int32_t p4, int32_t p5)
{
    init_blk_t blk;
    int32_t u;
    int32_t s1;

    (void)p1;
    (void)p2;
    if (*(int32_t *)(p3 + 0x10) != 0) {
        u = DAT_80044f4c(p5);
        ((tbl3_t)*(int32_t *)(p4 + 0x114))(p4, u, *(int32_t *)(p3 + 0x10));
        func_0x8001fcc0(&blk.w10);
        s1 = blk.fn14(p4 + 0x5c, *(int32_t *)(p4 + 0x50), (int32_t)*(int16_t *)(p4 + 0xb4));
        if (*(int32_t *)(p3 + 0x10) == 0x1f) {
            ((tbl2_t)*(int32_t *)(p4 + 0x138))(p4, 3);
        } else {
            ((tbl2_t)*(int32_t *)(p4 + 0x138))(p4, 0);
        }
        ((tbl2_t)*(int32_t *)(p4 + 0x144))(p4, 1);
        ((tbl3_t)*(int32_t *)(p4 + 0x134))(p4, ((0xa0 - s1 / 2) << 16) >> 16, (((int32_t)*(uint16_t *)(p3 + 2) + 4) << 16) >> 16);
    } else {
        ((tbl2_t)*(int32_t *)(p4 + 0x144))(p4, 0);
    }
}
