// CARDGAME:0x8008f750 (size 64, 0x40)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xcaa0

void CARDGAME_F0x8008f750(void *a0, void *a1) {
    signed char *v1;
    int v;
    void (*fn)(void *, int);
    v1 = (signed char *)a0;
    v = v1[0x575];
    *(int *)(v1 + 0x424) = 0;
    v1[0x422] = 1;
    fn = *(void (**)(void *, int))((char *)a1 + 0xEA8);
    fn(a1, v - 2);
}
