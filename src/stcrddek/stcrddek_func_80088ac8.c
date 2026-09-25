/*
 * STCRDDEK:0x80088ac8 STCRDDEK_F0x80088ac8
 * 108 bytes at STCRDDEK.PRO offset 0x5e18 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80088ac8
 *  Symbols     F0x80014504=0x80014504 STCRDDEK_F0x80088874=0x80088874
 *              STCRDDEK_F0x80088880=0x80088880 STCRDDEK_F0x80088894=0x80088894
 *              STCRDDEK_F0x800888a8=0x800888a8 STCRDDEK_F0x800888b0=0x800888b0
 *              STCRDDEK_func_800888b0=0x800888b0 func_0x80014504=0x80014504
 *  Compare     108 bytes from 0x80088ac8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x80088ac8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Next framed STCRDDEK:0x80088b34 at +0x6c (27bdffc8 addiu sp,sp,-0x38), size
 * 0x6c contiguous, no overlap.
 *
 * Callers: 1 direct jal from STCRDDEK 0x800859ec (UNCONDITIONAL_CALL); caller
 * saves v0 to 0xd0(s3) then dispatches via slots v0+0x8c/0x90 with (a0,
 * 0x125)/(a0, 0x2a) args.
 *
 * Callees: 1 direct cross-module jal to EXE 0x80014504 (a0=0x800888b0, a1=0x9c,
 * a2=0).
 *
 * Slot targets are tiny in-overlay setters at
 * 0x80088874/0x80088880/0x80088894/0x800888a8 (sw a1/a2 to small offsets,
 * return 1); 0x800888b0 is a framed in-overlay function.
 *
 * No Ghidra state change.
 */

#include "common/types.h"

extern void *F0x80014504(int32_t a0, int32_t a1, int32_t a2);
extern void STCRDDEK_F0x800888b0(void);
extern void STCRDDEK_F0x80088874(void);
extern void STCRDDEK_F0x80088880(void);
extern void STCRDDEK_F0x80088894(void);
extern void STCRDDEK_F0x800888a8(void);

void *STCRDDEK_F0x80088ac8(void)
{
    void *v0;

    v0 = F0x80014504((int32_t)STCRDDEK_F0x800888b0, 0x9c, 0);
    *(void **)((uint8_t *)v0 + 0x8c) = (void *)STCRDDEK_F0x80088874;
    *(void **)((uint8_t *)v0 + 0x90) = (void *)STCRDDEK_F0x80088880;
    *(void **)((uint8_t *)v0 + 0x94) = (void *)STCRDDEK_F0x80088894;
    *(void **)((uint8_t *)v0 + 0x98) = (void *)STCRDDEK_F0x800888a8;
    *(int32_t *)((uint8_t *)v0 + 0x50) = 0x1000;
    *(int32_t *)((uint8_t *)v0 + 0x54) = 3;
    return v0;
}
