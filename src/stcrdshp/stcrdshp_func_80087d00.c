/*
 * STCRDSHP:0x80087d00 STCRDSHP_func_80087d00
 * 396 bytes at STCRDSHP.PRO offset 0x5050 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80087d00
 *  Symbols     F0x8001ae38=0x8001ae38 F0x8001b364=0x8001b364
 *              STCRDSHP_func_80088b50=0x80088b50
 *  Compare     396 bytes from 0x80087d00 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80087d00
 */

#include "common/types.h"

/*
 * Caller STCRDSHP_func_80088b50
 *
 * @ 0x80088be8 passa (object, out-buffer).
 */

extern void *F0x8001ae38(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
extern void *F0x8001b364(int32_t a0, int32_t a1, int32_t a2, int32_t a3);

void STCRDSHP_func_80087d00(uint8_t *a0, void **a1)
{
    void **s0;
    void *elem;
    void *obj;
    int32_t s3;
    int32_t base;
    int32_t cur;

    a1[0] = F0x8001ae38((int32_t)*(int16_t *)(a0 + 0x50), 1, 0x1d, 0x16);
    s3 = 0;
    base = 0x350000;
    a1[1] = F0x8001ae38((int32_t)*(int16_t *)(a0 + 0x50), 1, 0xd3, 0xcc);
    a1[4] = F0x8001ae38((int32_t)*(int16_t *)(a0 + 0x50), 3, 0x117, 0x1c);
    a1[3] = F0x8001ae38((int32_t)*(int16_t *)(a0 + 0x50), 3, 0x11a, 0x1c);
    s0 = a1;
loop1:
    cur = base >> 16;
    obj = F0x8001ae38((int32_t)*(int16_t *)(a0 + 0x50), 1, 0xa7, cur);
    s0[5] = obj;
    s0 += 1;
    s3 += 1;
    base += 0xe0000;
    if (s3 < 3) {
        goto loop1;
    }
    do {
    } while (0);
    base = 0;
    obj = F0x8001b364((int32_t)*(int16_t *)(a0 + 0x50),
        *(int32_t *)(a0 + 0x54) - 2, 0xa7,
        ((*(int32_t *)(a0 + 0x6c) * 14 + 0x35) << 16) >> 16);
    a1[8] = obj;
    ((void (*)(void *, int32_t)) *(void **)((char *)obj + 0x80))(obj, base);
    a1[2] = F0x8001ae38((int32_t)*(int16_t *)(a0 + 0x50), 1, 0x9a, 0x71);
    s0 = *(void ***)(a0 + 0x24);
    if (base < *(int32_t *)(a0 + 0x20) - 3) {
        do {
            base += 1;
            elem = *s0;
            ((void (*)(void *, int32_t)) *(void **)((char *)elem + 0x15c))
                (elem, *(int32_t *)(a0 + 0x54) - 2);
            s0 += 1;
        } while (base < *(int32_t *)(a0 + 0x20) - 3);
    }
}
