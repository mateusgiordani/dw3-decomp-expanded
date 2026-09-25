#include <stdint.h>

extern void CARDGAME_F0x8009b890(int32_t a0, int32_t a1, int32_t a2);
extern void CARDGAME_F0x8009b76c(int32_t a0, int32_t a1);
extern void CARDGAME_F0x8009ba3c(int32_t a0, int32_t a1);

void CARDGAME_F0x8009bac8(int32_t ctx, int32_t arg)
{
    int32_t off;
    int32_t i;
    int32_t pass;
    int32_t row;

    for (i = 0x27, off = 0xc9c; i >= 0; i--, off -= 0x4c) {
        CARDGAME_F0x8009b890(ctx, arg, ctx + off);
        CARDGAME_F0x8009b76c(ctx, ctx + off);
    }
    for (pass = 0; pass < 2; pass++) {
        for (i = 0x27, off = 0xc9c, row = ctx + 0xb94; ; row -= 0x4c) {
            if ((pass == 0) ? (*(int16_t *)(row + 0x12e) != 0) : (*(int16_t *)(row + 0x12e) == 0)) {
                CARDGAME_F0x8009ba3c(ctx, ctx + off);
                off -= 0x4c;
                i--;
                if (i < 0) break;
            } else {
                off -= 0x4c;
                i--;
                if (i < 0) break;
            }
        }
    }
}
