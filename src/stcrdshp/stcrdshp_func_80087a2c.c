#include "common/types.h"

extern void *D_8008CB98;
extern int32_t STCRDSHP_func_80085fdc(int32_t arg0, void *arg1);
extern void STCRDSHP_func_80086074(void *arg0, void *arg1);
extern void STCRDSHP_func_8008674c(void *arg0);
extern void STCRDSHP_func_80086dc4(void *arg0, void *arg1);

void STCRDSHP_func_80087a2c(uint8_t *obj, uint8_t *ctx) {
    int32_t state;
    int32_t count;
    int32_t i;
    uint8_t *p;
    int32_t r;

    state = *(int32_t *)(obj + 0xc);
    if (state == 1) {
        goto case1;
    }
    if (state < 2) {
        goto init;
    }
    if (state == 2) {
        goto case2;
    }
    if (state == 3) {
        return;
    }
init:
    (*(void (*)(void *))*(void **)(obj + 0x38))(obj);
    *(void **)(obj + 0xa8) = (*(void *(*)(int32_t))D_8008CB98)(*(int32_t *)(obj + 0xa0));
    i = 0;
    p = obj;
    do {
        int32_t j = i * 2;
        int32_t b = *(int32_t *)(*(int32_t *)(obj + 0xa8) + 8);
        *(int32_t *)(p + 0x74) = (int32_t)*(int16_t *)(j + b);
        i += 1;
        p += 4;
    } while (i < 6);
    count = *(int32_t *)(*(int32_t *)(obj + 0xa8) + 4);
    r = count % 6;
    *(int32_t *)(obj + 0xa4) = count;
    /* resto primeiro + q-(-flag): formacao de quantidade e addu PAL v0,v1,v0 */
    r = (r != 0);
    r = count / 6 - -r;
    *(int32_t *)(obj + 0x70) = r;
    *(int32_t *)(ctx + 0x48) = STCRDSHP_func_80085fdc(*(int32_t *)(obj + 0x50), obj + 0x74);
    STCRDSHP_func_80086074(obj, ctx);
    *(int32_t *)(obj + 0xac) = 10;
    *(int32_t *)(obj + 0xbc) = 10;
    *(int32_t *)(obj + 0xcc) = 10;
    return;
case1:
    STCRDSHP_func_80086dc4(obj, ctx);
    STCRDSHP_func_8008674c(obj);
    return;
case2:
    if (*(int32_t *)(ctx + 0x48) != 0) {
        return;
    }
    *(int32_t *)(obj + 0xc) = 3;
    return;
}
