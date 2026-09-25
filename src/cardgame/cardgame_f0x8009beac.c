#include "common/types.h"

extern void (*D_80055c48)(uint32_t);

void CARDGAME_F0x8009beac(uint8_t *ptr, int32_t idx, uint16_t arg2, uint32_t arg3, uint16_t arg4, uint16_t arg5) {
    D_80055c48(0x40019);
    ptr += (idx * 3) << 3;
    *(uint16_t *)(ptr + 0xe0c) = arg4;
    *(uint16_t *)(ptr + 0xe0e) = arg5;
    *(uint16_t *)(ptr + 0xe12) = 0x1000;
    *(uint8_t *)(ptr + 0xe23) = 1;
    *(uint16_t *)(ptr + 0xe10) = 0;
    *(uint16_t *)(ptr + 0xe18) = arg2;
    *(uint32_t *)(ptr + 0xe1c) = arg3;
    *(uint16_t *)(ptr + 0xe16) = 12;
    *(uint16_t *)(ptr + 0xe14) = 12;
}
