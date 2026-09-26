/*
 * CARDGAME:0x800a3240 CARDGAME_F0x800a3240
 * 152 bytes at CARDGAME.PRO offset 0x20590 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800a3240
 *  Symbols     CARDGAME_F0x8009eefc=0x8009eefc CARDGAME_F0x8009f04c=0x8009f04c
 *              CARDGAME_F0x800a0708=0x800a0708 CARDGAME_F0x800a0754=0x800a0754
 *              CARDGAME_F0x800a2f00=0x800a2f00 CARDGAME_F0x800a3934=0x800a3934
 *              D_80055C54=0x80055c54 EXE_F0x80014504=0x80014504
 *  Compare     152 bytes from 0x800a3240 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800a3240
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0x20 (s0-s1, ra).
 *
 * Creates the 0x824-byte object handled by CARDGAME_F0x800a2f00 (EXE
 * 0x80014504, flags 0x1c), installs its five methods at +0x810..+0x820, stores
 * `id` at +0x2e8, calls the EXE vector at 0x80055c54 with 0x27 and returns the
 * object.
 */

#include <stdint.h>

typedef void (*vec_t)(int32_t);

int32_t EXE_F0x80014504(void *handler, int32_t size, int32_t flags);
extern vec_t D_80055C54;

int32_t CARDGAME_F0x800a2f00();
int32_t CARDGAME_F0x8009f04c();
int32_t CARDGAME_F0x8009eefc();
int32_t CARDGAME_F0x800a0754();
int32_t CARDGAME_F0x800a0708();
int32_t CARDGAME_F0x800a3934();

#define U8(p, o)     (*(uint8_t *)((p) + (o)))
#define S32(p, o)    (*(int32_t *)((p) + (o)))

int32_t CARDGAME_F0x800a3240(int32_t id)
{
    int32_t obj;

    obj = EXE_F0x80014504(CARDGAME_F0x800a2f00, 0x824, 0x1c);
    S32(obj, 0x810) = (int32_t)CARDGAME_F0x8009f04c;
    S32(obj, 0x814) = (int32_t)CARDGAME_F0x8009eefc;
    S32(obj, 0x818) = (int32_t)CARDGAME_F0x800a0754;
    S32(obj, 0x81c) = (int32_t)CARDGAME_F0x800a0708;
    S32(obj, 0x820) = (int32_t)CARDGAME_F0x800a3934;
    U8(obj, 0x2e8) = id;
    D_80055C54(0x27);
    return obj;
}
