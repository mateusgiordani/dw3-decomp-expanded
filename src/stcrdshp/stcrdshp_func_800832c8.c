/*
 * STCRDSHP:0x800832c8 STCRDSHP_func_800832c8
 * 752 bytes at STCRDSHP.PRO offset 0x618 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800832c8
 *  Symbols     D_80044b38=0x80044b38 D_8005cca8=0x8005cca8
 *              STCRDSHP_func_800832c8=0x800832c8
 *  Compare     752 bytes from 0x800832c8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x800832c8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Shapes: if (mode) {...} else {clear} puts the clear path last, as in PAL; the
 * resource lookup goes through the symbol table D_80044b38[261] (*0x80044f4c)
 * and D_8005cca8 (PAL lui/addiu base in a saved register and a shared %hi); the
 * slot id address obj + k*2 is a value-context local (addu obj,t).
 */

#include "common/types.h"
typedef void (*M2)(int32_t,int32_t);
typedef void (*M3)(int32_t,int32_t,int32_t);
typedef int32_t (*LK)(uint32_t);
extern uint32_t D_80044b38[];
extern uint32_t D_8005cca8;
#define CLEAR(p) (((M2)*(uint32_t *)((p)+0x144))((p),0))
#define LOOKUP(k) (((LK)D_80044b38[261])(D_8005cca8+(k)))
#define W(off) (*(int32_t *)((uint8_t *)obj+(off)))
void STCRDSHP_func_800832c8(void *obj,int32_t *items,int32_t mode)
{
    int32_t i,k,id,p,res;
    uint8_t *e;
    if (mode) {
        for(i=0;i<8;i++) {
            k=W(0x60)*8+i;
            e=(uint8_t *)obj+k*2;
            id=*(int16_t *)(e+0x98);
            if(k<W(0x3c0) && id) {
                res=LOOKUP(0x6a);p=items[i];
                ((M3)*(uint32_t *)(p+0x114))(p,res,id);
            } else CLEAR(items[i]);
        }
        p=items[0x10]; ((M3)*(uint32_t *)(p+0x118))(p,0,W(0x60)+1);
        p=items[0x10]; ((M2)*(uint32_t *)(p+0x148))(p,1);
        res=LOOKUP(0x32);p=items[0x11];((M3)*(uint32_t *)(p+0x114))(p,res,9);
        p=items[0x12];((M3)*(uint32_t *)(p+0x118))(p,0,W(0x64));
        p=items[0x12];((M2)*(uint32_t *)(p+0x148))(p,1);
        if(W(0x64)<2)return;
        if(W(0x60)>0) {
            res=LOOKUP(0x32);p=items[0xe];((M3)*(uint32_t *)(p+0x114))(p,res,10);
        } else CLEAR(items[0xe]);
        if(W(0x60)<W(0x64)-1) {
            res=LOOKUP(0x32);p=items[0xf];((M3)*(uint32_t *)(p+0x114))(p,res,11);
        } else CLEAR(items[0xf]);
    } else {
        for(i=0;i<8;i++) CLEAR(items[i]);
        CLEAR(items[0x10]); CLEAR(items[0x11]); CLEAR(items[0x12]);
        CLEAR(items[0xe]); CLEAR(items[0xf]);
    }
}
