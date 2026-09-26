/*
 * CARDGAME:0x800a32d8 CARDGAME_F0x800a32d8
 * 432 bytes at CARDGAME.PRO offset 0x20628 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a32d8
 *  Symbols     D_8004DE10=0x8004de10
 *  Compare     432 bytes from 0x800a32d8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a32d8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x20 (s0-s2, ra).
 *
 * Draws a semi-transparent flat quad (GPU code 0x2a, 5 words) at pos with size,
 * coloured from p1+0x64..0x66, followed by a draw-mode packet (0xe1,
 * transparency mode from p1+0x5c) and links both into the ordering table
 * returned by the EXE vector at 0x8004df8c (+0x138 method). Primitive memory
 * comes from 0x8004df68 and the next free pointer is published to 0x8004df6c.
 *
 * Matching note: the links use the PsyQ addPrim/P_TAG bitfield macros; the same
 * operation written with explicit masks swaps the two mask registers.
 */

#include <stdint.h>

typedef struct {
    int16_t x;
    int16_t y;
} Vec2;

typedef int32_t (*fn0_t)(void);
typedef int32_t (*fn1_t)(int32_t);
typedef uint32_t *(*ot_t)(int32_t, int32_t);
typedef void (*pub_t)(uint8_t *);

extern int32_t D_8004DE10[];

typedef struct {
    uint32_t addr : 24;
    uint32_t len : 8;
} P_TAG;

#define setaddr(p, a)  (((P_TAG *)(p))->addr = (uint32_t)(a))
#define getaddr(p)     ((uint32_t)((P_TAG *)(p))->addr)
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define U32(p, o)    (*(uint32_t *)((p) + (o)))

void CARDGAME_F0x800a32d8(int32_t p1, Vec2 pos, Vec2 size)
{
    int32_t obj;
    uint32_t *ot;
    uint8_t *prim;

    obj = ((fn1_t)D_8004DE10[0x17c / 4])(0x100);
    ot = ((ot_t)*(int32_t *)(obj + 0x138))(obj, 0);
    prim = (uint8_t *)((fn0_t)D_8004DE10[0x158 / 4])();
    prim[4] = U8(p1, 0x64);
    prim[5] = U8(p1, 0x65);
    prim[6] = U8(p1, 0x66);
    prim[3] = 5;
    prim[7] = 0x2a;
    S16(prim, 0x8) = pos.x;
    S16(prim, 0xc) = pos.x + size.x;
    S16(prim, 0x10) = pos.x;
    S16(prim, 0x14) = pos.x + size.x;
    S16(prim, 0xa) = pos.y;
    S16(prim, 0xe) = pos.y;
    S16(prim, 0x12) = pos.y + size.y;
    S16(prim, 0x16) = pos.y + size.y;
    addPrim(ot, prim);
    {
        uint8_t *mode = prim + 0x18;

        mode[3] = 1;
        U32(mode, 4) = ((U8(p1, 0x5c) & 3) << 5) | 0xe1000205;
        addPrim(ot, mode);
    }
    ((pub_t)D_8004DE10[0x15c / 4])(prim + 0x20);
}
