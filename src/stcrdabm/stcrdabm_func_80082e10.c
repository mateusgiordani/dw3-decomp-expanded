/*
 * STCRDABM:0x80082e10 STCRDABM_func_80082e10
 * 324 bytes at STCRDABM.PRO offset 0x160 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80082e10
 *  Symbols     D_8004DE10=0x8004de10
 *  Compare     324 bytes from 0x80082e10 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80082e10
 */

#include "common/types.h"

extern int32_t D_8004DE10[];

void STCRDABM_func_80082e10(void *object) {
    uint32_t *source;
    uint32_t *packet;
    int32_t resource;
    /* PAL uses sra for >>8: signed value */
    int32_t value;
    uint32_t mask;
    uint32_t low;
    
    uint8_t b;
    /* PAL: lui $s2,0x8005; addiu $s2,$s2,-0x21f0; shared by all three loads */
    int32_t *tbl = D_8004DE10;

    resource = (*(int32_t (**)(int32_t))(tbl + 0x5f))
        (*(int32_t *)((char *)object + 0x50));
    source = (uint32_t *)(uintptr_t)(*(uint32_t (**)(int32_t, int32_t))
        (uintptr_t)(resource + 0x138))(resource,
        *(int32_t *)((char *)object + 0x54));
    packet = (uint32_t *)(uintptr_t)(*(uint32_t (**)(void))(tbl + 0x56))();

    low = 0x00ffffff;
    b = 5;
    *((uint8_t *)packet + 3) = b;
    b = 0x2a;
    *((uint8_t *)packet + 7) = b;
    value = *(int32_t *)((char *)object + 0x5c);
    *((uint16_t *)((char *)packet + 0x14)) = 0x140;
    *((uint16_t *)((char *)packet + 0x0c)) = 0x140;
    *((uint16_t *)((char *)packet + 0x10)) = 0;
    *((uint16_t *)((char *)packet + 0x08)) = 0;
    *((uint16_t *)((char *)packet + 0x0e)) = 0;
    *((uint16_t *)((char *)packet + 0x0a)) = 0;
    *((uint16_t *)((char *)packet + 0x16)) = 0x100;
    *((uint16_t *)((char *)packet + 0x12)) = 0x100;
    *((uint8_t *)packet + 6) = (uint8_t)(value >> 8);
    *((uint8_t *)packet + 5) = (uint8_t)(value >> 8);
    *((uint8_t *)packet + 4) = (uint8_t)(value >> 8);
    mask = 0xff000000;
    *packet = (*packet & mask) | (*source & low);
    *source = (*source & mask) | ((uint32_t)(uintptr_t)packet & low);
    /* PAL: addiu $a1,$v0,0x18 shared tail cursor for the second half */
    {
        uint32_t *tail = packet + 6;
        *((uint8_t *)tail + 3) = 1;
        tail[1] = 0xe1000245;
        tail[0] = (tail[0] & mask) | (*source & low);
        *source = (*source & mask) | ((uint32_t)(uintptr_t)tail & low);
        (*(void (**)(void *))(tbl + 0x57))(tail + 2);
    }
}
