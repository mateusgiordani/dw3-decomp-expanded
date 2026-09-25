/*
 * CARDGAME:0x8008dc70 CARDGAME_F0x8008dc70
 * 200 bytes at CARDGAME.PRO offset 0xafc0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008dc70
 *  Symbols     (none)
 *  Compare     200 bytes from 0x8008dc70 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008dc70
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Behaviour: entry = ctx + (side*57 + index*7)*2; the byte at entry+0x738
 * selects bank 0 (+0x5a6 count, +0x600 array) or bank 1 (+0x66e count, +0x6c8
 * array); the halfword at entry+0x72e is appended and the count incremented;
 * then the callback in slot other+0xea0 receives (other, bank, 6, new count).
 * Any other selector value returns.
 *
 * Source shape (revision 6, all measured with GCC 2.8.1 SN32 / ASPSX 2.79 O2):
 * - switch with default return gives PAL's beqz/beq/j-return chain; - one
 * callback call per case puts other in a0, the count in a3 and 6 in a2 inside
 * each case; the identical call tails merge into one; - the scaled count offset
 * is computed before adding ctx (PAL addu base,off); - index*7 is evaluated
 * before side*57 (PAL sum destination v0).
 */

#include <stdint.h>

typedef void (*cardgame_dc70_cb_t)(int32_t, int32_t, int32_t, int32_t);

void CARDGAME_F0x8008dc70(int32_t ctx, int32_t other, int32_t side, int32_t index)
{
    int32_t t;
    int16_t cnt;

    t = index * 7;
    side = ctx + (side * 57 + t) * 2;
    switch (*(uint8_t *)(side + 0x738)) {
    case 0:
        {
            int32_t k = *(int16_t *)(ctx + 0x5a6) * 2;
            *(uint16_t *)(ctx + k + 0x600) = *(uint16_t *)(side + 0x72e);
        }
        cnt = *(int16_t *)(ctx + 0x5a6) + 1;
        *(int16_t *)(ctx + 0x5a6) = cnt;
        ((cardgame_dc70_cb_t)*(uint32_t *)(other + 0xea0))(other, 0, 6, cnt);
        break;
    case 1:
        {
            int32_t k = *(int16_t *)(ctx + 0x66e) * 2;
            *(uint16_t *)(ctx + k + 0x6c8) = *(uint16_t *)(side + 0x72e);
        }
        cnt = *(int16_t *)(ctx + 0x66e) + 1;
        *(int16_t *)(ctx + 0x66e) = cnt;
        ((cardgame_dc70_cb_t)*(uint32_t *)(other + 0xea0))(other, 1, 6, cnt);
        break;
    }
}
