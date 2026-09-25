#include "common/types.h"

/* RAM-resident vector block: PAL keeps 0x8004DE10 in callee-saved s2
 * (lui s2,0x8005 + addiu s2,-0x21f0) with slots +0x158 (alloc),
 * +0x15c (done), +0x17c (make). */
typedef struct {
    uint8_t pad[0x158];
    void *(*alloc)(void);
    void (*done)(void *packet);
    uint8_t pad2[0x1c];
    void *(*make)(void *object);
} stcrdshp_tbl_8004DE10_t;

extern stcrdshp_tbl_8004DE10_t stcrdshp_tbl_8004DE10;

void STCRDSHP_func_8008543c(void *self) {
    uint8_t *state = self;
    uint8_t *packet;
    uint8_t *list;
    int32_t level;
    uint8_t *resource;
    uint8_t *f;
    /* sched1-ready-list-boost (docs/c-matching-guide/card-techniques.md):
     * b is set twice, so li 0x2a loses the sched1 ready-list boost and the
     * lui of 0xe1000245 lands before sb 7; m is set right after the alloc
     * so the 0xffffff split stays above li 5, as in PAL. */
    uint32_t m;
    uint8_t b;

    resource = stcrdshp_tbl_8004DE10.make(*(void **)(state + 0x50));
    list = (*(uint8_t *(**)(void *, uint32_t))(resource + 0x138))(resource, *(uint32_t *)(state + 0x54));
    packet = stcrdshp_tbl_8004DE10.alloc();
    m = 0xffffff;
    b = 5;
    packet[3] = b;
    b = 0x2a;
    packet[7] = b;
    level = *(int32_t *)(state + 0x5c) >> 8;
    *(uint16_t *)(packet + 0x14) = 0x140;
    *(uint16_t *)(packet + 0x0c) = 0x140;
    *(uint16_t *)(packet + 0x10) = 0;
    *(uint16_t *)(packet + 0x08) = 0;
    *(uint16_t *)(packet + 0x0e) = 0;
    *(uint16_t *)(packet + 0x0a) = 0;
    *(uint16_t *)(packet + 0x16) = 0x100;
    *(uint16_t *)(packet + 0x12) = 0x100;
    packet[6] = (uint8_t)level;
    packet[5] = (uint8_t)level;
    packet[4] = (uint8_t)level;
    *(uint32_t *)packet = (*(uint32_t *)packet & 0xff000000) | (*(uint32_t *)list & m);
    *(uint32_t *)list = (*(uint32_t *)list & 0xff000000) | ((uint32_t)packet & m);
    f = packet + 0x18;
    f[3] = 1;
    *(uint32_t *)(f + 4) = 0xe1000245;
    *(uint32_t *)(packet + 0x18) = (*(uint32_t *)(packet + 0x18) & 0xff000000) | (*(uint32_t *)list & m);
    *(uint32_t *)list = (*(uint32_t *)list & 0xff000000) | ((uint32_t)f & m);
    stcrdshp_tbl_8004DE10.done(packet + 0x20);
}
