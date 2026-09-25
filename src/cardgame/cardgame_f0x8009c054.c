/*
 * CARDGAME:0x8009c054 CARDGAME_F0x8009c054
 * 136 bytes at CARDGAME.PRO offset 0x193a4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009c054
 *  Symbols     CARDGAME_F0x8009d310=0x8009d310 D_80055c48=0x80055c48
 *              _DAT_80055c48=0x80055c48
 *  Compare     136 bytes from 0x8009c054 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009c054
 */

#include "common/types.h"

/*
 * Next function CARDGAME:0x8009c0dc at +0x88 confirms size 0x88.
 *
 * Signature is conservative: scalar widths taken from store widths (sw/sh/sb),
 * not from guessed struct; offsets left raw (no invented struct).
 *
 * Indirect callee at *0x80055c48 is an EXE global function pointer shared with
 * siblings 0x8009c0dc/0x8009c124/0x8009c174/0x8009c1cc/0x8009c214.
 */

extern void (*D_80055c48)(int32_t);

void CARDGAME_F0x8009c054(uint8_t *base, int32_t a1, int32_t a2, int32_t a3, int32_t a4) {
    D_80055c48(0x40019);
    *(int16_t *)(base + 0xDF2) = 12;
    *(int16_t *)(base + 0xDF0) = 12;
    *(int16_t *)(base + 0xDF4) = (int16_t)a3;
    *(int32_t *)(base + 0xDE8) = a1;
    *(base + 0xDFB) = (uint8_t)a2;
    *(int32_t *)(base + 0xDE4) = a4;
    *(base + 0xDFA) = 1;
}
