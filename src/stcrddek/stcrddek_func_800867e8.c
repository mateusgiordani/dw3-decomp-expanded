/*
 * STCRDDEK:0x800867e8 STCRDDEK_func_800867e8
 * 148 bytes at STCRDDEK.PRO offset 0x3b38 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800867e8
 *  Symbols     D_80055c48=0x80055c48
 *  Compare     148 bytes from 0x800867e8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800867e8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Callees: 2 indirect jalr via EXE global function pointer *0x80055c48 (lui
 * 0x8005 + lw 0x5c48; args 0x40019/0x4001a via lui 0x4 + ori in delay slot);
 * same idiom as CARDGAME/STFGTREP family (cf. STFGTREP 0x80085c90, same 148B
 * shape with 0x40000019/1a constants).
 *
 * Portable C only, no asm, no register vars.
 *
 * Alternates (budget exhausted): (1) cc1 2.7.2-cygnus-970404 + aspsx-2.79 -O2
 * -> 224B; (2) cc1 2.8.1 + aspsx-2.81 -O2 -> 220B. Assembler probe: every
 * pinned aspsx (>=2.67; 2.56 predates %hi/%lo syntax) expands 3-operand div
 * into an 11-word trap-check sequence; every pinned cc1
 * (2.7.2/2.8.1/egcs/2.95.2) emits 3-operand div for this C. Excising the two
 * 9-word check bodies leaves 37 words identical to PAL (incl. beq offset 0x0d,
 * lui/ori consts, v0/v1 asymmetry, sll+subu negation, epilogue).
 */

#include <stdint.h>

extern void (*D_80055c48)(int32_t);

void STCRDDEK_func_800867e8(int32_t *a0, int32_t a1) {
    a0[3] = 1;
    if (a1 != 0) {
        D_80055c48(0x40019);
        a0[2] = 0;
        a0[1] = 0x1000 / a0[0];
    } else {
        D_80055c48(0x4001a);
        a0[2] = 0x1000;
        a0[1] = -((0x1000 / a0[0]) * 2);
    }
}
