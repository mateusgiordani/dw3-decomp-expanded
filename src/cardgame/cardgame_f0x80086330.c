/*
 * CARDGAME:0x80086330 CARDGAME_F0x80086330
 * 252 bytes at CARDGAME.PRO offset 0x3680 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80086330
 *  Symbols     CARDGAME_F0x80085fd0=0x80085fd0 CARDGAME_F0x80086330=0x80086330
 *  Compare     252 bytes from 0x80086330 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80086330
 */

#include <stdint.h>

extern unsigned int CARDGAME_F0x80085fd0(void *p, unsigned char *k, int idx);

typedef void (*CardCb6330)(void *);

void CARDGAME_F0x80086330(void *p, void *q, int tag) {
    int i;
    int found;
    char *cur;

    *(unsigned int *)((char *)p + 0x440) = 0;
    *(unsigned char *)((char *)p + 0x422) = 0;
    if (*(short *)((char *)q + 0x64) == 0) {
        *(unsigned char *)((char *)p + 0x49d) = 1;
        *(unsigned char *)((char *)p + 0x499) = 1;
        (*(CardCb6330 *)((char *)q + 0xec8))(q);
    }
    i = 0;
    *(unsigned int *)((char *)p + 0x424) = 0;
    *(unsigned int *)((char *)q + 0xde8) = (unsigned int)tag;
    *(unsigned int *)((char *)q + 0xde4) = 0;
    found = 0;
    if (*(short *)((char *)p + 0x5a6) > found) {
        cur = (char *)p;
        do {
            if (CARDGAME_F0x80085fd0(p, (unsigned char *)p + 0x5a8, *(short *)(cur + 0x600)) != 0) {
                found = 1;
                break;
            }
            i++;
            cur += 2;
        } while (i < *(short *)((char *)p + 0x5a6));
    }
    if (found != 0) {
        *(short *)((char *)q + 0xdf4) = 0;
        *(unsigned int *)((char *)p + 0x440) = 0;
        *(unsigned int *)((char *)p + 0x438) = 2;
    } else {
        *(short *)((char *)q + 0xdf4) = 1;
        *(unsigned int *)((char *)p + 0x440) = 1;
        *(unsigned int *)((char *)p + 0x438) = 3;
    }
}
