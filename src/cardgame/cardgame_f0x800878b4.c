/*
 * CARDGAME:0x800878b4 CARDGAME_F0x800878b4
 * 716 bytes at CARDGAME.PRO offset 0x4c04 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800878b4
 *  Symbols     CARDGAME_F0x800870a4=0x800870a4 CARDGAME_F0x80087458=0x80087458
 *              CARDGAME_F0x80087edc=0x80087edc DAT_8004B7D0=0x8004b7d0
 *  Compare     716 bytes from 0x800878b4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800878b4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Codegen notes: shared base is an extern array (DAT_8004B7D0, --symbol at
 * link): a plain (int32_t *)0x8004b7d0 constant makes cc1 2.8.1 emit li
 * 0x80040000 + ori, while extern yields lui %hi + addiu %lo (probe-verified,
 * base toolchain). Indirect calls are nested (not temp-split) so each result is
 * saved late in the next jalr delay slot. Status combos test '!= 0' with the
 * single-call arm first, giving the original beq-into-else-body layout and
 * fall-through final store (no extra j).
 */

#include <stdint.h>

typedef int32_t (*cardgame_sys0_t)(int32_t);
typedef int32_t (*cardgame_sys1_t)(int32_t, int32_t);

extern int32_t DAT_8004B7D0[];
extern void CARDGAME_F0x800870a4(int32_t p1, int32_t p2, int32_t p3, int32_t step);
extern int32_t CARDGAME_F0x80087458(int32_t p1, int32_t p2, int32_t p3);

void CARDGAME_F0x800878b4(int32_t p1, int32_t p2, int32_t p3)
{
    int32_t off;

    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(uint8_t *)(p2 + off + 0x150) |= 1;
    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(int16_t *)(p2 + off + 0x12e) = 1;
    if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 15)) & 1) != 0)
        *(uint8_t *)(p1 + 0x423) = 6;
    if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 12)) & 1) != 0)
        *(uint8_t *)(p1 + 0x423) = 11;
    if (*(int16_t *)(p3 + 10) != 0) {
        if (((((cardgame_sys0_t)DAT_8004B7D0[0xff])(0) & (1 << ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 7))) |
             (((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) & (1 << ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 7)))) != 0) {
            if (0 < *(int32_t *)(p1 + 0x43c))
                CARDGAME_F0x800870a4(p1, p2, p3, -1);
        } else if (((((cardgame_sys0_t)DAT_8004B7D0[0xff])(0) & (1 << ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 5))) |
                    (((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) & (1 << ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 5)))) != 0) {
            if (*(int32_t *)(p1 + 0x43c) < *(int16_t *)(p3 + 10) - 1)
                CARDGAME_F0x800870a4(p1, p2, p3, 1);
        } else if ((((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 13)) & 1) != 0) &&
                   (CARDGAME_F0x80087458(p1, p2, p3) != 0)) {
            *(uint8_t *)(p1 + 0x423) = 4;
        }
    }
}
