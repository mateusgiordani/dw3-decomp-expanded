#include "common/types.h"

/* CARDGAME:0x80092438, 512-byte PAL body at verified base 0x80082cb0.
 * Direct table arguments preserve the original fifth-argument stack store
 * before the fourth-argument load. Full evidence is retained in the handoff.
 */

typedef void (*CardFn1)(uint8_t *s0);
typedef void (*CardFn3)(uint8_t *s0, int32_t a1, int32_t a2);
typedef void (*CardFn4)(uint8_t *s0, int32_t a1, int32_t a2, int32_t a3);
typedef void (*CardFn5)(uint8_t *s0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);

extern uint32_t D_800A5910[];

void CARDGAME_F0x80092438(uint8_t *s1, uint8_t *s0, int32_t a2) {

    if ((a2 == 0) || (*(int16_t *)(s0 + 0x64) == 0)) {
        if (*(int16_t *)(s0 + 0x64) == 0) {
            /* Branch A: full setup + flag-gated clear + status 1. */
            ((CardFn1)*(void **)(s0 + 0xECC))(s0);
            *(uint32_t *)(s1 + 0x440) = 0;
            ((CardFn4)*(void **)(s0 + 0xF14))(s0, 15, 0xE500, 0x6100);
            ((CardFn3)*(void **)(s0 + 0xF3C))(s0, 15, *(int16_t *)(s1 + ((((int8_t *)s1)[0x575]) << 3) + 0x580));
            *(int16_t *)(s0 + 0x594) = 0;
            ((CardFn5)*(void **)(s0 + 0xF24))(s0, 15, 8, 0x1000, 0x1000);
            ((CardFn1)*(void **)(s0 + 0xEC8))(s0);
            s1[0x49D] = 1;
            s1[0x4AD] = 0;
            if (a2 == 0) {
                int32_t i = 14;
                uint8_t *q = s1 + i;
                do {
                    *(q + 0x46F) = 0;
                    q--;
                    i--;
                } while (i >= 0);
            }
            s1[0x499] = 1;
            s1[0x422] = 1;
        } else {
            /* Branch B: table-driven setup + status 5. */
            uint32_t *entry;

            entry = (uint32_t *)((uint8_t *)D_800A5910 + ((((int8_t *)s1)[0x575]) << 3));
            ((CardFn5)*(void **)(s0 + 0xF08))(s0, 15, 10, entry[0], entry[1]);
            s1[0x422] = 5;
            *(uint32_t *)(s1 + 0x424) = 0;
        }
    } else {
        /* Branch C: setup variant + status 2. */
        ((CardFn4)*(void **)(s0 + 0xF14))(s0, 15, 0xE500, 0x6100);
        ((CardFn3)*(void **)(s0 + 0xF3C))(s0, 15, *(int16_t *)(s1 + ((((int8_t *)s1)[0x575]) << 3) + 0x580));
        *(int16_t *)(s0 + 0x594) = 0;
        ((CardFn5)*(void **)(s0 + 0xF24))(s0, 15, 8, 0x1000, 0x1000);
        s1[0x422] = 2;
        *(uint32_t *)(s1 + 0x424) = 0;
        *(uint32_t *)(s1 + 0x428) = 0;
    }

    ((CardFn1)*(void **)(s0 + 0xEB4))(s0);
    {
        int32_t i = 14;
        uint8_t *q = s0 + 0x428;
        do {
            *(q + 0x151) = 0;
            q -= 0x4C;
            i--;
        } while (i >= 0);
    }
}
