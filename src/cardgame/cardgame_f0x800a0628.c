/*
 * CARDGAME:0x800a0628 CARDGAME_F0x800a0628
 * 224 bytes at CARDGAME.PRO offset 0x1d978 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a0628
 *  Symbols     CARDGAME_F0x800a0628=0x800a0628 F0x8001ebf8=0x8001ebf8
 *  Compare     224 bytes from 0x800a0628 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a0628
 */

#include <stdint.h>

extern void F0x8001ebf8(void *buf);

void CARDGAME_F0x800a0628(uint8_t *a0, int32_t a1, int32_t a2, uint32_t a3)
{
    struct {
        uint8_t *p;
        int32_t w[10];
        void (*fn)(int32_t);
        int32_t tail[10];
    } tmp;
    int32_t addr;
    int32_t t;
    uint8_t *entry;
    uint8_t *group;
    uint8_t cnt;

    addr = a1 * 0x72 + 0x72c;
    group = a0 + addr;
    F0x8001ebf8(&tmp);
    t = a2 * 2;
    tmp.fn((int32_t)(*(int16_t *)(a0 + t + 0x50) + 1));
    entry = group + a3 * 0xe;
    *(uint16_t *)(entry + 8) = tmp.p[1];
    *(uint16_t *)(entry + 10) = tmp.p[2];
    *(uint16_t *)(entry + 4) = 0;
    *(uint16_t *)(entry + 6) = 0;
    *(int16_t *)(entry + 2) = (int16_t)a2;
    entry[13] = (uint8_t)a1;
    entry[12] = (uint8_t)a1;
    cnt = a0[0x308];
    a0[0x308] = (uint8_t)(cnt + 1);
    entry[14] = cnt;
}
