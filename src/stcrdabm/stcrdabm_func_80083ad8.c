#include "common/types.h"

extern int32_t D0x8004de10[];
extern int32_t STCRDABM_func_8008566c(void);

/* STCRDABM:0x80083ad8 (248 bytes, 62 instructions); PAL reviewed body. */
void STCRDABM_func_80083ad8(int32_t task, int32_t *out) {
    int32_t state;
    int32_t *sys;
    int16_t params[4];
    int32_t handle;

    state = *(int32_t *)(task + 0xc);
    if (state == 0) goto work;
    if (state < 0) goto work;
    if (state < 4) return;
work:
    sys = D0x8004de10;
    ((void (*)(void))sys[0x54])();
    ((void (*)(int32_t))sys[0x55])(0xf000);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))sys[0x5d])(0x140, 0xf0, 0, 0);
    params[0] = 0;
    params[1] = 0;
    params[2] = 0x140;
    params[3] = 0xf0;
    handle = ((int32_t (*)(int16_t *, int32_t, int32_t))sys[0x5b])(params, 3, 0x1000);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))*(int32_t *)(handle + 0x12c))(handle, 0, 0, 0);
    *out = STCRDABM_func_8008566c();
    ((void (*)(int32_t))*(int32_t *)(task + 0x38))(task);
}
