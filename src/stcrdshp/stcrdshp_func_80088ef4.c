#include "common/types.h"

/* STCRDSHP:0x80088ef4, 192B. PAL base 0x80082cb0, file off 0x6244. */
/* EXE helper table base 0x80044B38 (+0x404 slot); arg base *0x8005CCA8. */
extern int32_t D_8005CCA8;
extern uint8_t D_80044B38[];

int32_t STCRDSHP_func_80088ef4(void) {
    uint8_t *base = D_80044B38;
    if ((*(int32_t (**)(int32_t))(base + 0x404))(D_8005CCA8 + 0x32) != 0) {
        return 1;
    }
    if ((*(int32_t (**)(int32_t))(base + 0x404))(D_8005CCA8 + 0x16) != 0) {
        return 1;
    }
    if ((*(int32_t (**)(int32_t))(base + 0x404))(D_8005CCA8 + 0x1d) != 0) {
        return 1;
    }
    if ((*(int32_t (**)(int32_t))(base + 0x404))(D_8005CCA8 + 0x94) != 0) {
        return 1;
    }
    return (*(int32_t (**)(int32_t))(base + 0x404))(D_8005CCA8 + 0x6a) != 0;
}
