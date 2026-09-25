/*
 * STCRDSHP:0x80085fdc STCRDSHP_func_80085fdc
 * 152 bytes at STCRDSHP.PRO offset 0x332c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80085fdc
 *  Symbols     EXE_F0x80014504=0x80014504 STCRDSHP_func_80085730=0x80085730
 *              STCRDSHP_func_80085788=0x80085788
 *              STCRDSHP_func_80085da0=0x80085da0
 *              STCRDSHP_func_80085fdc=0x80085fdc
 *  Compare     152 bytes from 0x80085fdc against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80085fdc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * 152-byte PAL body. Initialize the loop counter before the cursor and
 * increment it before loading each copied word, as in PAL.
 */

#include "common/types.h"

extern void STCRDSHP_func_80085da0(void);
extern void STCRDSHP_func_80085730(void);
extern void STCRDSHP_func_80085788(void);
extern void *EXE_F0x80014504(void *, int32_t, int32_t);

void *STCRDSHP_func_80085fdc(void *callback, uint32_t *values) {
    uint8_t *object;
    uint8_t *entry;
    int32_t i;

    object = EXE_F0x80014504(&STCRDSHP_func_80085da0, 0xc4, 0);
    i = 0;
    entry = object;
    *(void (**)(void))(object + 0xbc) = STCRDSHP_func_80085730;
    *(void (**)(void))(object + 0xc0) = STCRDSHP_func_80085788;
    *(int32_t *)(object + 0x54) = 0x1000;
    *(int32_t *)(object + 0x58) = 6;
    *(void **)(object + 0x50) = callback;
    do {
        i++;
        *(uint32_t *)(entry + 0x7c) = *values++;
        entry += 4;
    } while (i < 6);
    return object;
}
