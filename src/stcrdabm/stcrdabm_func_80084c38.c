/*
 * STCRDABM:0x80084c38 STCRDABM_func_80084c38
 * 2312 bytes at STCRDABM.PRO offset 0x1f88 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80084c38, jump table (.rodata) at 0x80082cb0
 *  Symbols     D0x80055C48=0x80055c48 D0x80085A6C=0x80085a6c
 *              D0x80085A70=0x80085a70 DAT_8004B7D0=0x8004b7d0
 *              DAT_80085A64=0x80085a64 STCRDABM_func_80083008=0x80083008
 *              STCRDABM_func_80083a78=0x80083a78
 *              STCRDABM_func_80083e14=0x80083e14
 *              STCRDABM_func_800840ac=0x800840ac
 *              STCRDABM_func_80084bc8=0x80084bc8
 *              STCRDABM_func_80084c38=0x80084c38
 *  Compare     2312 bytes from 0x80084c38 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80084c38
 */

#include "common/types.h"

/* STCRDABM:0x80084c38, PAL-SLES-03936, 2312-byte body.
 * Exact with PsyQ GCC 2.8.1 SN32 4.0.0010 / ASPSX 2.79, base O2 G0.
 * The 54-entry switch table is linked at 0x80082cb0 and checked separately.
 * See the r7 submission for PAL/Ghidra evidence, controls and replay commands.
 *
 * DAT_8004B7D0 offsets 0x3f4/0x3f8/0x3fc/0x408 hold indirect callbacks.
 * DAT_80085A64 offsets 8/12 hold the two overlay callbacks. Their semantic
 * roles and object/context layouts remain unconfirmed; offsets are retained.
 * The service argument 0x800450bd is the full PAL lui/ori value.
 *
 * Context remains a parameter: a separate cast-local changes s3/s4 allocation.
 * Keep the case-15 loop-local index/offset/value and the explicit for/break;
 * a while or backward goto produces different instructions with this compiler.
 */

extern void STCRDABM_func_80084bc8(void *obj);
extern int32_t STCRDABM_func_80083a78(void *obj);
extern void STCRDABM_func_80083e14(void *obj, void *ctx, int32_t mode);
extern void STCRDABM_func_800840ac(void *obj, void *ctx, int32_t mode);
extern int32_t STCRDABM_func_80083008(void);
extern int32_t DAT_8004B7D0[];
extern void (*D0x80055C48)(int32_t code);
extern int32_t DAT_80085A64[];


void STCRDABM_func_80084c38(void *obj_arg, uint8_t *ctx) {
    uint8_t *obj = (uint8_t *)obj_arg;
    int32_t state;
    int32_t saved;
    int32_t cur;
    int32_t i;
    int32_t fresh;
    uint8_t *entry;

    state = *(int32_t *)(obj + 0x10);
    switch (state) {
    case 0:
    default:
        /* 0x80084c8c: default path (also states 2, 5..9, 0xc..0xe,
         * 0x10..0x31 and every state >= 0x36). */
        ((int32_t (*)(void *, int32_t))DAT_80085A64[2])(obj + 0xb8, 1);
        STCRDABM_func_80084bc8(obj);
        *(int32_t *)(ctx + 0x44) = STCRDABM_func_80083a78(obj);
        goto inc_state;
    case 1:
        /* 0x80084cbc. */
        if (((int32_t (*)(void *))DAT_80085A64[3])(obj + 0xb8) == 0)
            return;
        STCRDABM_func_80083e14(obj, ctx, 1);
        if (*(int32_t *)(obj + 0x7c) != 0)
            ((int32_t (*)(void *, int32_t))DAT_80085A64[2])(obj + 0xc8, 1);
        *(int32_t *)(obj + 0x10) = 0xb;
        return;
    case 3:
        /* 0x80084d14. */
        *(int32_t *)(obj + 0x68) = 1;
        *(int32_t *)(obj + 0x10) = *(int32_t *)(obj + 0x10) + 1;
        return;
    case 4:
        /* 0x80084d2c: input-driven 0x60/0x74 adjustment. */
        saved = *(int32_t *)(obj + 0x60);
        if ((!((((int32_t (*)(int32_t))DAT_8004B7D0[0xfe])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 11)) & 1) && ((((int32_t (*)(int32_t))DAT_8004B7D0[0xfd])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 10)) & 1)) ||
            (!((((int32_t (*)(int32_t))DAT_8004B7D0[0xfe])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 11)) & 1) && ((((int32_t (*)(int32_t))DAT_8004B7D0[0xff])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 10)) & 1))) {
            *(int32_t *)(obj + 0x60) -= 1;
            if (*(int32_t *)(obj + 0x60) < 0) *(int32_t *)(obj + 0x60) = 0;
        } else if ((!((((int32_t (*)(int32_t))DAT_8004B7D0[0xfe])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 10)) & 1) && ((((int32_t (*)(int32_t))DAT_8004B7D0[0xfd])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 11)) & 1)) ||
                   (!((((int32_t (*)(int32_t))DAT_8004B7D0[0xfe])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 10)) & 1) && ((((int32_t (*)(int32_t))DAT_8004B7D0[0xff])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 11)) & 1))) {
            *(int32_t *)(obj + 0x60) += 1;
            if (*(int32_t *)(obj + 0x64) - 1 < *(int32_t *)(obj + 0x60))
                *(int32_t *)(obj + 0x60) = *(int32_t *)(obj + 0x64) - 1;
        }
        /* 0x80084f38. */
        if (saved != *(int32_t *)(obj + 0x60)) {
            D0x80055C48(0x4001b);
            *(int32_t *)(obj + 0x68) = 0;
            *(int32_t *)(obj + 0x74) = 0;
            STCRDABM_func_80084bc8(obj);
            STCRDABM_func_80083e14(obj, ctx, 1);
            if (*(int32_t *)(obj + 0xd0) == 0) {
                if (*(int32_t *)(obj + 0x7c) != 0) {
                    *(int32_t *)(obj + 0x10) = 10;
                    *(int32_t *)(obj + 0x14) = 1;
                    entry = *(uint8_t **)(ctx + 0x44);
                    ((void (*)(uint8_t *, int32_t))*(void **)(entry + 0x84))(
                        entry, (*(int32_t *)(obj + 0x60) * 0xc) | 1);
                    goto tail;
                }
            } else {
                entry = *(uint8_t **)(ctx + 0x44);
                ((void (*)(uint8_t *, int32_t))*(void **)(entry + 0x84))(
                    entry, (*(int32_t *)(obj + 0x60) * 0xc) | 1);
                if (*(int32_t *)(obj + 0x7c) == 0) {
                    *(int32_t *)(obj + 0x10) = 10;
                    *(int32_t *)(obj + 0x14) = 0;
                    goto tail;
                }
            }
            *(int32_t *)(obj + 0x10) = 0xf;
            goto tail;
        }
        saved = *(int32_t *)(obj + 0x74);
        if (((((int32_t (*)(int32_t))DAT_8004B7D0[0xfd])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 4)) & 1)) {
            if (*(int32_t *)(obj + 0x74) > 5)
                *(int32_t *)(obj + 0x74) -= 6;
        } else if (((((int32_t (*)(int32_t))DAT_8004B7D0[0xfd])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 6)) & 1)) {
            if (*(int32_t *)(obj + 0x74) < 6)
                *(int32_t *)(obj + 0x74) += 6;
        }
        if (((((int32_t (*)(int32_t))DAT_8004B7D0[0xfd])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 7)) & 1) || ((((int32_t (*)(int32_t))DAT_8004B7D0[0xff])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 7)) & 1)) {
            *(int32_t *)(obj + 0x74) -= 1;
            if (*(int32_t *)(obj + 0x74) < 0)
                *(int32_t *)(obj + 0x74) = 0;
        } else if (((((int32_t (*)(int32_t))DAT_8004B7D0[0xfd])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 5)) & 1) || ((((int32_t (*)(int32_t))DAT_8004B7D0[0xff])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 5)) & 1)) {
            *(int32_t *)(obj + 0x74) += 1;
            if (*(int32_t *)(obj + 0x74) > 11)
                *(int32_t *)(obj + 0x74) = 11;
        }
        if (*(int32_t *)(obj + 0x60) * 0xc + *(int32_t *)(obj + 0x74) > 0x139)
            *(int32_t *)(obj + 0x74) =
                0x139 - *(int32_t *)(obj + 0x60) * 0xc;
        cur = *(int32_t *)(obj + 0x74);
        if (saved == cur)
            goto tail;
        i = cur;
        *(int32_t *)(obj + 0x74) = -1;
        if (i < saved) {
            uint8_t *slot;
            if (i >= 0) {
                slot = (uint8_t *)(i * 4 + (uint32_t)obj);
                do {
                    if (*(int32_t *)(slot + 0x80) != 0)
                        goto found;
                    i--;
                    slot -= 4;
                } while (i >= 0);
            }
            goto scan_done;
        found:
            *(int32_t *)(obj + 0x74) = i;
            goto scan_done;
        } else if (saved < i) {
            uint8_t *slot;
            if (i < 12) {
                slot = (uint8_t *)(i * 4 + (uint32_t)obj);
                do {
                    if (*(int32_t *)(slot + 0x80) != 0)
                        goto found;
                    i++;
                    slot += 4;
                } while (i < 12);
            }
        }
    scan_done:
        /* 0x800852b4. */
        if (*(int32_t *)(obj + 0x74) == -1)
            *(int32_t *)(obj + 0x74) = saved;
        else {
            STCRDABM_func_800840ac(obj, ctx, 1);
            D0x80055C48(0x4001b);
        }
    tail:
        /* 0x800852f0/0x800852f4: bit test first, then service call. */
        if (((((int32_t (*)(int32_t))DAT_8004B7D0[0xfd])(0) >> ((int32_t (*)(int32_t, int32_t))DAT_8004B7D0[0x102])(0, 14)) & 1) == 0)
            return;
        D0x80055C48(0x800450bd);
        *(int32_t *)(obj + 0x68) = 0;
        *(int32_t *)(obj + 0x10) = 0x32;
        fresh = STCRDABM_func_80083008();
        *(int32_t *)(ctx + 0x48) = fresh;
        ((void (*)(int32_t, int32_t, int32_t))*(void **)(fresh + 0x64))(fresh, 0, 10);
        return;
    case 10:
        /* 0x80085378. */
        if (*(int32_t *)(obj + 0x14) == 0)
            STCRDABM_func_800840ac(obj, ctx, 0);
        ((int32_t (*)(void *, int32_t))DAT_80085A64[2])(obj + 0xc8, *(int32_t *)(obj + 0x14));
        ((void (*)(uint8_t *))*(void **)(obj + 0x3c))(obj);
        return;
    case 11:
        /* 0x800853c8. */
        if (((int32_t (*)(void *))DAT_80085A64[3])(obj + 0xc8) != 0)
            *(int32_t *)(obj + 0x10) = 0xf;
        return;
    case 15:
        /* 0x800853ec. */
        if (*(int32_t *)(*(uint8_t **)(ctx + 0x44) + 0xc) != 1)
            return;
        *(int32_t *)(obj + 0x68) = 1;
        STCRDABM_func_80083e14(obj, ctx, 1);
        if (*(int32_t *)(obj + 0xd0) != 0) {
            for (;;) {
                int32_t index, offset, value;
                index = *(int32_t *)(obj + 0x74);
                offset = index * 4;
                value = *(int32_t *)(obj + offset + 0x80);
                if (value != 0)
                    break;
                *(int32_t *)(obj + 0x74) = index + 1;
            }
            STCRDABM_func_800840ac(obj, ctx, 1);
        }
        *(int32_t *)(obj + 0x10) = 3;
        return;
    case 50:
        /* 0x80085468. */
        if (*(int32_t *)(*(uint8_t **)(ctx + 0x48) + 0xc) != 2)
            return;
        *(int32_t *)(obj + 0xc) = 3;
        return;
    case 51:
        /* 0x80085488. */
        if (((int32_t (*)(void *))DAT_80085A64[3])(obj + 0xc8) == 0)
            return;
        STCRDABM_func_80083e14(obj, ctx, 0);
        ((int32_t (*)(void *, int32_t))DAT_80085A64[2])(obj + 0xb8, 0);
        goto inc_state;
    case 52:
        /* 0x800854d0. */
        if (((int32_t (*)(void *))DAT_80085A64[3])(obj + 0xb8) == 0)
            return;
    inc_state:
        /* 0x800854ec. */
        *(int32_t *)(obj + 0x10) = *(int32_t *)(obj + 0x10) + 1;
        return;

    case 53:
        /* 0x80085500. */
        if (*(int32_t *)(ctx + 0x44) != 0)
            return;
        ((void (*)(uint8_t *, int32_t))*(void **)(obj + 0x28))(obj, 3);
        return;
    }
}
