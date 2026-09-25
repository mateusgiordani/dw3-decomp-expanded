#include "common/types.h"

typedef struct {
    char pad0[0x7c];
    uint32_t current;
    char pad80[0x14];
    uint32_t previous;
} STCRDSHP_85730;

void STCRDSHP_func_80085730(STCRDSHP_85730 *self, uint32_t *values) {
    STCRDSHP_85730 *entry;
    int32_t i = 0;

    entry = self;

    do {
        entry->previous = entry->current;
        entry->current = *values++;
        entry = (STCRDSHP_85730 *)((uint8_t *)entry + 4);
        i++;
    } while (i < 6);
    *(uint32_t *)((uint8_t *)self + 0x68) = 0;
    *(uint32_t *)((uint8_t *)self + 0x70) = 0;
    ((void (*)(void *, int32_t))(*(void (**)(void *))((uint8_t *)self + 0x28)))(self, 2);
}
