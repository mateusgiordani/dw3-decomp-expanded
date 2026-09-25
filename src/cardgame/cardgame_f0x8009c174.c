#include "common/types.h"

extern void (*D_80055c48)(uint32_t);

void CARDGAME_F0x8009c174(uint8_t *ptr, uint16_t val) {
    D_80055c48(0x40019);
    *(uint16_t *)(ptr + 0xe02) = 12;
    *(uint16_t *)(ptr + 0xe00) = 12;
    *(uint16_t *)(ptr + 0xe04) = val;
    *(uint16_t *)(ptr + 0xe0a) = 1;
}
