/*
 * STCRDDEK:0x80086574 STCRDDEK_func_80086574
 * 152 bytes at STCRDDEK.PRO offset 0x38c4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80086574
 *  Symbols     STCRDDEK_func_8008397c=0x8008397c
 *              STCRDDEK_func_80084890=0x80084890
 *              STCRDDEK_func_80085210=0x80085210
 *  Compare     152 bytes from 0x80086574 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80086574
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * state-dispatch tick (152 B, file off 0x38c4 = vaddr - 0x80082cb0).
 *
 * Struct layout intentionally left unrecovered beyond word offsets used here.
 */

#include <stdint.h>

extern void STCRDDEK_func_8008397c(uint32_t *arg0, uint32_t arg1);
extern void STCRDDEK_func_80084890(uint32_t *arg0);
extern void STCRDDEK_func_80085210(uint32_t *arg0, uint32_t arg1);

void STCRDDEK_func_80086574(uint32_t *arg0, uint32_t arg1) {
    int32_t state = (int32_t)arg0[0xc / 4];

    if (state == 1)
        goto state1;
    if (state < 2)
        goto setup;
    if (state < 4)
        goto done;
setup:
    ((void (*)(uint32_t *))arg0[0x38 / 4])(arg0);
    STCRDDEK_func_8008397c(arg0, arg1);
    arg0[0x444 / 4] = 10;
    arg0[0x454 / 4] = 10;
    arg0[0x464 / 4] = 10;
    arg0[0x474 / 4] = 10;
    arg0[0x484 / 4] = 10;
    goto done;
state1:
    STCRDDEK_func_80085210(arg0, arg1);
    STCRDDEK_func_80084890(arg0);
done:;
}
