/*
 * STCRDABM:0x8008304c STCRDABM_func_8008304c
 * 200 bytes at STCRDABM.PRO offset 0x39c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008304c
 *  Symbols     F0x8001ebf8=0x8001ebf8 STCRDABM_func_8008304c=0x8008304c
 *  Compare     200 bytes from 0x8008304c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x8008304c
 */

#include "common/types.h"
extern void F0x8001ebf8(void *);
void STCRDABM_func_8008304c(uint8_t *object)
{
    uint8_t work[0x54];
    int32_t index, value;
    int32_t row;
    F0x8001ebf8(work);
    (*(void (**)(int32_t,int32_t))(work+0x3c))(0x140,0x100);
    (*(void (**)(int32_t,int32_t))(work+0x40))(0x300,0x100);
    value=*(int32_t *)(object+0x5c);
    for(row=0; row<2; row++) {
        for(index=0; index<6; index++) {
            if(value>=0x13b) break;
            (*(void (**)(int32_t))(work+0x2c))(value++);
            (*(void (**)(int32_t,int32_t))(work+0x44))(index,row);
            (*(void (**)(void))(work+0x30))();
        }
    }
}
