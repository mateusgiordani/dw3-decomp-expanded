/*
 * STCRDDEK:0x800831f0 STCRDDEK_func_800831f0
 * 248 bytes at STCRDDEK.PRO offset 0x540 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800831f0
 *  Symbols     D_8004DE10=0x8004de10 F0x80014504=0x80014504
 *              STCRDDEK_func_8008a0b8=0x8008a0b8
 *  Compare     248 bytes from 0x800831f0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800831f0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Attempt history on pinned base: (1) single if with || got merged by GCC into
 * (state-1)<u 3 (60-word object, 8 B short); (2) this three-if/goto chain keeps
 * beq/bltz/slti+bne separate and matches. No compiler alternates used.
 */

#include <stdint.h>

extern int32_t D_8004DE10[];
extern int32_t STCRDDEK_func_8008a0b8(void);

void STCRDDEK_func_800831f0(int32_t arg0, int32_t *arg1)
{
    int32_t *tbl;
    int16_t h[4];
    int32_t obj;
    int32_t state;

    state = *(int32_t *)(arg0 + 0xc);
    if (state == 0)
        goto body;
    if (state < 0)
        goto body;
    if (state < 4)
        goto out;
body:
        tbl = D_8004DE10;
        ((void (*)(void))tbl[84])();
        ((void (*)(int32_t))tbl[85])(0xf000);
        ((void (*)(int32_t, int32_t, int32_t, int32_t))tbl[93])(0x140, 0xf0, 0, 0);
        h[0] = 0;
        h[1] = 0;
        h[2] = 0x140;
        h[3] = 0xf0;
        obj = ((int32_t (*)(int16_t *, int32_t, int32_t))tbl[91])(h, 3, 0x1000);
        ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(obj + 0x12c))(obj, 0, 0, 0);
        *arg1 = STCRDDEK_func_8008a0b8();
        ((void (*)(int32_t))*(void **)(arg0 + 0x38))(arg0);
out:
    return;
}
