/*
 * STCRDSHP:0x80087a2c STCRDSHP_func_80087a2c
 * 344 bytes at STCRDSHP.PRO offset 0x4d7c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80087a2c
 *  Symbols     D_8008CB98=0x8008cb98 STCRDSHP_func_80085fdc=0x80085fdc
 *              STCRDSHP_func_80086074=0x80086074
 *              STCRDSHP_func_8008674c=0x8008674c
 *              STCRDSHP_func_80086dc4=0x80086dc4
 *  Compare     344 bytes from 0x80087a2c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x80087a2c
 */

#include "common/types.h"

extern void *D_8008CB98;
extern int32_t STCRDSHP_func_80085fdc(int32_t arg0, void *arg1);
extern void STCRDSHP_func_80086074(void *arg0, void *arg1);
extern void STCRDSHP_func_8008674c(void *arg0);
extern void STCRDSHP_func_80086dc4(void *arg0, void *arg1);

void STCRDSHP_func_80087a2c(uint8_t *obj, uint8_t *ctx) {
    int32_t state;
    int32_t count;
    int32_t i;
    uint8_t *p;
    int32_t r;

    state = *(int32_t *)(obj + 0xc);
    if (state == 1) {
        goto case1;
    }
    if (state < 2) {
        goto init;
    }
    if (state == 2) {
        goto case2;
    }
    if (state == 3) {
        return;
    }
init:
    (*(void (*)(void *))*(void **)(obj + 0x38))(obj);
    *(void **)(obj + 0xa8) = (*(void *(*)(int32_t))D_8008CB98)(*(int32_t *)(obj + 0xa0));
    i = 0;
    p = obj;
    do {
        int32_t j = i * 2;
        int32_t b = *(int32_t *)(*(int32_t *)(obj + 0xa8) + 8);
        *(int32_t *)(p + 0x74) = (int32_t)*(int16_t *)(j + b);
        i += 1;
        p += 4;
    } while (i < 6);
    count = *(int32_t *)(*(int32_t *)(obj + 0xa8) + 4);
    r = count % 6;
    *(int32_t *)(obj + 0xa4) = count;
    /* resto primeiro + q-(-flag): formacao de quantidade e addu PAL v0,v1,v0 */
    r = (r != 0);
    r = count / 6 - -r;
    *(int32_t *)(obj + 0x70) = r;
    *(int32_t *)(ctx + 0x48) = STCRDSHP_func_80085fdc(*(int32_t *)(obj + 0x50), obj + 0x74);
    STCRDSHP_func_80086074(obj, ctx);
    *(int32_t *)(obj + 0xac) = 10;
    *(int32_t *)(obj + 0xbc) = 10;
    *(int32_t *)(obj + 0xcc) = 10;
    return;
case1:
    STCRDSHP_func_80086dc4(obj, ctx);
    STCRDSHP_func_8008674c(obj);
    return;
case2:
    if (*(int32_t *)(ctx + 0x48) != 0) {
        return;
    }
    *(int32_t *)(obj + 0xc) = 3;
    return;
}
