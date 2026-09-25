/*
 * CARDGAME:0x8008deac CARDGAME_F0x8008deac
 * 76 bytes at CARDGAME.PRO offset 0xb1fc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008deac
 *  Symbols     D_8004df9c=0x8004df9c
 *  Compare     76 bytes from 0x8008deac against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008deac
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Direct PAL callers target the global callback load eight bytes before the
 * former framed entry.
 *
 * The callback is loaded from 0x8004df9c; no incoming-v0 ABI.
 */

#include "common/types.h"

typedef struct {
    uint8_t pad_0x000[0x424];
    int32_t field_0x424;
} CardGameCtx_8008deac;

typedef int (*CallbackFn)(void);
extern CallbackFn D_8004df9c;

int CARDGAME_F0x8008deac(CardGameCtx_8008deac *ctx, int param_1_unused, int limit) {
    int v0;
    int v1;
    v0 = D_8004df9c();
    v1 = ctx->field_0x424 + v0;
    ctx->field_0x424 = v1;
    return (limit < v1) ? 1 : 0;
}
