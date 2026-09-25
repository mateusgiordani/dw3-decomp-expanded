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
