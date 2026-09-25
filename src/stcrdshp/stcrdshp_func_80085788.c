#include "common/types.h"

typedef struct {
    char pad0[0x2c];
    void (*callback)(void *, int);
} STCRDSHP_85788;

void STCRDSHP_func_80085788(STCRDSHP_85788 *self) {
    self->callback(self, 1);
}
