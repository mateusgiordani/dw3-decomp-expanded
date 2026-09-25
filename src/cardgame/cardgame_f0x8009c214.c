// CARDGAME:0x8009c214 (size 72, 0x48)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x19564
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #161
// Prologue 27bdffe8 addiu sp,-0x18 ; sw s0,0x10(sp) ; addu s0,a0 ; lui a0,0x8004 ; lui v0,0x8005 ; lw v0,0x5c48(v0) ; sw ra,0x14(sp) ; jalr v0 ; ori a0,0x503c delay
// Body: addiu v0,10 ; sh v0,0xe02(s0) ; sh v0,0xe00(s0) ; addiu v0,4 ; sh v0,0xe0a(s0) ; lw ra,0x14(sp) ; lw s0,0x10(sp) ; jr ra ; addiu sp,+0x18
// Next bytes CARDGAME:0x8009c25c at +0x48 (dispatch-table LAB slot +0xf00 in CARDGAME_F0x8009d310), confirms size 0x48 is self-contained (prologue/epilogue pair).
// Ghidra program CARDGAME read-only (never import); cardgame.s GUIDE only.
// DATA x-ref: table installer CARDGAME_F0x8009d310 stores &CARDGAME_F0x8009c214 at slot +0xefc (callback family with 0x8009c054/0x8009c0dc/0x8009c1cc).
// Callee: indirect jalr via word at 0x80055c48 (EXE-resident vector, -G0 absolute lui/lw); arg 0x8004503c is a 32-bit constant (lui 0x8004 + ori delay).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
#include "common/types.h"

typedef void (*cardgame_svc_fn)(uint32_t arg);
extern cardgame_svc_fn DAT_80055c48;

void CARDGAME_F0x8009c214(uint8_t *work)
{
    DAT_80055c48(0x8004503cu);
    *(int16_t *)(work + 0xe02) = 10;
    *(int16_t *)(work + 0xe00) = 10;
    *(int16_t *)(work + 0xe0a) = 4;
}
