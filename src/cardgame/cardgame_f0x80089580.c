/* CARDGAME:0x80089580, 368 PAL bytes. The shared switch exit preserves
 * the return-state merge across case 4; branch-local returns fold zero
 * differently under the matching PsyQ GCC 2.8.1 O2 build. */
#include <stdint.h>

typedef int32_t (*cardgame_sys0_t)(int32_t);
typedef int32_t (*cardgame_sys1_t)(int32_t, int32_t);
typedef void (*cardgame_m2_t)(int32_t, int32_t);
typedef void (*cardgame_m1_t)(int32_t);

extern int32_t DAT_8004B7D0[];

int32_t CARDGAME_F0x80089580(int32_t p1, int32_t p2)
{
    int32_t ret = 0;
    uint8_t st;
    uint8_t v;

    st = *(uint8_t *)(p1 + 0x422);
    switch (st) {
    case 1:
        v = *(uint8_t *)(p2 + 0xe9b);
        if (v == 2)
            *(uint8_t *)(p1 + 0x422) = v;
        break;
    case 2:
        if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 13)) & 1) != 0)
            goto L_state3;
        if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 14)) & 1) == 0)
            break;
    L_state3:
        *(uint8_t *)(p1 + 0x422) = 3;
        ((cardgame_m2_t)*(uint32_t *)(p2 + 0xeb0))(p2, 5);
        ((cardgame_m1_t)*(uint32_t *)(p2 + 0xee8))(p2);
        break;
    case 3:
        v = *(uint8_t *)(p2 + 0xe9b);
        if (v == 0)
            *(uint8_t *)(p1 + 0x422) = 4;
        break;
    case 4:
        ret = 1;
        break;
    }
    return ret;
}
