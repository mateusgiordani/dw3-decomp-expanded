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
