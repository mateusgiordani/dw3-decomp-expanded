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
