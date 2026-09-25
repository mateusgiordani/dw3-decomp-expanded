/* CARDGAME:0x8008daf4; parent recovery from PAL disassembly and read-only Ghidra.
 * Original delivery remains preserved in worker commit 32013dea50ca56abe6a17683ea2e7e808d38c14a. */
#include "common/types.h"

extern int32_t CARDGAME_F0x8008d938(void *, void *, int32_t);
extern void CARDGAME_F0x8008da2c(void *, void *, int32_t, int32_t);
int32_t CARDGAME_F0x8008daf4(void *ctx, void *other, int32_t side, int32_t index)
{
    /* Preserve PAL initialization order before preparing the adjusted index. */
    int32_t result = 0;
    int32_t adjusted = index;
    if (side != 0)
        adjusted += 6;
    if (CARDGAME_F0x8008d938(ctx, other, adjusted) != 0) {
        CARDGAME_F0x8008da2c(ctx, other, side, index);
        result = 1;
    }
    return result;
}
