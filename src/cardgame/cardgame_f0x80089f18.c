/*
 * CARDGAME:0x80089f18 CARDGAME_F0x80089f18
 * 336 bytes at CARDGAME.PRO offset 0x7268 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80089f18
 *  Symbols     CARDGAME_F0x80089f18=0x80089f18
 *  Compare     336 bytes from 0x80089f18 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80089f18
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * full 336-byte PAL match with GCC 2.8.1 / ASPSX 2.79 O2 G0.
 *
 * Materialize each scaled index before forming the memory address: this keeps
 * base-first addu operand order with destination v0.
 */

#include <stdint.h>

typedef void (*cardgame_exe_vec_t)(uint32_t);

void CARDGAME_F0x80089f18(int32_t s, int32_t c, int32_t kind, int32_t delta) {
    int32_t off = 0;

    ((cardgame_exe_vec_t)*(uint32_t *)0x80055c48)(0x4001b);
    switch (kind) {
    case 1:
        off = 6;
        break;
    case 2:
        off = 12;
        break;
    }
    {
        int32_t k = (off + *(int32_t *)(s + 0x43c)) * 76;
        *(uint8_t *)(c + k + 0x150) &= 0xfe;
    }
    {
        int32_t k = (off + *(int32_t *)(s + 0x43c)) * 76;
        *(int16_t *)(c + k + 0x12e) = 0;
    }
    *(int32_t *)(s + 0x43c) += delta;
    {
        int32_t k = (off + *(int32_t *)(s + 0x43c)) * 76;
        *(uint8_t *)(c + k + 0x150) |= 1;
    }
    {
        int32_t k = (off + *(int32_t *)(s + 0x43c)) * 76;
        *(int16_t *)(c + k + 0x12e) = 1;
    }
}
