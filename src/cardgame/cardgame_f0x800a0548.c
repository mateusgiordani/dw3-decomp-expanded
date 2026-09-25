/* CARDGAME:0x800a0548; parent recovery from PAL disassembly and read-only Ghidra.
 * Original delivery remains preserved in worker commit 1a80e22775ac5a0af3b27b0e8fd872d513c2ae08. */
#include "common/types.h"

extern void CARDGAME_F0x8009edcc(uint8_t *);
typedef void (*CardCall814)(uint8_t *, uint8_t *, uint32_t, int32_t);
void CARDGAME_F0x800a0548(uint8_t *ctx)
{
    ctx[0x574] = 0;
    CARDGAME_F0x8009edcc(ctx);
    (*(CardCall814 *)(ctx + 0x814))(ctx, ctx + 0x600,
        (uint32_t)(int32_t)*(int16_t *)(ctx + 0x5a6) << 16, 0);
}
