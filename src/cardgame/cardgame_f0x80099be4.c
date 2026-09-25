// CARDGAME:0x80099be4 (size 308, 0x134)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x16f34
// Boundary: symbols/function_labels.csv CARDGAME,0x80099be4,308 (PAL prologue + jr ra, sweep);
//   prev CARDGAME:0x800999a0 (580B) ends at 0x80099be4; next CARDGAME:0x80099d18 prologue
//   (27bdffe8/addiu sp,-0x18) observed in disasm right after jr ra delay slot.
// Ghidra CARDGAME (ddw3-pal-sles-03936, read-only, min_address 0x80082cb0, MIPS LE):
//   disasm 77 insns; decompile hypothesis only; x-ref to: 2 callers in
//   CARDGAME_F0x80099f60 @0x80099f80/@0x80099f94 (UNCONDITIONAL_CALL, a2=0/1, a3=s0+168/+252);
//   callees: tail jal CARDGAME_F0x800999a0 (RAM-slot jalr via 0x8004DF9C not in graph).
// Word check: 77/77 Ghidra insn bytes == PAL cardgame.bin @0x16f34 (authoritative program).
// Body: state byte at a3+8 dispatches (beq==1 / slti<2 / beq==2 / beq==3 chain):
//   1 -> a3[0] = 0x1000 - ((a3[2]<<12)/a3[3]); 3 -> a3[0] = (a3[2]<<12)/a3[3];
//   both then a3[2] -= slot(), terminal state (2 / 0) unless (diff<<16) > 0;
//   common tail call (a0,a1,a2,a3). The slot jalr has a nop delay slot, so the
//   callee observably receives the undisturbed incoming a0 (no arg move emitted).
// Dispatch note: the PAL chain jumps forward on every taken test (beq/slti+bne
// into out-of-line case bodies over a default `j`), which the explicit-goto form
// reproduces; 2-case switch/if-chain only emits beq/beq on the tested toolchains.
// Toolchain: psyq-gcc-2.95.2-4.0.0030 + aspsx-2.86 -O2 -G0 -mno-check-zero-division
//   (o2-g2952-no-div-check; bare div/mflo per overlay-wide division evidence).
#include <stdint.h>

typedef int32_t (*cardgame_slot_fn_t)(void);

extern void CARDGAME_F0x800999a0(int32_t a0, int32_t a1, int32_t a2, int16_t *a3);

void CARDGAME_F0x80099be4(int32_t a0, int32_t a1, int32_t a2, int16_t *a3)
{
    int32_t state;
    int32_t elapsed;

    state = ((uint8_t *)a3)[8];
    if (state == 1)
        goto L1;
    if (state < 2)
        goto LT;
    if (state == 2)
        goto LT;
    if (state == 3)
        goto L3;
    goto LT;
L1:
    a3[0] = (int16_t)(0x1000 - (a3[2] << 12) / a3[3]);
    elapsed = ((cardgame_slot_fn_t)*(uint32_t *)0x8004DF9C)();
    a3[2] = (int16_t)(a3[2] - elapsed);
    if (a3[2] > 0)
        goto LT;
    ((uint8_t *)a3)[8] = 2;
    goto LT;
L3:
    a3[0] = (int16_t)((a3[2] << 12) / a3[3]);
    elapsed = ((cardgame_slot_fn_t)*(uint32_t *)0x8004DF9C)();
    a3[2] = (int16_t)(a3[2] - elapsed);
    if (a3[2] > 0)
        goto LT;
    ((uint8_t *)a3)[8] = 0;
LT:
    CARDGAME_F0x800999a0(a0, a1, a2, a3);
}
