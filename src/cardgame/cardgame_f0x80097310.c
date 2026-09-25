/*
 * CARDGAME:0x80097310 CARDGAME_F0x80097310
 * 212 bytes at CARDGAME.PRO offset 0x14660 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80097310
 *  Symbols     D0x800a59a8=0x800a59a8 DAT_80044f4c=0x80044f4c
 *  Compare     212 bytes from 0x80097310 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80097310
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue addiu sp,-0x20, saves s0/s1/s2/ra at 0x10-0x1c;
 * epilogue lw ra/s2/s1/s0; jr ra/addiu sp,+0x20 at 0x800973cc-0x800973e0.
 *
 * Prev CARDGAME:0x80097238 ends jr ra/addiu sp,+0x30 at 0x80097308/0x8009730c;
 * next CARDGAME:0x800973e4 starts addiu sp,-0x30 at 0x800973e4. Size 0xD4
 * contiguous.
 *
 * Layout: beq-to-null fall-into-body (body inline, null block out-of-line at
 * 0x800973b8).
 *
 * Selector *(p2+0x10): nonzero -> full sequence; zero -> (*(p3+0x144))(p3,0).
 *
 * Body:
 * (*(p3+0x134))(p3,(short)(*(ushort*)p2+D0x800a59a8[p5]),(short)(*(ushort*)(p2+2)+4));
 * u=(*DAT_80044f4c)(p4); (*(p3+0x114))(p3,u,*(p2+0x10)).
 *
 * Callers: CARDGAME_F0x80097508 jal at 0x800976d0/0x80097768/0x800977c0 (5
 * args: a0 dead, a1 selector struct, a2 table object, a3 DAT arg, stack flag
 * index).
 */

#include "common/types.h"

extern int32_t (*DAT_80044f4c)(int32_t arg);
extern uint16_t D0x800a59a8[];

typedef void (*tbl2_t)(int32_t a0, int32_t a1);
typedef void (*tbl3_t)(int32_t a0, int32_t a1, int32_t a2);

void CARDGAME_F0x80097310(int32_t p1, int32_t p2, int32_t p3, int32_t p4, int32_t p5)
{
    int32_t u;

    (void)p1;
    if (*(int32_t *)(p2 + 0x10) != 0) {
        ((tbl3_t)*(int32_t *)(p3 + 0x134))(p3, (((int32_t)*(uint16_t *)p2 + (int32_t)D0x800a59a8[p5]) << 16) >> 16, (((int32_t)*(uint16_t *)(p2 + 2) + 4) << 16) >> 16);
        u = DAT_80044f4c(p4);
        ((tbl3_t)*(int32_t *)(p3 + 0x114))(p3, u, *(int32_t *)(p2 + 0x10));
    } else {
        ((tbl2_t)*(int32_t *)(p3 + 0x144))(p3, 0);
    }
}
