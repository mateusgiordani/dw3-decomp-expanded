/*
 * STCRDDEK:0x80083314 STCRDDEK_func_80083314
 * 136 bytes at STCRDDEK.PRO offset 0x664 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80083314
 *  Symbols     (none)
 *  Compare     136 bytes from 0x80083314 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80083314
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * All 34 words match Ghidra disasm word-for-word (prologue 27bdffe0 at
 * 0x80083314, terminal jr ra + delay-slot addiu sp at 0x80083394/0x80083398).
 *
 * No Ghidra state change.
 *
 * Behavior (conservative): call the function pointer at obj+0x28 as fp(obj, 1),
 * store 1 at obj+0x10, store arg1 at obj+0x58, then if arg1 is 0 store 0 at
 * obj+0x5c and (0xff00 / arg2) at obj+0x60, else store 0xff00 at obj+0x5c and
 * -(0xff00 / arg2) at obj+0x60. The arg1==0 case is written first so the zero
 * path stays on the fall-through path, as in the original branch layout (bne to
 * the nonzero block). The 0xff00 / arg2 division is the signed MIPS div (div +
 * mflo). Struct layout beyond these five words is unrecovered, so the object
 * stays int32_t. The indirect call is preserved as-is per R15 (owning table
 * unknown; slot +0x28).
 *
 * Precedent: fightstg-800a17c8 (a286 preserves g2952's div-$0 form unchecked;
 * a279 does not).
 *
 * (alt2) psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.86: still 208 bytes; a286 also
 * expands the rd!=0 3-operand div macro with guards, so 2.8.1 cannot reach the
 * bare div+mflo pair with any pinned assembler.
 */

#include <stdint.h>

void STCRDDEK_func_80083314(int32_t arg0, int32_t arg1, int32_t arg2)
{
    (*(void (**)(int32_t, int32_t))(arg0 + 0x28))(arg0, 1);
    *(int32_t *)(arg0 + 0x10) = 1;
    *(int32_t *)(arg0 + 0x58) = arg1;
    if (arg1 == 0) {
        *(int32_t *)(arg0 + 0x5c) = 0;
        *(int32_t *)(arg0 + 0x60) = 0xff00 / arg2;
    } else {
        *(int32_t *)(arg0 + 0x5c) = 0xff00;
        *(int32_t *)(arg0 + 0x60) = -(0xff00 / arg2);
    }
}
