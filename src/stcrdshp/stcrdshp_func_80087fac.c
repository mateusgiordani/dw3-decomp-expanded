#include "common/types.h"

/* STCRDSHP:0x80087fac (280B, 70 instructions)
 * PAL bytes: reference/extracted/pro/stcrdshp.bin base 0x80082cb0 file-off 0x52FC (21244).
 * Boundary: reviewed inventory body-80087fac; prologue addiu sp,-0x28, epilogue
 * jr ra / addiu sp,+0x28; frame 0x28 with ra@0x24 s4@0x20 s3@0x1c s2@0x18 s1@0x14
 * s0@0x10 (s0 save in beqz delay slot). Next function STCRDSHP:0x800880c4
 * starts immediately after (file-off 21524, no gap/overlap).
 * Ghidra: project ddw3-pal-sles-03936 program STCRDSHP, disassembly read-only
 * (no cache mutation); hypothesis cross-checked word-for-word against PAL bytes.
 * Shape: if (flag != 0) run a 3-iteration loop calling the service vector at
 * EXE base D_80044B38 + 0x414 with (D_8005CCA8 + 0x32), then the object method
 * *(obj+0x114)(obj, r, i+5) for the object pointer at ctx+0x14+4*i, then the
 * tail D_80044F4C(D_8005CCA8+0x32) with last argument 4; else run a 3-iteration
 * loop calling *(obj+0x144)(obj, 0), then the same method on ctx+4.
 * First parameter (a0) is never read before overwrite in PAL; callers
 * (0x80088470 and 0x800889b8) pass s3 in a0, so it is kept as an unused void *.
 * Revision 5 (GCC 2.8.1 SN32 / ASPSX 2.79 O2, exact): the object pointers are
 * indexed ((uint8_t **)(ctx + 0x14))[i] instead of walking an explicit pointer.
 * The loop optimizer then derives the pointer itself, placing its setup last
 * in the pre-header and its +4 step right after the load, and hoists the else
 * loop's zero argument; i = 0 stays a separate statement before exebase. */
typedef int32_t (*stcrdshp_svc_t)(int32_t);
typedef void (*stcrdshp_m3_t)(void *, int32_t, int32_t);
typedef void (*stcrdshp_m2_t)(void *, int32_t);

extern int32_t D_8005CCA8;
extern uint8_t D_80044B38[];
extern stcrdshp_svc_t D_80044F4C;

void STCRDSHP_func_80087fac(void *unused, uint8_t *ctx, int32_t flag) {
    uint8_t *exebase;
    int32_t i;
    int32_t r;
    int32_t v;
    uint8_t *obj;

    (void)unused;
    if (flag != 0) {
        i = 0;
        exebase = D_80044B38;
        for (; i < 3; i++) {
            v = ((stcrdshp_svc_t)*(int32_t *)(exebase + 0x414))(D_8005CCA8 + 0x32);
            obj = ((uint8_t **)(ctx + 0x14))[i];
            ((stcrdshp_m3_t)*(void **)(obj + 0x114))(obj, v, i + 5);
        }
        r = D_80044F4C(D_8005CCA8 + 0x32);
        obj = *(uint8_t **)(ctx + 4);
        ((stcrdshp_m3_t)*(void **)(obj + 0x114))(obj, r, 4);
    } else {
        i = 0;
        for (; i < 3; i++) {
            obj = ((uint8_t **)(ctx + 0x14))[i];
            ((stcrdshp_m2_t)*(void **)(obj + 0x144))(obj, 0);
        }
        obj = *(uint8_t **)(ctx + 4);
        ((stcrdshp_m2_t)*(void **)(obj + 0x144))(obj, 0);
    }
}
