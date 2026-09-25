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
