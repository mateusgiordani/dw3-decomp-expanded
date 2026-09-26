/*
 * STCRDDEK:0x800866a8 STCRDDEK_F0x800866a8
 * 116 bytes at STCRDDEK.PRO offset 0x39f8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800866a8
 *  Symbols     DAT_8004B7D0=0x8004b7d0
 *  Compare     116 bytes from 0x800866a8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800866a8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x20 (s0-s2, ra).
 *
 * Moves the object to state 3 (p1[0xc]) when the button bit reported by the
 *
 * EXE system table at 0x8004b7d0 is set: the pad word from slot 0x3f4 is
 * shifted by the bit index that slot 0x408 returns for button code 14.
 */

#include <stdint.h>

typedef int32_t (*sys0_t)(int32_t);
typedef int32_t (*sys1_t)(int32_t, int32_t);

extern int32_t DAT_8004B7D0[];

#define S32(p, o)    (*(int32_t *)((p) + (o)))

void STCRDDEK_F0x800866a8(int32_t p1)
{
    if ((((sys0_t)DAT_8004B7D0[0x3f4 / 4])(0) >> ((sys1_t)DAT_8004B7D0[0x408 / 4])(0, 14)) & 1)
        S32(p1, 0xc) = 3;
}
