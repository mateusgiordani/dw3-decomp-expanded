// CARDGAME:0x80096cf0 (size 308, 0x134)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x14040
// Framed function: prologue 27bdffd8 addiu sp,-0x28, epilogue jr ra / 27bd0028.
// Dispatch on a3[4] (halfword at +8): 0 -> early return; otherwise a GCC
// balanced case tree (==2, <3 -> default, ==3). The `case 0:` label on the
// default body is what yields PAL's `slti 3` range test: its compare folds
// away without a constant load, jump1 inverts the range branch before CSE,
// and CSE's skip-block path keeps the `li s1,2` compare register for the
// `a3[4] = 2` store. Case 2 presets 0x1000 and shares the +2 store with
// case 3 (PAL Lde8). Tail call CARDGAME_F0x80096c20(a0, a1, a2, a3).
// Indirect: EXE slot 0x8004df9c (lui 0x8005 / lw -0x2064) via jalr, no args.
// Declared as a function-pointer array: with that slot load the default body
// stores +2 before the slot load (PAL `sh v0,2; lui; lw; nop; jalr; nop`).
// `int16_t q` keeps case 3's quotient out of the SI divmod pattern so it is
// allocated to v0 as in PAL.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base),
// fn_exact_pipeline --strip-div-guard.
// Status C_NONMATCHING (r7): 308 B, one differing byte (offset 0xac, the
// case-2 `j` target). tools/div_guard.py emits the post-mflo gap nop before
// the intervening `$L8` label; PAL's original assembler placed it after the
// label. Re-assembling this exact compiler output with the nop after the label
// is an exact_byte_match (evidence-only diagnostic; see strategy-r7.md).
#include <stdint.h>

typedef int (*CardSlotFn)(void);

extern void CARDGAME_F0x80096c20(int32_t a0, int32_t a1, int32_t a2, int16_t *a3);
extern CardSlotFn D0x8004df9c[];

void CARDGAME_F0x80096cf0(int32_t a0, int32_t a1, int32_t a2, int16_t *a3)
{
    int16_t s;
    int16_t t;
    int32_t r;
    int16_t q;

    s = a3[4];
    if (s == 0)
        return;
    switch (s) {
    case 0:
    default:
        a3[1] = (int16_t)(0x1000 - (((int32_t)a3[2] << 12) / (int32_t)a3[3]));
        r = D0x8004df9c[0]();
        t = (int16_t)(a3[2] - r);
        a3[2] = t;
        if (t <= 0)
            a3[4] = 2;
        break;
    case 2:
        q = 0x1000;
        goto store;
    case 3:
        r = D0x8004df9c[0]();
        t = (int16_t)(a3[2] - r);
        a3[2] = t;
        if (t <= 0)
            a3[4] = 0;
        q = ((int32_t)a3[2] << 12) / (int32_t)a3[3];
    store:
        a3[1] = q;
        break;
    }
    CARDGAME_F0x80096c20(a0, a1, a2, a3);
}
