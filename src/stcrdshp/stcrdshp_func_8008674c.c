#include "common/types.h"

/*
 * STCRDSHP:0x8008674c (1656 bytes, 0x8008674c-0x80086dc0).
 *
 * PAL evidence (read-only, project ddw3-pal-sles-03936, program STCRDSHP):
 * - disasm 0x8008674c/0x800867ec/0x8008688c/0x8008692c/0x800869cc/0x80086a6c/
 *   0x80086b0c/0x80086bac/0x80086c4c/0x80086cec/0x80086d8c, 40 instr max each;
 *   full 414-instruction body, jr ra + addiu sp,sp,0x120 end at 0x80086dbc.
 * - decompile 0x8008674c; x-ref to 0x8008674c (sole caller
 *   STCRDSHP_func_80087a2c, site 0x80087b4c, jal with a0 = object).
 * - 9 spot words match reference/extracted/pro/stcrdshp.bin
 *   (offset 0x3a9c = 0x8008674c-0x80082cb0); body SHA-256
 *   14daee2b51a50c031b7a2b8619de279faa666dad9e1b63147092e0cc596ccdc1.
 * - Upstream stcrdshp.s holds only a jal call site (.L0x00004e9c); no body
 *   guide. Nothing copied from upstream/recomp.
 *
 * Frame/model notes (uncertain, preserved as uncertainty):
 * - EXE_F0x8001f648 fills 0xa0 bytes at sp+0x10 (see decomp EXE C_MATCHING
 *   source: function pointers at +0x70..+0x9c). Observed call slots match
 *   +0x74/+0x7c/+0x84/+0x88/+0x8c/+0x94 with consistent arity per slot;
 *   generated stack offsets (132/140/148/156/164/176) agree with PAL.
 * - EXE_F0x8001ebf8 fills the buffer at sp+0xb0 (exact size unverified;
 *   max observed slot is +0x50). Slot +0x00 holds a byte-pointer,
 *   slot +0x2c takes one int arg, slot +0x50 takes no args.
 * - PAL materializes the 0x80044b38 holder (offset 0x424) three times and
 *   the 0x8004de10 holder (offset 0x188) twice; the C keeps one live range
 *   per materialization point (dh_a/dh_b/dh_c, tm_a/tm_b) with no claimed
 *   semantics for the pointed-to tables.
 * - Absolute RAM helpers (0x8008cb10 indexed table) are encoded as
 *   immediates in PAL; the C models them as externs without semantics.
 */

extern void EXE_F0x8001f648(void *buf);
extern void EXE_F0x8001ebf8(void *buf);

extern uint8_t D_80044B38[];
extern uint8_t D_8004DE10[];
extern void *D_8008CB10[];

typedef struct {
    uint8_t pad00[0x54];
    int32_t f54;
    int32_t f58;
    int32_t f5c;
    int32_t f60;
    int32_t f64;
    int32_t f68;
    int32_t f6c;
    int32_t f70;
    uint8_t pad74[0x18];
    int32_t f8c;
    int32_t f90;
    uint8_t pad94[0x14];
    void *fA8;
    uint8_t padAC[0x08];
    int32_t fB4;
    uint8_t padB8[0x0c];
    int32_t fC4;
    uint8_t padC8[0x0c];
    int32_t fD4;
} STCRDSHP_8674C;

typedef struct {
    uint8_t pad00[0x74];
    void (*f74)(int32_t, int32_t);
    uint8_t pad78[0x04];
    void (*f7c)(int32_t, int32_t);
    uint8_t pad80[0x04];
    void (*f84)(void *, int32_t, int32_t, int32_t);
    void (*f88)(void *);
    void (*f8c)(int32_t, int32_t, int32_t);
    uint8_t pad90[0x04];
    void (*f94)(int32_t, int32_t);
    uint8_t pad98[0x08];
} STCRDSHP_8674C_Api;

typedef struct {
    uint8_t *ptr00;
    uint8_t pad04[0x28];
    void (*f2c)(int32_t);
    uint8_t pad30[0x20];
    int32_t (*f50)(void);
} STCRDSHP_8674C_Menu;

void STCRDSHP_func_8008674c(STCRDSHP_8674C *object) {
    STCRDSHP_8674C_Api api;
    STCRDSHP_8674C_Menu menu;
    void *resource;
    int32_t now;
    int32_t card;
    int32_t choice;
    int32_t aux;
    int16_t *deck;
    uint8_t *dh_a;
    uint8_t *tm_a;
    uint8_t *dh_b;
    uint8_t *tm_b;
    uint8_t *dh_c;

    EXE_F0x8001f648(&api);
    api.f74(0x280, 0);
    api.f7c(object->f54, object->f58);
    deck = *(int16_t **)((uint8_t *)object->fA8 + 8);
    card = deck[object->f6c * 6 + object->f5c];
    if (object->fB4 != 0) {
        EXE_F0x8001ebf8(&menu);
        menu.f2c(card);
        if (object->fB4 != 0x1000) {
            api.f8c(object->fB4, 0x1000, 0x1000);
            api.f94(0x140, 0x87);
        }
        choice = menu.f50();
        if (choice == 1) {
            aux = 0x12;
        } else if (choice == 2) {
            aux = 0x13;
        } else {
            aux = *menu.ptr00 + 0x13;
        }
        dh_a = D_80044B38;
        resource = ((void *(*)(uint32_t))*(void **)(dh_a + 0x424))(0x063e0000);        api.f84(resource, aux, 0x103, 0x7e);
        resource = ((void *(*)(uint32_t))*(void **)(dh_a + 0x424))(0x063e0000);        api.f84(resource, 0xd, 0xfc, 0x7c);
        if (object->fB4 != 0x1000) {
            api.f94(0x140, 0x87);
        }
        resource = ((void *(*)(uint32_t))*(void **)(dh_a + 0x424))(0x063e0000);        api.f84(resource, 10, 0x82, 0x7c);
        if (object->fB4 != 0x1000) {
            api.f94(0x140, 0xaf);
        }
        resource = ((void *(*)(uint32_t))*(void **)(dh_a + 0x424))(0x063e0000);        api.f84(resource, 0x10, 0x103, 0xa4);
        resource = ((void *(*)(uint32_t))*(void **)(dh_a + 0x424))(0x063e0000);        api.f84(resource, 0xd, 0xfc, 0xa2);
        if (object->fB4 != 0x1000) {
            api.f94(0x140, 0xa5);
        }
        /* PAL tests choice != 0 first (beq s4,zero 0x800869d8 falls into the
         * direct 0xb draw); the card chain lives under the else-if. */
        if (choice != 0) {
            resource = ((void *(*)(uint32_t))*(void **)(dh_a + 0x424))(0x063e0000);            api.f84(resource, 0xb, 0x4a, 0x92);
        } else if (card == 0x45 || card == 0x70 || card == 0x9b || card == 0xc6 || card == 0xf1) {
            /* PAL reloads the D_80044B38 holder absolutely here
             * (lui 0x8004 at 0x80086a00) instead of the held register. */
            resource = ((void *(*)(uint32_t))*(void **)(D_80044B38 + 0x424))(0x063e0000);            api.f84(resource, 0xb, 0x4a, 0x92);
        } else {
            resource = ((void *(*)(uint32_t))*(void **)(dh_a + 0x424))(0x063e0000);            api.f84(resource, 0xc, 0xc7, 0x92);
        }
        if (object->fC4 != 0x1000) {
            api.f94(0x140, 200);
        }
        /* Same absolute holder reload at the 0x29 draw (lui 0x8004 at 0x80086a70). */
        resource = ((void *(*)(uint32_t))*(void **)(D_80044B38 + 0x424))(0x063e0000);        api.f84(resource, 0x29, 0xd6, 0xbf);
    }
    if (object->f60 != 0) {
        tm_a = D_8004DE10;
        now = ((int32_t (*)(void))*(void **)(tm_a + 0x188))();
        if (now - object->f68 >= 4) {
            object->f68 = ((int32_t (*)(void))*(void **)(tm_a + 0x188))();
            object->f64 += 1;
            if (object->f64 > 5) {
                object->f64 = 0;
            }
        }
        api.f88(D_8008CB10[object->f64]);
        dh_b = D_80044B38;
        resource = ((void *(*)(uint32_t))*(void **)(dh_b + 0x424))(0x063e0000);        api.f84(resource, 7, object->f5c * 0x2a + 0x24, 0x44);
        api.f88(0);
        if (object->f70 > 1) {
            tm_b = D_8004DE10;
            now = ((int32_t (*)(void))*(void **)(tm_b + 0x188))();
        if (now - object->f90 >= 0x11) {
                object->f90 = ((int32_t (*)(void))*(void **)(tm_b + 0x188))();
                object->f8c = 1 - object->f8c;
            }
            if (object->fB4 == 0x1000 && object->f8c != 0) {
                if (object->f6c > 0) {
                    resource = ((void *(*)(uint32_t))*(void **)(dh_b + 0x424))(0x063e0000);                    api.f84(resource, 0x1a, 0xe, 0x55);
                }
                if (object->f6c < object->f70 - 1) {
                    resource = ((void *(*)(uint32_t))*(void **)(dh_b + 0x424))(0x063e0000);                    api.f84(resource, 0x1b, 0x121, 0x55);
                }
            }
        }
    }
    if (object->fC4 != 0) {
        EXE_F0x8001f648(&api);
        api.f74(0x280, 0);
        api.f7c(object->f54, object->f58 - 2);
        if (object->fC4 != 0x1000) {
            api.f8c(object->fC4, 0x1000, 0x1000);
            api.f94(0x140, 0x3f);
        }
        dh_c = D_80044B38;
        resource = ((void *(*)(uint32_t))*(void **)(dh_c + 0x424))(0x063e0000);        api.f84(resource, 0x2c, 0x7b, 0x33);
        if (object->fD4 != 0) {
            if (object->fD4 != 0x1000) {
                api.f8c(object->fD4, 0x1000, 0x1000);
                api.f94(0x140, 100);
            }
            resource = ((void *(*)(uint32_t))*(void **)(dh_c + 0x424))(0x063e0000);            api.f84(resource, 0x2d, 0xaf, 0x50);
        }
    }
}
