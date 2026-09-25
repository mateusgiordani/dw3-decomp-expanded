/*
 * CARDGAME:0x80089580 CARDGAME_F0x80089580
 * 368 bytes at CARDGAME.PRO offset 0x68d0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80089580
 *  Symbols     DAT_8004B7D0=0x8004b7d0
 *  Compare     368 bytes from 0x80089580 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80089580
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * The shared switch exit preserves the return-state merge across case 4;
 * branch-local returns fold zero differently under the matching PsyQ GCC 2.8.1
 * O2 build.
 */

#include <stdint.h>

typedef int32_t (*cardgame_sys0_t)(int32_t);
typedef int32_t (*cardgame_sys1_t)(int32_t, int32_t);
typedef void (*cardgame_m2_t)(int32_t, int32_t);
typedef void (*cardgame_m1_t)(int32_t);

extern int32_t DAT_8004B7D0[];

int32_t CARDGAME_F0x80089580(int32_t p1, int32_t p2)
{
    int32_t ret = 0;
    uint8_t st;
    uint8_t v;

    st = *(uint8_t *)(p1 + 0x422);
    switch (st) {
    case 1:
        v = *(uint8_t *)(p2 + 0xe9b);
        if (v == 2)
            *(uint8_t *)(p1 + 0x422) = v;
        break;
    case 2:
        if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 13)) & 1) != 0)
            goto L_state3;
        if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 14)) & 1) == 0)
            break;
    L_state3:
        *(uint8_t *)(p1 + 0x422) = 3;
        ((cardgame_m2_t)*(uint32_t *)(p2 + 0xeb0))(p2, 5);
        ((cardgame_m1_t)*(uint32_t *)(p2 + 0xee8))(p2);
        break;
    case 3:
        v = *(uint8_t *)(p2 + 0xe9b);
        if (v == 0)
            *(uint8_t *)(p1 + 0x422) = 4;
        break;
    case 4:
        ret = 1;
        break;
    }
    return ret;
}
