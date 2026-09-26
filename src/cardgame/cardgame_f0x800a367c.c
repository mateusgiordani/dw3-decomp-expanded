/*
 * CARDGAME:0x800a367c CARDGAME_F0x800a367c
 * 104 bytes at CARDGAME.PRO offset 0x209cc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a367c
 *  Symbols     CARDGAME_F0x800a3548=0x800a3548 CARDGAME_F0x800a3558=0x800a3558
 *              CARDGAME_F0x800a35a4=0x800a35a4 CARDGAME_F0x800a35b0=0x800a35b0
 *              CARDGAME_F0x800a35bc=0x800a35bc EXE_F0x80014504=0x80014504
 *  Compare     104 bytes from 0x800a367c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a367c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x18 (s0, ra).
 *
 * Creates the 0x78-byte fade object handled by CARDGAME_F0x800a35bc (EXE
 * 0x80014504, flags 4), installs its four methods at +0x68..+0x74, stores
 * `mode` at +0x5c and returns the object.
 */

#include <stdint.h>

int32_t EXE_F0x80014504(void *handler, int32_t size, int32_t flags);

int32_t CARDGAME_F0x800a35bc();
int32_t CARDGAME_F0x800a3548();
int32_t CARDGAME_F0x800a3558();
int32_t CARDGAME_F0x800a35a4();
int32_t CARDGAME_F0x800a35b0();

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

int32_t CARDGAME_F0x800a367c(int32_t mode)
{
    int32_t obj;

    obj = EXE_F0x80014504(CARDGAME_F0x800a35bc, 0x78, 4);
    U8(obj, 0x5c) = mode;
    S32(obj, 0x68) = (int32_t)CARDGAME_F0x800a3548;
    S32(obj, 0x6c) = (int32_t)CARDGAME_F0x800a3558;
    S32(obj, 0x70) = (int32_t)CARDGAME_F0x800a35a4;
    S32(obj, 0x74) = (int32_t)CARDGAME_F0x800a35b0;
    return obj;
}
