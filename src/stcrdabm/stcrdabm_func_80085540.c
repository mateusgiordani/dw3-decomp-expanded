/*
 * STCRDABM:0x80085540 STCRDABM_func_80085540
 * 300 bytes at STCRDABM.PRO offset 0x2890 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80085540
 *  Symbols     D0x80048d34=0x80048d34 D0x80085a64=0x80085a64
 *              D0x80085a68=0x80085a68 STCRDABM_func_80083bfc=0x80083bfc
 *              STCRDABM_func_80084500=0x80084500
 *              STCRDABM_func_80084c38=0x80084c38
 *              STCRDABM_func_80085540=0x80085540
 *  Compare     300 bytes from 0x80085540 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80085540
 */

#include "common/types.h"

/*
 * PAL-SLES-03936; reviewed body-80085540, base 0x80082cb0.
 *
 * Dispatches on object phase at +0xc; default path runs the +0x10 state machine
 * through overlay vectors at 0x80085a64/0x80085a68.
 *
 * Forward goto layout reproduces the original block order: default, phase-1
 * service, phase-3 teardown, shared epilogue.
 *
 * A separate byte pointer for the EXE table preserves the PAL high/low address
 * allocation. Exact with PsyQ GCC 2.8.1 / ASPSX 2.79, O2/G0 base.
 *
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
