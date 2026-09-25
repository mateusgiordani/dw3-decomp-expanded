#include "common/types.h"
typedef struct { int16_t key; int16_t value; } CardTableEntry;
extern CardTableEntry D_8008CD20[];
int32_t STCRDSHP_func_8008923c(int32_t value) {
    int32_t index;
    for (index = 0; D_8008CD20[index].key != 0; index++) {
        if (D_8008CD20[index].key == value) return D_8008CD20[index].value;
    }
    return 1;
}
