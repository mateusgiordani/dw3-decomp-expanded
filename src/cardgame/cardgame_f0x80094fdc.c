/*
 * CARDGAME:0x80094fdc CARDGAME_F0x80094fdc
 * 84 bytes at CARDGAME.PRO offset 0x1232c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80094fdc
 *  Symbols     (none)
 *  Compare     84 bytes from 0x80094fdc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80094fdc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Decompile (Ghidra, hypothesis only): *(param_1 + 0x424) = 0; (**(code
 * **)(param_2 + 0xee4))(param_2, param_3, 0, 0, 1); *(param_1 + 0x422) = 1.
 *
 * Words: 21/21 PAL words match reference (next word 27bdffd8 =
 * CARDGAME:0x80095030 prologue, contiguous, size 0x54 confirmed
 * self-contained).
 *
 * Caller/callee: 3 callers, 0 direct callees (indirect jalr via v0 =
 * *(cbctx+0xee4)).
 *
 * Keep the callback load in the call expression: a separate fn local moves it
 * before the outgoing stack argument and changes the native delay slot.
 */

typedef void (*cardgame_indirect_80094fdc_t)(void *arg0, int arg1, int arg2, int arg3, int arg4);

void CARDGAME_F0x80094fdc(void *ctx, void *cbctx, int param) {
    ((int *)ctx)[0x424 / 4] = 0;
    (*(cardgame_indirect_80094fdc_t *)((char *)cbctx + 0xEE4))(cbctx, param, 0, 0, 1);
    ((unsigned char *)ctx)[0x422] = 1;
}
