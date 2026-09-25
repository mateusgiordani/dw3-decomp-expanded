/*
 * STCRDSHP:0x80087fac STCRDSHP_func_80087fac
 * 280 bytes at STCRDSHP.PRO offset 0x52fc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80087fac
 *  Symbols     D_80044B38=0x80044b38 D_80044F4C=0x80044f4c
 *              D_8005CCA8=0x8005cca8
 *  Compare     280 bytes from 0x80087fac against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80087fac
 */

#include "common/types.h"

/*
 * Shape: if (flag != 0) run a 3-iteration loop calling the service vector at
 * EXE base D_80044B38 + 0x414 with (D_8005CCA8 + 0x32), then the object method
 * *(obj+0x114)(obj, r, i+5) for the object pointer at ctx+0x14+4*i, then the
 * tail D_80044F4C(D_8005CCA8+0x32) with last argument 4; else run a 3-iteration
 * loop calling *(obj+0x144)(obj, 0), then the same method on ctx+4.
 *
 * First parameter (a0) is never read before overwrite in PAL; callers
 * (0x80088470 and 0x800889b8) pass s3 in a0, so it is kept as an unused void *.
 *
 * Revision 5 (GCC 2.8.1 SN32 / ASPSX 2.79 O2, exact): the object pointers are
 * indexed ((uint8_t **)(ctx + 0x14))[i] instead of walking an explicit pointer.
 *
 * The loop optimizer then derives the pointer itself, placing its setup last in
 * the pre-header and its +4 step right after the load, and hoists the else
 * loop's zero argument; i = 0 stays a separate statement before exebase.
 */
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
