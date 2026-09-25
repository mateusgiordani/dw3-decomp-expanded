/*
 * STCRDDEK:0x800887d4 STCRDDEK_F0x800887d4
 * 160 bytes at STCRDDEK.PRO offset 0x5b24 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800887d4
 *  Symbols     F0x80014504=0x80014504 STCRDDEK_F0x800884e8=0x800884e8
 *              STCRDDEK_F0x80088628=0x80088628 STCRDDEK_F0x80088634=0x80088634
 *              STCRDDEK_F0x800886c8=0x800886c8 STCRDDEK_F0x800887c8=0x800887c8
 *              STCRDDEK_func_800884e8=0x800884e8
 *              STCRDDEK_func_80088628=0x80088628
 *              STCRDDEK_func_80088634=0x80088634 func_0x80014504=0x80014504
 *  Compare     160 bytes from 0x800887d4 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x800887d4
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * STCRDDEK_func_80088634(v0, param); STCRDDEK_func_80088628 (v0, 0x280, 0x100);
 * return v0.
 *
 * Callers: 1 direct jal from STCRDDEK 0x80089dfc (UNCONDITIONAL_CALL, delay
 * addu a0,v0,a0); caller stores v0 to 0x0(s3), then dispatches via slot
 * 0x28(a0) with a1 = 2.
 *
 * Callees: cross-module jal to EXE 0x80014504 (a0 = 0x800884e8, a1 = 0xf4, a2 =
 * 0x34); in-overlay jals to STCRDDEK 0x80088634 (framed, a0 = obj, a1 = param)
 * and 0x80088628 (leaf setter: sw a1,0x5c(a0) / sw a2,0x60(a0)).
 *
 * 0x800884e8/0x800886c8 are framed in-overlay functions; 0x800887c8 is a leaf
 * setter (li v0,0xa; jr; sw v0,0x10(a0)) immediately before this prologue.
 *
 * No Ghidra state change.
 */

#include "common/types.h"

extern void *F0x80014504(int32_t a0, int32_t a1, int32_t a2);
extern void STCRDDEK_F0x800884e8(void);
extern void STCRDDEK_F0x800886c8(void);
extern void STCRDDEK_F0x800887c8(void);
extern void STCRDDEK_F0x80088634(void *a0, int32_t a1);
extern void STCRDDEK_F0x80088628(void *a0, int32_t a1, int32_t a2);

void *STCRDDEK_F0x800887d4(int32_t a0)
{
    void *v0;

    v0 = F0x80014504((int32_t)STCRDDEK_F0x800884e8, 0xf4, 0x34);
    *(void **)((uint8_t *)v0 + 0xec) = (void *)STCRDDEK_F0x800886c8;
    *(void **)((uint8_t *)v0 + 0xf0) = (void *)STCRDDEK_F0x800887c8;
    *(int32_t *)((uint8_t *)v0 + 0x54) = 0x1000;
    *(int32_t *)((uint8_t *)v0 + 0x58) = 3;
    *(int32_t *)((uint8_t *)v0 + 0x50) = 2;
    *(int32_t *)((uint8_t *)v0 + 0x94) = 10;
    *(int32_t *)((uint8_t *)v0 + 0x64) = -1;
    STCRDDEK_F0x80088634(v0, a0);
    STCRDDEK_F0x80088628(v0, 0x280, 0x100);
    return v0;
}
