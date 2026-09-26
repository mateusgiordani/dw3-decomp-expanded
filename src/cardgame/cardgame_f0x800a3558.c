/*
 * CARDGAME:0x800a3558 CARDGAME_F0x800a3558
 * 76 bytes at CARDGAME.PRO offset 0x208a8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a3558
 *  Symbols     (none)
 *  Compare     76 bytes from 0x800a3558 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a3558
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Starts a colour fade on the fade object: the current colour (+0x64) becomes
 * the target (+0x61), (r, g, b) the start colour (+0x5e), both timers
 * (+0x54/+0x58) the duration, state +0x50 = 1 and +0x5d the `keep` flag.
 *
 * Matching note: the object is accessed through a struct, so its stores do not
 * alias the stack argument `keep` (MEM_IN_STRUCT_P), whose load sinks to the
 * end as in PAL.
 */

#include <stdint.h>

typedef struct {
    uint8_t pad0[0x50];
    int32_t state;
    int32_t timer;
    int32_t duration;
    uint8_t pad5c;
    uint8_t keep;
    uint8_t start[3];
    uint8_t target[3];
    uint8_t cur[3];
} Fade;

void CARDGAME_F0x800a3558(Fade *f, int32_t r, int32_t g, int32_t b, int32_t duration, int32_t keep)
{
    int32_t i;

    for (i = 0; i < 3; i++)
        f->target[i] = f->cur[i];
    f->start[0] = r;
    f->start[1] = g;
    f->start[2] = b;
    f->timer = duration;
    f->duration = duration;
    f->state = 1;
    f->keep = keep;
}
