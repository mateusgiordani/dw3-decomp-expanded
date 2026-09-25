/*
 * STCRDABM:0x800840ac STCRDABM_func_800840ac
 * 1108 bytes at STCRDABM.PRO offset 0x13fc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800840ac
 *  Symbols     D0x80044b38=0x80044b38 D0x80044f4c=0x80044f4c
 *              D0x80048d34=0x80048d34 D0x8005cca8=0x8005cca8
 *              F0x8001ebf8=0x8001ebf8 STCRDABM_func_800840ac=0x800840ac
 *  Compare     1108 bytes from 0x800840ac against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x800840ac
 */

#include "common/types.h"

/*
 * PAL-SLES-03936, 1108-byte body at module base 0x80082cb0. Callers
 * 800852d0/80085390/80085454 discard the return value.
 *
 * EXE:8001ebf8 initializes 0x54 bytes: data pointer +0, callbacks +0x2c/+0x50.
 *
 * Widget methods are reloaded after each call.
 *
 * Normal service calls use vector 80044b38+414; special-ID path uses the
 * absolute 80044f4c slot. These are two views of the same PAL pointer.
 *
 * Table +0x4df is unsigned presence; +0x3a2 is a signed byte value.
 */
extern void F0x8001ebf8(void *);
extern void *(*D0x80044f4c)(int32_t);
extern int32_t D0x8005cca8;
extern uint8_t D0x80044b38[];
extern uint8_t D0x80048d34[];

/* Keep method address and arguments in one expression: a sequenced widget
 * temporary changes the helper-result lifetime and the next call's schedule. */
#define STCRDABM_W3(ws, o, m, a, b) ((*(void (**)(void *, void *, int32_t))((char *)*(int32_t *)((ws) + (o)) + (m)))((void *)*(int32_t *)((ws) + (o)), (void *)(a), (int32_t)(b)))
#define STCRDABM_W2(ws, o, m, a) ((*(void (**)(void *, int32_t))((char *)*(int32_t *)((ws) + (o)) + (m)))((void *)*(int32_t *)((ws) + (o)), (int32_t)(a)))

void STCRDABM_func_800840ac(int32_t state, int32_t widgets, int32_t flag) {
    int32_t base;
    int32_t off;
    int32_t slot;
    uint8_t *table;
    uint32_t buf[21];
    void (*cb)(int32_t);
    void *res;
    uint8_t *services;

    base = *(int32_t *)(state + 0x60);
    off = *(int32_t *)(state + 0x74);
    slot = (((base << 1) + base) << 2) + off + 1;
    *(int32_t *)(state + 0x78) = slot;
    table = D0x80048d34;
    if (table[slot + 0x4df] != 0 && flag != 0) {
        F0x8001ebf8(buf);
        cb = (void (*)(int32_t))buf[11];
        cb((*(int32_t *)(state + 0x78)));
        services = D0x80044b38;
        res = (*(void *(**)(int32_t))(services + 0x414))(D0x8005cca8 + 0x16);
        STCRDABM_W3(widgets, 0x1c, 0x114, res, (*(int32_t *)(state + 0x78)));
        res = (*(void *(**)(int32_t))(services + 0x414))(D0x8005cca8 + 0x24);
        STCRDABM_W3(widgets, 0x28, 0x114, res, 8);
        STCRDABM_W3(widgets, 0x2c, 0x118, 0, ((int8_t *)table)[(*(int32_t *)(state + 0x78)) + 0x3a2]);
        STCRDABM_W2(widgets, 0x2c, 0x148, 1);
        if (((int32_t (*)(void))buf[20])() != 0) {
            STCRDABM_W2(widgets, 0x20, 0x144, 0);
            STCRDABM_W2(widgets, 0x24, 0x144, 0);
            res = (*(void *(**)(int32_t))(services + 0x414))(D0x8005cca8 + 0x1d);
            STCRDABM_W3(widgets, 0x30, 0x114, res, (*(int32_t *)(state + 0x78)));
            STCRDABM_W2(widgets, 0x34, 0x144, 0);
            STCRDABM_W2(widgets, 0x38, 0x144, 0);
            STCRDABM_W2(widgets, 0x3c, 0x144, 0);
            STCRDABM_W2(widgets, 0x40, 0x144, 0);
            return;
        } else {
            res = (*(void *(**)(int32_t))(services + 0x414))(D0x8005cca8 + 0x24);
            STCRDABM_W3(widgets, 0x20, 0x114, res, 8);
            STCRDABM_W3(widgets, 0x24, 0x118, 0, ((uint8_t *)buf[0])[5]);
            STCRDABM_W2(widgets, 0x24, 0x148, 1);
            if ((*(int32_t *)(state + 0x78)) == 0x45 || (*(int32_t *)(state + 0x78)) == 0x70 || (*(int32_t *)(state + 0x78)) == 0x9b || (*(int32_t *)(state + 0x78)) == 0xc6 || (*(int32_t *)(state + 0x78)) == 0xf1) {
                res = D0x80044f4c(D0x8005cca8 + 0x1d);
                STCRDABM_W3(widgets, 0x30, 0x114, res, (*(int32_t *)(state + 0x78)));
                STCRDABM_W2(widgets, 0x34, 0x144, 0);
                STCRDABM_W2(widgets, 0x38, 0x144, 0);
                STCRDABM_W2(widgets, 0x3c, 0x144, 0);
                STCRDABM_W2(widgets, 0x40, 0x144, 0);
                return;
            } else {
                STCRDABM_W2(widgets, 0x30, 0x144, 0);
                res = (*(void *(**)(int32_t))(services + 0x414))(D0x8005cca8 + 0x24);
                STCRDABM_W3(widgets, 0x34, 0x114, res, 6);
                STCRDABM_W3(widgets, 0x38, 0x118, 0, ((uint8_t *)buf[0])[1]);
                STCRDABM_W2(widgets, 0x38, 0x148, 1);
                res = (*(void *(**)(int32_t))(services + 0x414))(D0x8005cca8 + 0x24);
                STCRDABM_W3(widgets, 0x3c, 0x114, res, 7);
                STCRDABM_W3(widgets, 0x40, 0x118, 0, ((uint8_t *)buf[0])[2]);
                STCRDABM_W2(widgets, 0x40, 0x148, 1);
                return;
            }
        }
    } else {
        STCRDABM_W2(widgets, 0x1c, 0x144, 0);
        STCRDABM_W2(widgets, 0x28, 0x144, 0);
        STCRDABM_W2(widgets, 0x2c, 0x144, 0);
        STCRDABM_W2(widgets, 0x20, 0x144, 0);
        STCRDABM_W2(widgets, 0x24, 0x144, 0);
        STCRDABM_W2(widgets, 0x30, 0x144, 0);
    }
    STCRDABM_W2(widgets, 0x34, 0x144, 0);
    STCRDABM_W2(widgets, 0x38, 0x144, 0);
    STCRDABM_W2(widgets, 0x3c, 0x144, 0);
    STCRDABM_W2(widgets, 0x40, 0x144, 0);
}
