/*
 * STCRDSHP:0x80085bac STCRDSHP_func_80085bac
 * 300 bytes at STCRDSHP.PRO offset 0x2efc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80085bac
 *  Symbols     D_80044B38=0x80044b38 F0x8001f648=0x8001f648
 *  Compare     300 bytes from 0x80085bac against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80085bac
 */

#include "common/types.h"
extern void F0x8001f648(void *);
extern int32_t D_80044B38[];
void STCRDSHP_func_80085bac(uint8_t *object) {
    uint8_t work[0xa0];
    int32_t *exe;
    int32_t index, value;
    int32_t col, row;
    F0x8001f648(work);
    (*(void (**)(int32_t,int32_t))(work+0x7c))(*(int32_t *)(object+0x54),*(int32_t *)(object+0x58)-1);
    (*(void (**)(int32_t,int32_t))(work+0x74))(0x280,0);
    for(index=0; index<*(int32_t *)(object+0x68); index++) {
        exe = D_80044B38;
        col=index/6;
        row=col;
        col=index-row*6;
        (*(void (**)(int32_t))(work+0x88))(*(int32_t *)(object+0x70));
        value=((int32_t (*)(int32_t))exe[0x109])(0x063e0000);
        (*(void (**)(int32_t,int32_t,int32_t,int32_t))(work+0x84))(value,6,col*0x2a+0x23,row*0x36+0x44);
    }
}
