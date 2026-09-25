/* CARDGAME:0x8009ba3c; parent recovery from PAL disassembly and read-only Ghidra.
 * Original delivery remains preserved in worker commit 21951aa3d2d8afdbe8bf1408b01cd51952d40af6. */
#include "common/types.h"

extern void CARDGAME_F0x8009acac(void *, uint8_t *);
extern void CARDGAME_F0x8009b028(void *, uint8_t *);
extern void CARDGAME_F0x8009af28(void *, uint8_t *);
extern void CARDGAME_F0x8009a998(void *, uint8_t *);
extern void CARDGAME_F0x8009b678(void *, uint8_t *);
extern void CARDGAME_F0x8009b168(void *, uint8_t *);
void CARDGAME_F0x8009ba3c(void *ctx, uint8_t *row)
{
    if (*(int16_t *)(row + 0x18) != 0 && *(int16_t *)(row + 0x1a) != 0) {
        CARDGAME_F0x8009acac(ctx, row);
        CARDGAME_F0x8009b028(ctx, row);
        CARDGAME_F0x8009af28(ctx, row);
        CARDGAME_F0x8009a998(ctx, row);
        CARDGAME_F0x8009b678(ctx, row);
        CARDGAME_F0x8009b168(ctx, row);
    }
}
