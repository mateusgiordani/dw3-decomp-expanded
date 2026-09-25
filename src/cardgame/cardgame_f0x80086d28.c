// CARDGAME:0x80086d28 (size 80, 0x50)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x4078
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #12
// PAL words (20, little-endian): 27bdffe8 afbf0010 00c01821 24020001 10620006
//   00003021 24020002 10620005 00000000 08021b58 00000000 08021b58 24060006
//   2406000c 0c021a8c 00000000 8fbf0010 00000000 03e00008 27bd0018
// Disasm: addiu sp,-24 ; sw ra,16(sp) ; move v1,a2 ; li v0,1 ; beq v1,v0,L1
//   (delay: move a2,zero) ; li v0,2 ; beq v1,v0,L2 (delay: nop) ; j Ljoin
//   (delay: nop) ; L1: j Ljoin (delay: li a2,6) ; L2: li a2,12 ;
//   Ljoin: jal 0x80086a30 (delay: nop) ; lw ra,16(sp) ; nop ; jr ra (delay: addiu sp,+24)
// Next framed CARDGAME:0x80086d78 at +0x50, prev CARDGAME:0x80086a30 size 0x2f8
// ends exactly at 0x80086d28: size 0x50 self-contained (prologue/epilogue pair).
// Ghidra program CARDGAME read-only (never import); upstream cardgame.s GUIDE only.
// Frame -0x18/+0x18, 4 Ghidra xrefs (jal sites in 0x8008a068/0x8008ad98),
// 1 callee CARDGAME:0x80086a30 (extern + --symbol).
// Portable C: a2 dispatcher (1->6, 2->12, else 0) forwarding a0/a1 to callee.
// The sel=0 initializer lets the compiler fill the first beq delay slot with
// move a2,zero, and the explicit goto-join shape emits the two j-to-join with
// nop delays plus jal-delay nop, matching the reference's unscheduled layout.
// The subu/addu/j $31 emitted by cc1 are assembled by ASPSX to the reference
// addiu/jr ra words via pseudo-instruction expansion.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 -mips1 -msoft-float
// (variant base). exact_byte_match, candidate == reference sha256 26725c31....

#include "common/types.h"

extern void CARDGAME_F0x80086a30(void *a0, void *a1, int32_t a2);

void CARDGAME_F0x80086d28(void *a0, void *a1, int32_t a2)
{
    int32_t sel = 0;

    if (a2 == 1)
        goto L6;
    if (a2 == 2)
        goto L12;
    goto Ljoin;
L6:
    sel = 6;
    goto Ljoin;
L12:
    sel = 12;
Ljoin:
    CARDGAME_F0x80086a30(a0, a1, sel);
}
