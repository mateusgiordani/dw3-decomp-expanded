/*
 * STCRDSHP:0x80085788 STCRDSHP_func_80085788
 * 40 bytes at STCRDSHP.PRO offset 0x2ad8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80085788
 *  Symbols     (none)
 *  Compare     40 bytes from 0x80085788 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80085788
 */

#include "common/types.h"

typedef struct {
    char pad0[0x2c];
    void (*callback)(void *, int);
} STCRDSHP_85788;

void STCRDSHP_func_80085788(STCRDSHP_85788 *self) {
    self->callback(self, 1);
}
