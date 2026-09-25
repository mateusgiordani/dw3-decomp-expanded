/* CARDGAME:0x80093b10 - 180-byte PAL body at verified base 0x80082cb0.
 * State dispatch (byte at +0x422: 1 = work, 2 = done/return 1), counters at
 * +0x424/+0x428, byte slot array at +0x72c with 114-byte stride.
 * Single direct callee CARDGAME_F0x8008daf4(ctx, other, side, count).
 * Switch layout reproduces the PAL jump-over-work shape; the named stride
 * local preserves the original address-add operand order.
 * Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
 * Status: exact_byte_match (see handoff). */
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
