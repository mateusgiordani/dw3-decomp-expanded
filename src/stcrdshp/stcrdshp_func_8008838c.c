/*
 * STCRDSHP:0x8008838c STCRDSHP_func_8008838c
 * 1904 bytes at STCRDSHP.PRO offset 0x56dc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008838c, jump table (.rodata) at 0x80082e74
 *  Symbols     D_80042B98=0x80042b98 D_80044F4C=0x80044f4c
 *              D_8004B7D0=0x8004b7d0 D_80055C48=0x80055c48
 *              D_8005CCA8=0x8005cca8 D_8008CB80=0x8008cb80
 *              D_8008CB88=0x8008cb88 D_8008CB8C=0x8008cb8c
 *              STCRDSHP_func_8008536c=0x8008536c
 *              STCRDSHP_func_80085634=0x80085634
 *              STCRDSHP_func_80087b84=0x80087b84
 *              STCRDSHP_func_80087e8c=0x80087e8c
 *              STCRDSHP_func_80087fac=0x80087fac
 *  Compare     1904 bytes from 0x8008838c and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x8008838c
 */

#include "common/types.h"

/*
 * STCRDSHP:0x8008838c, size 1904 (file off 22236..24140).
 * Card-shop state machine; dispatched on *(self + 0x10) via jump table at
 * 0x80082e74 (102 entries, indices 0..0x65). Explicit states:
 *   1, 2, 3, 10, 11, 20, 21, 22, 23, 50, 51, 52, 100, 101;
 * every other index (including 4..9) targets the default block. Index 0 is
 * an explicit `case 0:` sharing the default block: the PAL dispatch is
 * `sltiu v0,v1,0x66` (state < 102, no subtract), so the table covers 0..101.
 * Caller: STCRDSHP_func_80088b50 (call site 0x80088c18, args self, arg).
 */

typedef int32_t (*PollFn)(int32_t);
typedef int32_t (*PollArgFn)(int32_t, int32_t);
typedef void (*MsgFn)(uint32_t);
typedef void (*StateCb)(void *, int32_t);
typedef void (*ObjCall1)(void *, int32_t);
typedef void (*ObjCall2)(void *, int32_t, int32_t);
typedef void (*ObjCallP)(void *, void *, int32_t);

extern void (*D_8008CB88)(void *, int32_t);
extern int32_t (*D_8008CB8C)(void *);
/* PAL: cases 1 and 51 address both overlay callbacks through one shared base
 * (lui 0x8009, addiu -0x3480 => 0x8008cb80; slots +0x8/+0xc). Single-use
 * cases emit the direct lui+lw form via the two externs above. */
extern uint32_t D_8008CB80[];
/* PAL: case 10 calls its count/check helpers indirectly through the table at
 * 0x80042b98 (lui 0x8004, addiu 0x2b98; count slot +0xc, check slot +0x4). */
extern uint32_t D_80042B98[];
extern void (*D_80055C48)(uint32_t);
extern int32_t (*D_80042BA4)(int32_t, void *);
extern int32_t (*D_80042B9C)(int32_t);
extern void *(*D_80044F4C)(uint32_t);
extern int32_t D_8005CCA8;
extern uint32_t D_8004B7D0[];
extern void STCRDSHP_func_80087e8c(void *, void *, int32_t);
extern void STCRDSHP_func_80087fac(void *, void *, int32_t);
extern int32_t STCRDSHP_func_80087b84(void *, int32_t);
/* Recovery note: the 0x8008536c TU types this callee void, but this call
 * site stores v0 afterwards, which carries the created object. */
extern void *STCRDSHP_func_8008536c(void *);
/* Recovery note: the 0x80085634 TU types this callee void, but both call
 * sites here consume v0 (move a0,v0 / sw v0), so the true binary behavior
 * returns the created object; typed void * for this call-site evidence. */
extern void *STCRDSHP_func_80085634(void);

void STCRDSHP_func_8008838c(uint8_t *self, uint8_t *arg) {
    int32_t state;
    int32_t counter;
    uint8_t *obj;
    void *res;

    state = *(int32_t *)(self + 0x10);
    switch (state) {
    case 0:
    default:
        D_8008CB88(self + 0x398, 1);
        *(int32_t *)(self + 0x10) += 1;
        return;
    case 1: {
        uint32_t *cb = D_8008CB80;
        if (((int32_t (*)(void *))cb[3])(self + 0x398) == 0)
            return;
        STCRDSHP_func_80087e8c(self, arg, 1);
        ((void (*)(void *, int32_t))cb[2])(self + 0x3a8, 1);
        *(int32_t *)(self + 0x10) += 1;
        return;
    }
    case 2:
        if (D_8008CB8C(self + 0x3a8) == 0)
            return;
        STCRDSHP_func_80087fac(self, arg, 1);
        obj = *(uint8_t **)(arg + 0x20);
        ((ObjCall2)(*(void **)(obj + 0x84)))(obj, 0x9a, *(int32_t *)(self + 0x6c) * 0xe + 0x35);
        obj = *(uint8_t **)(arg + 0x20);
        ((ObjCall1)(*(void **)(obj + 0x80)))(obj, 1);
        *(int32_t *)(self + 0x10) += 1;
        return;
    case 3: {
        int32_t old_counter;
        uint32_t *exevec = D_8004B7D0;
        old_counter = *(int32_t *)(self + 0x6c);
        if ((((PollFn)exevec[0x3f4 / 4])(0) >> ((PollArgFn)exevec[0x408 / 4])(0, 4) & 1) ||
            (((PollFn)exevec[0x3fc / 4])(0) >> ((PollArgFn)exevec[0x408 / 4])(0, 4) & 1)) {
            counter = *(int32_t *)(self + 0x6c) - 1;
            *(int32_t *)(self + 0x6c) = counter;
            if (counter < 0)
                *(int32_t *)(self + 0x6c) = 0;
        } else if ((((PollFn)exevec[0x3f4 / 4])(0) >> ((PollArgFn)exevec[0x408 / 4])(0, 6) & 1) ||
                   (((PollFn)exevec[0x3fc / 4])(0) >> ((PollArgFn)exevec[0x408 / 4])(0, 6) & 1)) {
            counter = *(int32_t *)(self + 0x6c) + 1;
            *(int32_t *)(self + 0x6c) = counter;
            if (counter >= 3)
                *(int32_t *)(self + 0x6c) = 2;
        }
        if (old_counter != *(int32_t *)(self + 0x6c)) {
            D_80055C48(0x8004513e);
            obj = *(uint8_t **)(arg + 0x20);
            ((ObjCall2)(*(void **)(obj + 0x84)))(obj, 0x9a, *(int32_t *)(self + 0x6c) * 0xe + 0x35);
        }
        exevec = D_8004B7D0;
        if (((PollFn)exevec[0x3f4 / 4])(0) >> ((PollArgFn)exevec[0x408 / 4])(0, 0xd) & 1) {
            D_80055C48(0x8004503c);
            *(int32_t *)(self + 0x10) = 10;
            return;
        }
        if (((PollFn)exevec[0x3f4 / 4])(0) >> ((PollArgFn)exevec[0x408 / 4])(0, 0xe) & 1) {
            D_80055C48(0x800450bd);
            ((StateCb)(*(void **)(self + 0x2c)))(self, 0x32);
            return;
        }
        return;
    }
    case 10: {
        int32_t count;
        int32_t i;
        int32_t mode = 1;
        *(int32_t *)(self + 0x14) = mode;
        switch (*(int32_t *)(self + 0x6c)) {
        case 0:
        default:
            *(void **)(arg + 0x24) = STCRDSHP_func_80087b84(self, *(int32_t *)(self + 0x60));
            *(int32_t *)(self + 0x10) = 0x32;
            return;
        case 1:
            count = ((int32_t (*)(int32_t, void *))D_80042B98[3])(mode, self + 0x70);
            for (i = 0; i < count; i++) {
                if (((int32_t (*)(int32_t))D_80042B98[1])(((int16_t *)(self + 0x70))[i]) == 0x62) {
                    *(void **)(arg + 0x24) = STCRDSHP_func_8008536c(self);
                    *(int32_t *)(self + 0x10) = 0x32;
                    break;
                }
            }
            if (*(int32_t *)(arg + 0x24) != 0)
                return;
            ((StateCb)(*(void **)(self + 0x2c)))(self, 0x14);
            return;
        case 2:
            *(int32_t *)(self + 0x10) = 100;
            *(int32_t *)(self + 0x68) = mode;
            return;
        }
    }
    case 11:
        if (*(int32_t *)(arg + 0x24) != 0)
            return;
        *(int32_t *)(self + 0x10) = 1;
        return;
    case 20:
        obj = *(uint8_t **)(arg + 0x20);
        ((ObjCall1)(*(void **)(obj + 0x94)))(obj, 1);
        obj = *(uint8_t **)(arg + 0x20);
        ((ObjCall1)(*(void **)(obj + 0x88)))(obj, 7);
        D_8008CB88(self + 0x3b8, 1);
        *(int32_t *)(self + 0x10) += 1;
        return;
    case 21:
        if (D_8008CB8C(self + 0x3b8) == 0)
            return;
        res = D_80044F4C((uint32_t)D_8005CCA8 + 0x32);
        obj = *(uint8_t **)(arg + 8);
        ((ObjCall2)(*(void **)(obj + 0x114)))(obj, (int32_t)res, 0x15);
        *(int32_t *)(self + 0x10) += 1;
        return;
    case 22: {
        uint32_t *exevec = D_8004B7D0;
        if (!(((PollFn)exevec[0x3f4 / 4])(0) >> ((PollArgFn)exevec[0x408 / 4])(0, 0xd) & 1))
            return;
        D_80055C48(0x4001c);
        obj = *(uint8_t **)(arg + 8);
        ((ObjCall1)(*(void **)(obj + 0x144)))(obj, 0);
        D_8008CB88(self + 0x3b8, 0);
        *(int32_t *)(self + 0x10) += 1;
        return;
    }
    case 23:
        if (D_8008CB8C(self + 0x3b8) == 0)
            return;
        obj = *(uint8_t **)(arg + 0x20);
        ((ObjCall1)(*(void **)(obj + 0x94)))(obj, 0);
        obj = *(uint8_t **)(arg + 0x20);
        ((ObjCall1)(*(void **)(obj + 0x88)))(obj, 0);
        *(int32_t *)(self + 0x10) = 3;
        return;
    case 50:
        if (*(int32_t *)(self + 0x14) == 0) {
            obj = (uint8_t *)STCRDSHP_func_80085634();
            *(uint8_t **)(arg + 0x28) = obj;
            ((ObjCall2)(*(void **)(obj + 100)))(obj, 0, 0x1e);
            *(int32_t *)(*(uint8_t **)(arg + 0x28) + 0x54) = 6;
        }
        STCRDSHP_func_80087fac(self, arg, 0);
        obj = *(uint8_t **)(arg + 0x20);
        ((ObjCall1)(*(void **)(obj + 0x80)))(obj, 0);
        D_8008CB88(self + 0x3a8, 0);
        *(int32_t *)(self + 0x10) += 1;
        return;
    case 51: {
        uint32_t *cb = D_8008CB80;
        if (((int32_t (*)(void *))cb[3])(self + 0x3a8) == 0)
            return;
        if (*(int32_t *)(self + 0x14) == 0) {
            STCRDSHP_func_80087e8c(self, arg, 0);
            ((void (*)(void *, int32_t))cb[2])(self + 0x398, 0);
            *(int32_t *)(self + 0x10) += 1;
            return;
        }
        *(int32_t *)(self + 0x10) = 0xb;
        return;
    }
    case 52:
        if (D_8008CB8C(self + 0x398) == 0)
            return;
        *(int32_t *)(self + 0x10) = 0x65;
        return;
    case 100:
        obj = (uint8_t *)STCRDSHP_func_80085634();
        *(uint8_t **)(arg + 0x28) = obj;
        ((ObjCall2)(*(void **)(obj + 100)))(obj, 0, 10);
        *(int32_t *)(self + 0x10) += 1;
        return;
    case 101:
        if (*(int32_t *)(*(uint8_t **)(arg + 0x28) + 0xc) != 2)
            return;
        *(int32_t *)(self + 0xc) = 3;
        return;
    }
}
