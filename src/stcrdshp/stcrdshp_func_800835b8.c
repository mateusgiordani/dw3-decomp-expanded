/*
 * STCRDSHP:0x800835b8 STCRDSHP_func_800835b8
 * 456 bytes at STCRDSHP.PRO offset 0x908 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800835b8
 *  Symbols     D_80044B38=0x80044b38 D_80048D34=0x80048d34
 *              D_8005CCA8=0x8005cca8 STCRDSHP_func_800835b8=0x800835b8
 *  Compare     456 bytes from 0x800835b8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x800835b8
 */

#include "common/types.h"
extern uint8_t D_80044B38[];
extern uint8_t D_80048D34[];
extern int32_t D_8005CCA8;
void STCRDSHP_func_800835b8(uint8_t *state, uint8_t *objects, int32_t mode) {
    int32_t value, resource, tmp, addr;
    uint8_t *obj, *tbl, *dbase;
    if(mode!=0) {
        tmp=*(int32_t *)(state+0x5c);
        tmp=tmp*2;
        addr=(int32_t)state+tmp;
        value=*(int16_t *)(addr+0x98);
        tbl=D_80044B38;
        resource=(*(int32_t (**)(int32_t))(tbl+0x414))(D_8005CCA8+0x6a);
        obj=*(uint8_t **)(objects+0x24);
        (*(void (**)(void *,int32_t,int32_t))(obj+0x114))(obj,resource,value);
        resource=(*(int32_t (**)(int32_t))(tbl+0x414))(D_8005CCA8+0x32);
        obj=*(uint8_t **)(objects+0x28);
        (*(void (**)(void *,int32_t,int32_t))(obj+0x114))(obj,resource,8);
        dbase=D_80048D34;
        obj=*(uint8_t **)(objects+0x2c);
        (*(void (**)(void *,int32_t,int32_t))(obj+0x118))(obj,0,*(int8_t *)(value+dbase+0x7c));
        obj=*(uint8_t **)(objects+0x2c);
        (*(void (**)(void *,int32_t))(obj+0x148))(obj,1);
        resource=(*(int32_t (**)(int32_t))(tbl+0x414))(D_8005CCA8+0x32);
        obj=*(uint8_t **)(objects+0x30);
        (*(void (**)(void *,int32_t,int32_t))(obj+0x114))(obj,resource,0xf);
        resource=(*(int32_t (**)(int32_t))(tbl+0x414))(D_8005CCA8+0x32);
        obj=*(uint8_t **)(objects+0x34);
        (*(void (**)(void *,int32_t,int32_t))(obj+0x114))(obj,resource,4);
    } else {
        obj=*(uint8_t **)(objects+0x24);
        (*(void (**)(void *,int32_t))(obj+0x144))(obj,0);
        obj=*(uint8_t **)(objects+0x28);
        (*(void (**)(void *,int32_t))(obj+0x144))(obj,0);
        obj=*(uint8_t **)(objects+0x2c);
        (*(void (**)(void *,int32_t))(obj+0x144))(obj,0);
        obj=*(uint8_t **)(objects+0x30);
        (*(void (**)(void *,int32_t))(obj+0x144))(obj,0);
        obj=*(uint8_t **)(objects+0x34);
        (*(void (**)(void *,int32_t))(obj+0x144))(obj,0);
    }
}
