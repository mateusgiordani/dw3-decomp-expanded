/*
 * STCRDDEK:0x8008666c STCRDDEK_F0x8008666c
 * 60 bytes at STCRDDEK.PRO offset 0x39bc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008666c
 *  Symbols     EXE_F0x8001f648=0x8001f648
 *  Compare     60 bytes from 0x8008666c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x8008666c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * frame 0xb8 (s0, ra).
 *
 * Fills a 0xa0-byte system info block (EXE 0x8001f648) and calls its method at
 * +0x7c with the object's words at +0x54 and +0x58.
 */

#include <stdint.h>

typedef struct {
    int32_t pad0[31];
    void (*fn)(int32_t, int32_t);
    int32_t pad80[8];
} SysInfo;

void EXE_F0x8001f648(SysInfo *out);

#define S32(p, o)    (*(int32_t *)((p) + (o)))

void STCRDDEK_F0x8008666c(int32_t p1)
{
    SysInfo info;

    EXE_F0x8001f648(&info);
    info.fn(S32(p1, 0x54), S32(p1, 0x58));
}
