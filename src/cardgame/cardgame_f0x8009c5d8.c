/* CARDGAME:0x8009c5d8, full PAL range 124 bytes, base 0x80082cb0.
 * Native GCC 2.8.1 / ASPSX 2.79 exact recipe: submissions/cardgame-8009c5d8/manifest-r5.json. */
#include "common/types.h"

extern int32_t (*D_80055c48)(uint32_t);

/* Access view at base + index*76, not an array element type: sizeof this view
 * is not the stride. Only the PAL byte/halfword/word offsets below are known. */
typedef struct {
    uint8_t unknown[0x120];
    uint16_t f120, f122;
    uint8_t unknown124[10];
    uint16_t f12e;
    uint32_t f130, f134;
    uint8_t unknown138[18];
    uint8_t f14a;
} CardInitView9c5d8;

int32_t CARDGAME_F0x8009c5d8(uint8_t *base, int32_t index)
{
    CardInitView9c5d8 *row;
    D_80055c48(0x8004613e);
    row = (CardInitView9c5d8 *)(base + index * 76);
    row->f14a = 4;
    row->f130 = 0;
    row->f134 = 0;
    row->f120 = 0x1000;
    row->f122 = 0x1000;
    row->f12e = 0;
    return 0;
}
