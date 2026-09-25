#include "common/types.h"

extern uint32_t D_80048DA0;

void STCRDSHP_func_80088afc(void *self) {
    uint8_t *sub = *(uint8_t **)((uint8_t *)self + 0x24);
    uint8_t *object = *(uint8_t **)(sub + 0x10);

    ((void (*)(void *, int32_t, uint32_t))(*(void (**)(void))(object + 0x118)))
        (object, 0, D_80048DA0);
    object = *(uint8_t **)(sub + 0x10);
    ((void (*)(void *, int32_t))(*(void (**)(void))(object + 0x148)))(object, 1);
}
