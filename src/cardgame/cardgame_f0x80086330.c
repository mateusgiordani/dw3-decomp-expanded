#include <stdint.h>

extern unsigned int CARDGAME_F0x80085fd0(void *p, unsigned char *k, int idx);

typedef void (*CardCb6330)(void *);

void CARDGAME_F0x80086330(void *p, void *q, int tag) {
    int i;
    int found;
    char *cur;

    *(unsigned int *)((char *)p + 0x440) = 0;
    *(unsigned char *)((char *)p + 0x422) = 0;
    if (*(short *)((char *)q + 0x64) == 0) {
        *(unsigned char *)((char *)p + 0x49d) = 1;
        *(unsigned char *)((char *)p + 0x499) = 1;
        (*(CardCb6330 *)((char *)q + 0xec8))(q);
    }
    i = 0;
    *(unsigned int *)((char *)p + 0x424) = 0;
    *(unsigned int *)((char *)q + 0xde8) = (unsigned int)tag;
    *(unsigned int *)((char *)q + 0xde4) = 0;
    found = 0;
    if (*(short *)((char *)p + 0x5a6) > found) {
        cur = (char *)p;
        do {
            if (CARDGAME_F0x80085fd0(p, (unsigned char *)p + 0x5a8, *(short *)(cur + 0x600)) != 0) {
                found = 1;
                break;
            }
            i++;
            cur += 2;
        } while (i < *(short *)((char *)p + 0x5a6));
    }
    if (found != 0) {
        *(short *)((char *)q + 0xdf4) = 0;
        *(unsigned int *)((char *)p + 0x440) = 0;
        *(unsigned int *)((char *)p + 0x438) = 2;
    } else {
        *(short *)((char *)q + 0xdf4) = 1;
        *(unsigned int *)((char *)p + 0x440) = 1;
        *(unsigned int *)((char *)p + 0x438) = 3;
    }
}
