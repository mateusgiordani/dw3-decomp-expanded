// CARDGAME:0x8008ec44, 644 bytes, PAL-SLES-03936 (base 0x80082cb0).
// Exact with PsyQ GCC 2.8.1 SN32 4.0.0010 / ASPSX 2.79,
// O2 variant o2-g0-no-strength-reduce. Return value is consumed by
// callers in CARDGAME:0x800a36e4; q is unused by the PAL body.
// Preserve the empty-range guard, loop-start copy and two unit advances:
// flow counts the advances before combine folds them to the halfword stride.
// Reproduction and controls: submissions/cardgame-8008ec44/strategy-r10-final-astra.md.
#include <stdint.h>

extern void func_8001ebf8(void *buf);
extern int16_t D0x800a58f4[];

typedef void (*cardgame_8008ec44_cb_t)(int32_t x);

int32_t CARDGAME_F0x8008ec44(uint8_t *p, void *q, int32_t idx, int32_t mode, uint32_t flags)
{
    void *buf[22];
    int32_t ret = 0;
    int32_t count = 0;
    int32_t idx2;
    int32_t i;
    uint8_t *rec;
    int32_t base6;
    int32_t adv;
    int32_t x;
    uint8_t *bp;
    int32_t k;
    int16_t *tbl;
    uint8_t *e;

    (void)q;
    switch (mode) {
    case 2:
        idx2 = 0;
        e = p + idx * 200;
        count = *(int16_t *)(e + 0x5a6);
        break;
    case 3:
        idx2 = 0;
        e = p + idx * 200;
        count = *(int16_t *)(e + 0x5a4);
        break;
    case 4:
        idx2 = 0;
        e = p + idx * 200;
        count = *(int16_t *)(e + 0x5a2);
        break;
    default:
        idx2 = 0;
        break;
    }
    i = 0;
    if (count <= i)
        return ret;
    base6 = idx * 200;
    i = idx2;
    rec = p;
    adv = base6;
    do {
        rec[0x446] = 0;
        switch (mode) {
        case 2:
            idx2 = *(int16_t *)(p + adv + 0x600);
            break;
        case 3:
            idx2 = *(int16_t *)(p + ((i + *(int16_t *)(p + base6 + 0x5a0)) * 2 + base6) + 0x5b0);
            break;
        case 4:
            idx2 = *(int16_t *)(p + adv + 0x614);
            break;
        }
        e = p + idx2 * 2;
        x = *(int16_t *)(e + 0x50);
        func_8001ebf8(buf);
        (*(cardgame_8008ec44_cb_t *)((char *)buf + 0x2c))(x + 1);
        x = 0;
        if ((*(uint8_t **)buf)[3] == 0x10) {
            x = flags & 1;
        } else if ((flags & 2) != 0) {
            x = 1;
        }
        if (x) {
            k = 0;
            bp = *(uint8_t **)buf;
            tbl = D0x800a58f4;
            for (; k < 6; k++, tbl++) {
                if ((flags & *tbl) != 0 && bp[0] == k + 1) {
                    rec[0x446] = 1;
                    ret = 1;
                    break;
                }
            }
        }
        rec++;
        i++;
        adv++;
        adv++;
    } while (i < count);
    return ret;
}
