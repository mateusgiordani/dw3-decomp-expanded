/*
 * CARDGAME:0x800a1a5c CARDGAME_F0x800a1a5c
 * 156 bytes at CARDGAME.PRO offset 0x1edac (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a1a5c
 *  Symbols     CARDGAME_F0x8009d9e4=0x8009d9e4 CARDGAME_F0x8009da68=0x8009da68
 *              CARDGAME_F0x8009edcc=0x8009edcc CARDGAME_F0x800a19f8=0x800a19f8
 *  Compare     156 bytes from 0x800a1a5c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a1a5c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x20 (s0-s2, ra).
 *
 * Turn reset: updates the record at p1+0x59c (CARDGAME_F0x800a19f8), runs
 * CARDGAME_F0x8009da68, reports the record's +4/+8 values through the method at
 * p1+0x810, clears the per-turn fields, runs CARDGAME_F0x8009edcc, bumps the
 * turn counter p1[0x300] and runs CARDGAME_F0x8009d9e4.
 */

#include <stdint.h>

typedef void (*report_t)(int32_t, int32_t, int32_t);

void CARDGAME_F0x800a19f8(int32_t rec);
void CARDGAME_F0x8009da68(int32_t p1, int32_t p2);
void CARDGAME_F0x8009edcc(int32_t p1, int32_t p2);
void CARDGAME_F0x8009d9e4(int32_t p1, int32_t p2);

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S16(p, o)    (*(int16_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

void CARDGAME_F0x800a1a5c(int32_t p1, int32_t p2)
{
    int32_t rec;

    rec = p1 + 0x59c;
    CARDGAME_F0x800a19f8(rec);
    CARDGAME_F0x8009da68(p1, p2);
    ((report_t)S32(p1, 0x810))(p1, S16(rec, 4), S16(rec, 8));
    U8(p1, 0x304) = 0;
    U8(p1, 0x305) = 0;
    S16(p1, 0x59c) = 0;
    S16(p1, 0x59e) = 0;
    S16(p1, 0x664) = 0;
    S16(p1, 0x666) = 0;
    CARDGAME_F0x8009edcc(p1, p2);
    U8(p1, 0x300)++;
    CARDGAME_F0x8009d9e4(p1, p2);
}
