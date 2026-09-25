/* CARDGAME:0x8008dc70 (200B, 0xc8).
 * PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xafc0.
 * Boundary: prev CARDGAME:0x8008db7c ends exactly at 0x8008dc70; next prologue
 *   27bdffd8 (addiu sp,-0x28) at 0x8008dd38 = +0xc8 (contiguous, no gap).
 * Ghidra ddw3-pal-sles-03936/CARDGAME read-only: 50 insns match PAL; one
 *   caller CARDGAME_F0x8008dd38 @ 0x8008dd90 (jal, return ignored).
 * Behaviour: entry = ctx + (side*57 + index*7)*2; the byte at entry+0x738
 *   selects bank 0 (+0x5a6 count, +0x600 array) or bank 1 (+0x66e count,
 *   +0x6c8 array); the halfword at entry+0x72e is appended and the count
 *   incremented; then the callback in slot other+0xea0 receives
 *   (other, bank, 6, new count). Any other selector value returns.
 * Source shape (revision 6, all measured with GCC 2.8.1 SN32 / ASPSX 2.79 O2):
 *   - switch with default return gives PAL's beqz/beq/j-return chain;
 *   - one callback call per case puts other in a0, the count in a3 and 6 in
 *     a2 inside each case; the identical call tails merge into one;
 *   - the scaled count offset is computed before adding ctx (PAL addu base,off);
 *   - index*7 is evaluated before side*57 (PAL sum destination v0). */
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
