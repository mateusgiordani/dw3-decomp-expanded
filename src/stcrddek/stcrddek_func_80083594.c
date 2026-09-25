/*
 * STCRDDEK:0x80083594 STCRDDEK_func_80083594
 * 68 bytes at STCRDDEK.PRO offset 0x8e4 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80083594
 *  Symbols     F0x80014504=0x80014504 STCRDDEK_func_80083314=0x80083314
 *              STCRDDEK_func_800834e0=0x800834e0 func_0x80014504=0x80014504
 *  Compare     68 bytes from 0x80083594 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80083594
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * No Ghidra state change.
 *
 * Callees: 1 direct cross-module jal to EXE 0x80014504 (a0=0x800834e0
 * callback-arg, a1=0x68, a2=0). Stored words 0x80083314/0x800834e0 confirmed as
 * in-overlay code pointers; owning struct layout and allocator arg roles left
 * unrecovered.
 *
 * Pattern matches sibling allocator-wrappers (STCRDDEK 0x80088ac8, STSTATUS
 * 0x80084f10).
 */

#include "common/types.h"

extern void *F0x80014504(int32_t a0, int32_t a1, int32_t a2);
extern void STCRDDEK_func_800834e0(void);
extern void STCRDDEK_func_80083314(void);

void STCRDDEK_func_80083594(void)
{
    void *v0;

    v0 = F0x80014504((int32_t)STCRDDEK_func_800834e0, 0x68, 0);
    *(void **)((uint8_t *)v0 + 0x64) = (void *)STCRDDEK_func_80083314;
    *(int32_t *)((uint8_t *)v0 + 0x50) = 0x1000;
    *(int32_t *)((uint8_t *)v0 + 0x54) = 0;
}
