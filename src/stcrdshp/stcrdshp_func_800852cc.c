#include "common/types.h"

typedef void (*STCRDSHP_Callback)(void *object);

extern void STCRDSHP_func_800844b8(void *object, int32_t arg);
extern void STCRDSHP_func_80083ba8(void *object);
extern void STCRDSHP_func_8008300c(void *object, int32_t arg);
extern void STCRDSHP_func_800843c0(void *);

void STCRDSHP_func_800852cc(void *object, int32_t arg) {
    int32_t state = *(int32_t *)((char *)object + 0x0c);

    if (state == 1)
        goto state1;
    if (state < 2)
        goto initialize;
    if (state > 3)
        goto initialize;
    return;
initialize:
    ((STCRDSHP_Callback)*(int32_t *)((char *)object + 0x38))(object);
    STCRDSHP_func_8008300c(object, arg);
    *(int32_t *)((char *)object + 0x720) = 10;
    *(int32_t *)((char *)object + 0x710) = 10;
    *(int32_t *)((char *)object + 0x6f0) = 10;
    *(int32_t *)((char *)object + 0x700) = 10;
    STCRDSHP_func_800843c0(object);
    return;
state1:
    STCRDSHP_func_800844b8(object, arg);
    STCRDSHP_func_80083ba8(object);
}
