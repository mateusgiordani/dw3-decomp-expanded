/*
 * CARDGAME:0x80096cf0 CARDGAME_F0x80096cf0
 * 308 bytes at CARDGAME.PRO offset 0x14040 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80096cf0
 *  Symbols     CARDGAME_F0x80096c20=0x80096c20 D0x8004df9c=0x8004df9c
 *  Compare     308 bytes from 0x80096cf0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80096cf0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue 27bdffd8 addiu sp,-0x28, epilogue jr ra / 27bd0028.
 *
 * Dispatch on a3[4] (halfword at +8): 0 -> early return; otherwise a GCC
 * balanced case tree (==2, <3 -> default, ==3). The `case 0:` label on the
 * default body is what yields PAL's `slti 3` range test: its compare folds away
 * without a constant load, jump1 inverts the range branch before CSE, and CSE's
 * skip-block path keeps the `li s1,2` compare register for the `a3[4] = 2`
 * store. Case 2 presets 0x1000 and shares the +2 store with case 3 (PAL Lde8).
 * Tail call CARDGAME_F0x80096c20(a0, a1, a2, a3).
 *
 * Indirect: EXE slot 0x8004df9c (lui 0x8005 / lw -0x2064) via jalr, no args.
 *
 * Declared as a function-pointer array: with that slot load the default body
 * stores +2 before the slot load (PAL `sh v0,2; lui; lw; nop; jalr; nop`).
 *
 * `int16_t q` keeps case 3's quotient out of the SI divmod pattern so it is
 * allocated to v0 as in PAL.
 *
 * PAL's original assembler placed it after the label.
 */

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
