// CARDGAME:0x8008f560, 496-byte PAL body at verified base 0x80082cb0
// (reference/extracted/pro/cardgame.bin file-off 0xC9B0; ends at 0x8008f750).
// Toolchain psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, -O2 -G0,
// variant o2-g0-no-strength-reduce. Caller CARDGAME:0x80084320 (+0x852d4).
// exact_byte_match, PAL sha256 54615662...c6312d001b. Recovery notes in the
// task handoff; address formation follows cardgame_f0x80085f38 lesson 008.

#include <stdint.h>

typedef void (*cardgame_f560_fn1_t)(void *ctx, int32_t arg);
typedef void (*cardgame_f560_fn0_t)(void *ctx);
typedef void (*cardgame_f560_fn3_t)(void *ctx, int32_t a1, int32_t a2, int32_t a3);

int32_t CARDGAME_F0x8008f560(uint8_t *obj, uint8_t *ctx) {
    cardgame_f560_fn1_t fn1;
    cardgame_f560_fn0_t fn0;
    cardgame_f560_fn3_t fn3;
    int32_t result = 0;
    int32_t saved;
    int32_t i;
    int32_t j;
    int32_t off;
    int32_t base;
    int32_t cell;
    int32_t found;
    int32_t i2;
    int32_t j2;
    int32_t off2;
    int32_t base2;

    if (obj[0x423] != 0) {
        switch (obj[0x423]) {
        case 1:
            fn1 = *(cardgame_f560_fn1_t *)(ctx + 0xEB8);
            fn1(ctx, 0x3FF);
            *(int32_t *)(obj + 0x424) = 0;
            *(int32_t *)(obj + 0x428) = 0;
            break;
        case 2:
            fn0 = *(cardgame_f560_fn0_t *)(ctx + 0xEB4);
            fn0(ctx);
            break;
        }
        saved = obj[0x423];
        obj[0x423] = 0;
        obj[0x422] = (uint8_t)saved;
    }
    switch (obj[0x422]) {
    case 1: {
        if (*(int32_t *)(obj + 0x424) == *(int32_t *)(obj + 0x430) / 2) {
            for (i = 0, off = 0; i < 2; i++, off += 200) {
                uint8_t *p;
                j = 0;
                base = off;
                for (; j < 5; j++) {
                    cell = j + base;
                    p = obj + cell;
                    cell = p[0x5A8];
                    if (cell != 0) {
                        p[0x5A8] = (uint8_t)(cell - 1);
                    }
                    fn3 = *(cardgame_f560_fn3_t *)(ctx + 0xEA0);
                    fn3(ctx, i, j, p[0x5A8]);
                }
            }
        }
        *(int32_t *)(obj + 0x424) += 1;
        if (*(int32_t *)(obj + 0x424) > *(int32_t *)(obj + 0x430)) {
            found = 0;
            i2 = 0;
            off2 = 0;
            do {
                uint8_t *p2;
                j2 = 0;
                base2 = off2;
                do {
                    cell = j2 + base2;
                    p2 = obj + cell;
                    if (p2[0x5A8] != 0) {
                        found = 1;
                    }
                    j2++;
                } while (j2 < 5);
                i2++;
                off2 += 200;
            } while (i2 < 2);
            if (found != 0) {
                obj[0x423] = 1;
            } else {
                obj[0x423] = 2;
            }
        }
        break;
    }
    case 2:
        result = 1;
        break;
    }
    return result;
}
