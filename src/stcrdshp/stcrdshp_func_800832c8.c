// Rev 5 (anthropic/claude-opus-5-5/s0923c): exact_byte_match, 752/752 B, with
// psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 (base) and link symbols
// --symbol D_80044b38=0x80044b38 --symbol D_8005cca8=0x8005cca8.
// Shapes: if (mode) {...} else {clear} puts the clear path last, as in PAL;
// the resource lookup goes through the symbol table D_80044b38[261]
// (*0x80044f4c) and D_8005cca8 (PAL lui/addiu base in a saved register and
// a shared %hi); the slot id address obj + k*2 is a value-context local
// (addu obj,t). See submissions/stcrdshp-800832c8/strategy-r5-o55.md.
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
