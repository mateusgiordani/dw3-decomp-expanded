/* CARDGAME:0x8009c770, full PAL body 136 bytes, base 0x80082cb0. */
#include "common/types.h"

extern int32_t (*D_80055c48)(uint32_t);

/* Absolute-offset access view at base + index*76; sizeof is not the stride. */
typedef struct {
    uint8_t unknown000[0x108];
    uint32_t f108, f10c;
    uint8_t unknown110[8];
    uint32_t f118, f11c;
    uint8_t unknown120[14];
    uint16_t f12e;
    uint32_t f130, f134;
    uint8_t unknown138[18];
    uint8_t f14a;
    uint8_t unknown14b[4];
    uint8_t f14f;
} CardInitView9c770;

int32_t CARDGAME_F0x8009c770(uint8_t *base, int32_t index)
{
    CardInitView9c770 *row;
    uint32_t w0, w1;
    D_80055c48(0x40014);
    row = (CardInitView9c770 *)(base + index * 76);
    w0 = row->f108;
    w1 = row->f10c;
    row->f14a = 8;
    row->f130 = 0;
    row->f134 = 0;
    row->f14f = 2;
    row->f12e = 0;
    row->f118 = w0;
    row->f11c = w1;
    return 0;
}
