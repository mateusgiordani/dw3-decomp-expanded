#include "common/types.h"

/* STCRDABM:0x80085540 (300 bytes, 75 instructions)
 * PAL-SLES-03936; reviewed body-80085540, base 0x80082cb0.
 * Task callback registered by STCRDABM_func_8008566c (size 0xd8, mode 0x4c).
 * Dispatches on object phase at +0xc; default path runs the +0x10
 * state machine through overlay vectors at 0x80085a64/0x80085a68.
 * Forward goto layout reproduces the original block order:
 * default, phase-1 service, phase-3 teardown, shared epilogue.
 * A separate byte pointer for the EXE table preserves the PAL high/low
 * address allocation. Exact with PsyQ GCC 2.8.1 / ASPSX 2.79, O2/G0 base.
 * Slot +0x38 is serviced with the object argument.
 */
 extern void STCRDABM_func_80083bfc(int32_t object, int32_t arg);
 extern void STCRDABM_func_80084500(int32_t object);
 extern void STCRDABM_func_80084c38(int32_t object, int32_t arg);
 extern int32_t D0x80048d34[];
 extern void (*D0x80085a64)(void);
 extern int32_t (*D0x80085a68)(void);

 void STCRDABM_func_80085540(int32_t object, int32_t arg) {
     int32_t phase;
     int32_t state;
     int32_t tmp;
     uint8_t *exe;

     phase = *(int32_t *)(object + 0xc);
     if (phase == 1) {
         goto service;
     }
     if (phase < 2) {
         goto idle;
     }
     if (phase == 2) {
         return;
     }
     if (phase == 3) {
         goto teardown;
     }
 idle:
     state = *(int32_t *)(object + 0x10);
     if (state == 0) {
         goto bump;
     }
     if (state == 1) {
         goto ready;
     }
 bump:
     (*D0x80085a64)();
     *(int32_t *)(object + 0x10) = *(int32_t *)(object + 0x10) + 1;
     return;
 ready:
     if ((*D0x80085a68)() != 0) {
         return;
     }
     STCRDABM_func_80083bfc(object, arg);
     *(int32_t *)(object + 0xb8) = 8;
     *(int32_t *)(object + 0xc8) = 8;
     *(int32_t *)(object + 0x64) = 0x1b;
     *(int32_t *)(object + 0x78) = state;
     (*(void (*)(int32_t))*(int32_t *)(object + 0x38))(object);
     return;
 service:
     STCRDABM_func_80084c38(object, arg);
     STCRDABM_func_80084500(object);
     return;
 teardown:
     exe = (uint8_t *)D0x80048d34;
     tmp = (*(int32_t (*)(void))*(int32_t *)(exe + 0x2714))();
     (*(void (*)(int32_t, int32_t))*(int32_t *)(exe + 0x270c))(tmp, 0);
 }
