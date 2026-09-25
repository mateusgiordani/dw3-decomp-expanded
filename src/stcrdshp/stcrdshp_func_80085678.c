/*
 * STCRDSHP:0x80085678 STCRDSHP_func_80085678
 * 184 bytes at STCRDSHP.PRO offset 0x29c8 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80085678
 *  Symbols     F0x8001ebf8=0x8001ebf8
 *  Compare     184 bytes from 0x80085678 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80085678
 */

#include "common/types.h"
extern void F0x8001ebf8(void *);
void STCRDSHP_func_80085678(uint8_t *object)
{
    uint8_t work[0x54];
    int32_t index, value;
    int32_t *entry;
    F0x8001ebf8(work);
    (*(void (**)(int32_t,int32_t))(work+0x3c))(0x140,0x100);
    (*(void (**)(int32_t,int32_t))(work+0x40))(0x300,0x100);
    entry=(int32_t *)(object+0x7c);
    for(index=0; index<6; index++) {
        value=*entry;
        if((uint32_t)(value-1)>=0x13a) return;
        (*(void (**)(int32_t))(work+0x2c))(value);
        entry++;
        (*(void (**)(int32_t,int32_t))(work+0x44))(0,index);
        (*(void (**)(void))(work+0x30))();
    }
}
