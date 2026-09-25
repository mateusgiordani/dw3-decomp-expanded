// CARDGAME:0x8008ad58 (size 32, 0x20)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x80a8
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #34
// Prologue 27bdffe8 addiu sp,-0x18 ; sw ra,0x10(sp) ; jal 0x80089ed4 ; ...
// Next function CARDGAME:0x8008ad78 at +0x20, confirms size.

extern void func_80089ed4(void);

void CARDGAME_F0x8008ad58(void) {
    func_80089ed4();
}
