/*
 * STCRDSHP:0x80085730 STCRDSHP_func_80085730
 * 88 bytes at STCRDSHP.PRO offset 0x2a80 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x80085730
 *  Symbols     (none)
 *  Compare     88 bytes from 0x80085730 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80085730
 */

#include "common/types.h"

typedef struct {
    char pad0[0x7c];
    uint32_t current;
    char pad80[0x14];
    uint32_t previous;
} STCRDSHP_85730;

void STCRDSHP_func_80085730(STCRDSHP_85730 *self, uint32_t *values) {
    STCRDSHP_85730 *entry;
    int32_t i = 0;

    entry = self;

    do {
        entry->previous = entry->current;
        entry->current = *values++;
        entry = (STCRDSHP_85730 *)((uint8_t *)entry + 4);
        i++;
    } while (i < 6);
    *(uint32_t *)((uint8_t *)self + 0x68) = 0;
    *(uint32_t *)((uint8_t *)self + 0x70) = 0;
    ((void (*)(void *, int32_t))(*(void (**)(void *))((uint8_t *)self + 0x28)))(self, 2);
}
