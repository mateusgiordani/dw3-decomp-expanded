/*
 * CARDGAME:0x800a1b48 CARDGAME_F0x800a1b48
 * 624 bytes at CARDGAME.PRO offset 0x1ee98 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a1b48, jump table (.rodata) at 0x80083a00
 *  Symbols     CARDGAME_F0x8009f110=0x8009f110 CARDGAME_F0x8009f8c0=0x8009f8c0
 *              CARDGAME_F0x8009f910=0x8009f910 CARDGAME_F0x8009fdd8=0x8009fdd8
 *              CARDGAME_F0x800a0548=0x800a0548 CARDGAME_F0x800a0b80=0x800a0b80
 *              CARDGAME_F0x800a0f4c=0x800a0f4c CARDGAME_F0x800a0f94=0x800a0f94
 *              CARDGAME_F0x800a1a5c=0x800a1a5c CARDGAME_F0x800a1af8=0x800a1af8
 *  Compare     624 bytes from 0x800a1b48 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a1b48
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x20 (s0-s2, ra).
 *
 * Card game mode driver. A pending mode at p1+0x2f7 (jump table 0x80083a00)
 * runs its exit action and becomes the current mode at p1+0x2f8 (the old one
 * moves to p1+0x2f6); the current mode (jump table 0x80083a28) then runs one
 * step and may request mode 10 (or 9). Returns 1 when mode 4 or 8 finished.
 *
 * Matching note: mode 1 dispatches on p1+0x2f9 with a two-case switch (PAL
 * tests 0 and 1 separately; `== 0 || != 1` folds to one test).
 */

#include <stdint.h>

void CARDGAME_F0x800a0548(int32_t p1, int32_t p2);
void CARDGAME_F0x800a0f4c(int32_t p1, int32_t p2);
void CARDGAME_F0x800a1a5c(int32_t p1, int32_t p2);
int32_t CARDGAME_F0x8009f110(int32_t p1, int32_t p2);
int32_t CARDGAME_F0x8009f8c0(int32_t p1, int32_t p2);
int32_t CARDGAME_F0x8009f910(int32_t p1, int32_t p2);
int32_t CARDGAME_F0x8009fdd8(int32_t p1, int32_t p2);
int32_t CARDGAME_F0x800a0b80(int32_t p1, int32_t p2);
int32_t CARDGAME_F0x800a0f94(int32_t p1, int32_t p2);
void CARDGAME_F0x800a1af8(int32_t p1, int32_t p2);

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))
#define SCENE        S32(p2, 0x18)

int32_t CARDGAME_F0x800a1b48(int32_t p1, int32_t p2)
{
    int32_t ret;

    ret = 0;
    if (U8(p1, 0x2f7) != 0) {
        switch (U8(p1, 0x2f7)) {
        case 1:
        case 2:
            U8(p1, 0x2f9) = 0;
            S32(p1, 0x2fc) = 0;
            break;
        case 3:
        case 4:
        case 6:
            U8(p1, 0x2f9) = 0;
            break;
        case 5:
        case 7:
            CARDGAME_F0x800a0548(p1, p2);
            break;
        case 8:
            CARDGAME_F0x800a0f4c(p1, p2);
            break;
        case 9:
            CARDGAME_F0x800a1a5c(p1, p2);
            break;
        }
        U8(p1, 0x2f6) = U8(p1, 0x2f8);
        U8(p1, 0x2f8) = U8(p1, 0x2f7);
        U8(p1, 0x2f7) = 0;
    }
    switch (U8(p1, 0x2f8)) {
    case 1:
        switch (U8(p1, 0x2f9)) {
        case 0:
        default:
            U8(SCENE, 0xe9e) = 1;
            U8(p1, 0x2f9) = 1;
            break;
        case 1:
            if (U8(SCENE, 0xe9e) == 2)
                U8(p1, 0x2f7) = 10;
            break;
        }
        break;
    case 2:
        if (CARDGAME_F0x8009f110(p1, p2) != 0)
            U8(p1, 0x2f7) = 10;
        break;
    case 3:
        if (CARDGAME_F0x8009f8c0(p1, p2) != 0)
            U8(p1, 0x2f7) = 10;
        break;
    case 4:
        switch (CARDGAME_F0x8009f910(p1, p2)) {
        case 1:
            U8(p1, 0x2f7) = 10;
            break;
        case 2:
            U8(p1, 0x302) = 1;
            ret = 1;
            break;
        case 3:
            U8(p1, 0x302) = 0;
            ret = 1;
            break;
        }
        break;
    case 5:
    case 7:
        if (CARDGAME_F0x8009fdd8(p1, p2) != 0)
            U8(p1, 0x2f7) = 10;
        break;
    case 6:
        if (CARDGAME_F0x800a0b80(p1, p2) != 0)
            U8(p1, 0x2f7) = 10;
        break;
    case 8:
        switch (CARDGAME_F0x800a0f94(p1, p2)) {
        case 1:
            U8(p1, 0x2f7) = 9;
            break;
        case 2:
            ret = 1;
            break;
        }
        break;
    case 9:
        U8(p1, 0x2f7) = 10;
        break;
    case 10:
        CARDGAME_F0x800a1af8(p1, p2);
        break;
    }
    return ret;
}
