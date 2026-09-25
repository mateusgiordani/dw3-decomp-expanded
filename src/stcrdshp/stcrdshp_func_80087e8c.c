#include "common/types.h"

/* STCRDSHP:0x80087e8c (288B, 72 instructions)
 * PAL bytes: reference/extracted/pro/stcrdshp.bin base 0x80082cb0 file-off 0x527c (21116)
 * Boundary: prologue addiu sp,-0x28, epilogue jr ra / addiu sp,0x28;
 * frame 0x28 with ra@0x20 s3@0x1c s2@0x18 s1@0x14 s0@0x10.
 * Next function STCRDSHP:0x80087fac starts immediately after (no gap/overlap).
 * Ghidra: project ddw3-pal-sles-03936 program STCRDSHP, disasm/decompile/xrefs
 * read-only (no cache mutation). All 72 words match the PAL bytes.
 * Callers: STCRDSHP:0x8008842c (flag=1), STCRDSHP:0x80088a30 (flag=0),
 * both pass (a0=s3 struct base, a1=s4 object-pointer array, a2=flag).
 * File name does not prove a card-shop-menu role; semantics unconfirmed, names conservative.
 */
extern struct {
    char pad[0x414];
    int32_t (*fn414)(int32_t);
} D_80044B38;
extern int32_t D_8005CCA8;
extern uint32_t D_80048DA0;

void STCRDSHP_func_80087e8c(void *p1, void **objs, int32_t flag) {
    int32_t r;
    void *obj;

    if (flag != 0) {
        r = D_80044B38.fn414(D_8005CCA8 + 0x94);
        obj = objs[0];
        ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))((uint8_t *)obj + 0x114)))(obj, r, *(int32_t *)((uint8_t *)p1 + 100));
        r = D_80044B38.fn414(D_8005CCA8 + 0x32);
        obj = objs[3];
        ((void (*)(void *, int32_t, int32_t))(*(void (**)(void))((uint8_t *)obj + 0x114)))(obj, r, 3);
        obj = objs[4];
        ((void (*)(void *, int32_t, uint32_t))(*(void (**)(void))((uint8_t *)obj + 0x118)))(obj, 0, D_80048DA0);
        obj = objs[4];
        ((void (*)(void *, int32_t))(*(void (**)(void))((uint8_t *)obj + 0x148)))(obj, 1);
    } else {
        obj = objs[0];
        ((void (*)(void *, int32_t))(*(void (**)(void))((uint8_t *)obj + 0x144)))(obj, 0);
        obj = objs[3];
        ((void (*)(void *, int32_t))(*(void (**)(void))((uint8_t *)obj + 0x144)))(obj, 0);
        obj = objs[4];
        ((void (*)(void *, int32_t))(*(void (**)(void))((uint8_t *)obj + 0x144)))(obj, 0);
    }
}
