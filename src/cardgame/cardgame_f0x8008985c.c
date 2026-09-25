/* CARDGAME:0x8008985c, 280 PAL bytes. Direct PAL callers target the
 * global callback load eight bytes before the former framed entry.
 * The callback is loaded from 0x8004dc0c; no incoming-v0 ABI.
 * Evidence: reports/handoffs/cardgame-80089864-c-recovery.md. */
#include <stdint.h>

typedef int32_t (*cardgame_flag_cb_t)(void);
extern cardgame_flag_cb_t D_8004dc0c;
typedef void (*cardgame_m4_t)(void *p, int32_t a1, int32_t a2, int32_t a3);
typedef void (*cardgame_m3_t)(void *p, int32_t a1, int32_t a2);

void CARDGAME_F0x8008985c(void *p1, void *p2)
{
    int32_t flag = D_8004dc0c() & 1;

    *(int32_t *)((uint8_t *)p1 + 0x434) = flag;
    *(int32_t *)((uint8_t *)p1 + 0x430) = 0;
    *(int32_t *)((uint8_t *)p1 + 0x42c) = 0;
    *(int32_t *)((uint8_t *)p1 + 0x428) = 0;
    *(int32_t *)((uint8_t *)p1 + 0x424) = 0;
    ((cardgame_m4_t)*(uint32_t *)((uint8_t *)p2 + 0xf14))(p2, 0, 0x7400, 0x6100);
    ((cardgame_m4_t)*(uint32_t *)((uint8_t *)p2 + 0xf14))(p2, 1, 0xa400, 0x6100);
    if (flag != 0) {
        ((cardgame_m3_t)*(uint32_t *)((uint8_t *)p2 + 0xf3c))(p2, 0, 0x57);
        ((cardgame_m3_t)*(uint32_t *)((uint8_t *)p2 + 0xf3c))(p2, 1, 0x58);
    } else {
        ((cardgame_m3_t)*(uint32_t *)((uint8_t *)p2 + 0xf3c))(p2, 1, 0x57);
        ((cardgame_m3_t)*(uint32_t *)((uint8_t *)p2 + 0xf3c))(p2, 0, 0x58);
    }
    *(uint8_t *)((uint8_t *)p2 + 0x14d) = 2;
    *(uint8_t *)((uint8_t *)p2 + 0x199) = 2;
    *(uint16_t *)((uint8_t *)p2 + 0x120) = 0;
    *(uint8_t *)((uint8_t *)p2 + 0x151) = 0;
    *(uint8_t *)((uint8_t *)p2 + 0x150) = 0;
    *(uint16_t *)((uint8_t *)p2 + 0x16c) = 0;
    *(uint8_t *)((uint8_t *)p2 + 0x19d) = 0;
    *(uint8_t *)((uint8_t *)p2 + 0x19c) = 0;
    *(int32_t *)((uint8_t *)p1 + 0x43c) = 0;
    *(int32_t *)((uint8_t *)p1 + 0x440) = 0;
    *(uint8_t *)((uint8_t *)p1 + 0x422) = 1;
}
