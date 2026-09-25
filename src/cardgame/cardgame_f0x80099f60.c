/* CARDGAME:0x80099f60; parent recovery from PAL disassembly and read-only Ghidra.
 * Original delivery remains preserved in worker commit b435f45bca8840da79aab0a3a9c67253c89f5439. */
#include "common/types.h"

extern void CARDGAME_F0x80099be4(uint8_t *, void *, int32_t, uint8_t *);
extern void CARDGAME_F0x80099440(uint8_t *, void *, int32_t);
extern void CARDGAME_F0x80098898(uint8_t *, void *, int32_t);
void CARDGAME_F0x80099f60(uint8_t *ctx, void *other)
{
    uint8_t *first;
    CARDGAME_F0x80099be4(ctx, other, 0, ctx + 0xa8);
    CARDGAME_F0x80099be4(ctx, other, 1, ctx + 0xfc);
    first = ctx + 0x60;
    CARDGAME_F0x80099440(first, other, 0);
    ctx += 0xb4;
    CARDGAME_F0x80099440(ctx, other, 1);
    CARDGAME_F0x80098898(first, other, 0);
    CARDGAME_F0x80098898(ctx, other, 1);
}
