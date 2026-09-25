/* CARDGAME:0x8009c1cc; parent recovery from PAL disassembly and read-only Ghidra.
 * Original delivery remains preserved in worker commit 4dd5402419861832977644d605c52e525694147e. */
#include "common/types.h"

extern void (*D_80055c48)(uint32_t);
void CARDGAME_F0x8009c1cc(uint8_t *ctx)
{
    D_80055c48(0x4001a);
    *(uint16_t *)(ctx + 0xe02) = 6;
    *(uint16_t *)(ctx + 0xe00) = 6;
    *(uint16_t *)(ctx + 0xe0a) = 5;
}
