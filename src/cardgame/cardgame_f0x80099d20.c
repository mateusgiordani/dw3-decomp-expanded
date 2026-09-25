/* CARDGAME:0x80099d18, 208 bytes, PAL-SLES-03936.
 * The callback load at 0x80099d18/1c precedes the prologue at 0x80099d20.
 * The predecessor returns at 0x80099d10 (delay 0x80099d14); the direct caller
 * at 0x80099f1c targets 0x80099d18. The former 200-byte window is internal.
 * Source filename retained for revision provenance; definition uses true entry.
 * Per-axis divisor/quotient reuse preserves PAL loads with the pinned GCC
 * 2.8.1 / ASPSX 2.79 O2 and strip-div-guard setting; see strategy-r7.md and
 * boundary-review-r8.md. This is a measured source shape, not a general CSE rule.
 */
#include "common/types.h"

typedef uint8_t (*cardgame_tick_fn)(void);
extern cardgame_tick_fn DAT_8004df9c;

void CARDGAME_F0x80099d18(uint8_t *work)
{
    /* PAL control flow branches (beq) to the snap path and falls through
       into the interpolation, i.e. the source tests nonzero first. */
    if ((work[0xe] -= DAT_8004df9c()) != 0) {
        int qx;
        int qy;

        qx = work[0xf];
        qx = (*(int16_t *)(work + 8) - *(int16_t *)(work + 4)) * work[0xe] / qx;
        qy = work[0xf];
        qy = (*(int16_t *)(work + 10) - *(int16_t *)(work + 6)) * work[0xe] / qy;
        *(int16_t *)(work + 0) = *(uint16_t *)(work + 8) - qx;
        *(int16_t *)(work + 2) = *(uint16_t *)(work + 10) - qy;
    } else {
        work[0x10] = 1;
        work[0xf] = 0;
        work[0xe] = 0;
        *(int16_t *)(work + 0) = *(uint16_t *)(work + 8);
        *(int16_t *)(work + 2) = *(uint16_t *)(work + 10);
    }
}
