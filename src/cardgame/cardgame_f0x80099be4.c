/*
 * CARDGAME:0x80099be4 CARDGAME_F0x80099be4
 * 308 bytes at CARDGAME.PRO offset 0x16f34 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float
 *  Variant     base
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x80099be4
 *  Symbols     CARDGAME_F0x800999a0=0x800999a0
 *  Compare     308 bytes from 0x80099be4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80099be4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Body: state byte at a3+8 dispatches (beq==1 / slti<2 / beq==2 / beq==3
 * chain): 1 -> a3[0] = 0x1000 - ((a3[2]<<12)/a3[3]); 3 -> a3[0] =
 * (a3[2]<<12)/a3[3]; both then a3[2] -= slot(), terminal state (2 / 0) unless
 * (diff<<16) > 0; common tail call (a0,a1,a2,a3). The slot jalr has a nop delay
 * slot, so the callee observably receives the undisturbed incoming a0 (no arg
 * move emitted).
 *
 * Dispatch note: the PAL chain jumps forward on every taken test (beq/slti+bne
 * into out-of-line case bodies over a default `j`), which the explicit-goto
 * form reproduces; 2-case switch/if-chain only emits beq/beq on the tested
 * toolchains.
 */

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
