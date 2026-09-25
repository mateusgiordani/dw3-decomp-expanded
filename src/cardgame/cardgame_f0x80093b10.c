/*
 * CARDGAME:0x80093b10 CARDGAME_F0x80093b10
 * 180 bytes at CARDGAME.PRO offset 0x10e60 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80093b10
 *  Symbols     CARDGAME_F0x8008daf4=0x8008daf4 CARDGAME_F0x80093b10=0x80093b10
 *  Compare     180 bytes from 0x80093b10 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80093b10
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * 180-byte PAL body at verified base 0x80082cb0.
 *
 * State dispatch (byte at +0x422: 1 = work, 2 = done/return 1), counters at
 * +0x424/+0x428, byte slot array at +0x72c with 114-byte stride.
 *
 * Single direct callee CARDGAME_F0x8008daf4(ctx, other, side, count).
 *
 * Switch layout reproduces the PAL jump-over-work shape; the named stride local
 * preserves the original address-add operand order.
 */

#include <stdint.h>

extern int32_t CARDGAME_F0x8008daf4(void *ctx, void *other, int32_t side, int32_t index);

int32_t CARDGAME_F0x80093b10(void *ctx, void *other, int32_t side)
{
    char *pb = (char *)ctx;
    uint8_t state = *(uint8_t *)(pb + 0x422);
    int32_t result = 0;

    switch (state) {
    case 1:
        if (CARDGAME_F0x8008daf4(ctx, other, side, *(int32_t *)(pb + 0x428)) != 0) {
            int32_t count = *(int32_t *)(pb + 0x428);
            int32_t scaled;
            *(int32_t *)(pb + 0x424) = 0;
            count -= 1;
            *(int32_t *)(pb + 0x428) = count;
            if (count < 0) {
                *(uint8_t *)(pb + 0x422) = 2;
                scaled = side * 114;
                *(uint8_t *)(pb + scaled + 0x72c) = 0;
            }
        }
        break;
    case 2:
        result = 1;
        break;
    }
    return result;
}
