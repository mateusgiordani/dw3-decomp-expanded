// CARDGAME:0x8008f530 (size 48, 0x30)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xc880

extern void func_8008f2d8(void *ctx, int a1, int a2, int a3);

void CARDGAME_F0x8008f530(int *ctx, int a1) {
    func_8008f2d8(ctx, a1, -0x80, 0x10);
    ctx[0x438 / 4] = 0;
}
