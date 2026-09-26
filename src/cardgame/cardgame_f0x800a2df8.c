/*
 * CARDGAME:0x800a2df8 CARDGAME_F0x800a2df8
 * 264 bytes at CARDGAME.PRO offset 0x20148 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a2df8
 *  Symbols     CARDGAME_F0x80084320=0x80084320 CARDGAME_F0x8009e560=0x8009e560
 *              CARDGAME_F0x8009e62c=0x8009e62c CARDGAME_F0x8009e668=0x8009e668
 *              CARDGAME_F0x800a1b48=0x800a1b48 CARDGAME_F0x800a2180=0x800a2180
 *              CARDGAME_F0x800a2608=0x800a2608
 *  Compare     264 bytes from 0x800a2df8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a2df8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x28 (s0-s4, ra).
 *
 * Runs three per-frame helpers, then dispatches on the state byte p1[0x2f4]: 1
 * calls CARDGAME_F0x80084320 with p2's word at +0x18; 2 clears the state when
 * CARDGAME_F0x800a2180 reports done; 3 runs CARDGAME_F0x800a2608 (1 -> state 1,
 * 2 -> return 1); anything else returns CARDGAME_F0x800a1b48 != 0.
 *
 * Matching note: `case 0:` shares the default body, placed first, which gives
 * PAL's compare order (==1, <2, ==2, ==3); case 3 is a nested switch.
 */

#include <stdint.h>

void CARDGAME_F0x8009e62c(int32_t p1, int32_t p2);
void CARDGAME_F0x8009e668(int32_t p1, int32_t p2);
void CARDGAME_F0x8009e560(int32_t p1, int32_t p2);
void CARDGAME_F0x80084320(int32_t p1, int32_t arg);
int32_t CARDGAME_F0x800a1b48(int32_t p1, int32_t p2);
int32_t CARDGAME_F0x800a2180(int32_t p1, int32_t p2);
int32_t CARDGAME_F0x800a2608(int32_t p1, int32_t p2);

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

int32_t CARDGAME_F0x800a2df8(int32_t p1, int32_t p2)
{
    int32_t ret;

    ret = 0;
    CARDGAME_F0x8009e62c(p1, p2);
    CARDGAME_F0x8009e668(p1, p2);
    CARDGAME_F0x8009e560(p1, p2);
    switch (U8(p1, 0x2f4)) {
    case 0:
    default:
        if (CARDGAME_F0x800a1b48(p1, p2) != 0)
            ret = 1;
        break;
    case 1:
        CARDGAME_F0x80084320(p1, S32(p2, 0x18));
        break;
    case 2:
        if ((uint8_t)CARDGAME_F0x800a2180(p1, p2) != 0)
            U8(p1, 0x2f4) = 0;
        break;
    case 3:
        switch (CARDGAME_F0x800a2608(p1, p2)) {
        case 1:
            U8(p1, 0x2f4) = 1;
            break;
        case 2:
            ret = 1;
            break;
        }
        break;
    }
    return ret;
}
