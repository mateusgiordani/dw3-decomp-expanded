#include "common/types.h"

typedef void (*PairCall)(int32_t, int32_t);
typedef void (*TripleCall)(int32_t, int32_t, int32_t);
typedef void (*QuadCall)(int32_t, int32_t, int32_t, int32_t);
extern int32_t (*D_80044f5c)(int32_t);
extern void F0x8001f648(uint8_t *);

void CARDGAME_F0x80097a24(int32_t a0, int32_t a1, int32_t a2, int32_t a3)
{
    uint8_t buf[0xa0];

    F0x8001f648(buf);
    (*(PairCall *)(buf + 0x7c))(0x100, 1);
    (*(PairCall *)(buf + 0x74))(0x280, 0);
    (*(TripleCall *)(buf + 0x8c))(0x1000, 0x1000, 0x1000);
    (*(QuadCall *)(buf + 0x84))(D_80044f5c(0x025d0002), 0x18, a2, a3);
}
