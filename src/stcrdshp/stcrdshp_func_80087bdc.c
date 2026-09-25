#include "common/types.h"

extern uint32_t D_8004DE10[];
extern void *STCRDSHP_func_80088ca0(void);

typedef struct {
    uint8_t pad[0xc];
    int32_t state;
    uint8_t pad2[0x28];
    void (*notify)(void *self);
} STCRDSHP_87bdc_ctx;

void STCRDSHP_func_80087bdc(STCRDSHP_87bdc_ctx *ctx, void **out) {
    uint8_t *object;
    int16_t rect[4];

    if (ctx->state == 0)
        goto init;
    if (ctx->state < 0)
        goto init;
    if (ctx->state < 4)
        goto done;
init:
    ((void (*)(void))D_8004DE10[0x54])();
    ((void (*)(int32_t))D_8004DE10[0x55])(0xf000);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))D_8004DE10[0x5d])(0x140, 0xf0, 0, 0);
    rect[0] = 0;
    rect[1] = 0;
    rect[2] = 0x140;
    rect[3] = 0xf0;
    object = ((void *(*)(int16_t *, int32_t, int32_t))D_8004DE10[0x5b])(rect, 3, 0x1000);
    ((void (*)(void *, int32_t, int32_t, int32_t))(*(void **)(object + 0x12c)))(object, 0, 0, 0);
    *out = STCRDSHP_func_80088ca0();
    ctx->notify(ctx);
done: ;
}
