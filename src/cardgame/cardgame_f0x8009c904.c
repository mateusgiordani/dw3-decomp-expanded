// CARDGAME:0x8009c904 (size 52, 0x34)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x19c54
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #167
// Prologue 27bdffe0 addiu sp,-0x20 ; jal 0x8009c898 ; epilogue 27bd0020
// Next function CARDGAME:0x8009c938 at +0x34, confirms size.

extern void func_8009c898(void *a0, int a1, int a2, int a3, int a4, int a5);

void CARDGAME_F0x8009c904(void *a0, int a1, int a2, int a3) {
    func_8009c898(a0, a1, 0, a2, a3, 1);
}
