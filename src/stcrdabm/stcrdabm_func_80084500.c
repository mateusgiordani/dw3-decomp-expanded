/*
 * STCRDABM:0x80084500 STCRDABM_func_80084500
 * 1736 bytes at STCRDABM.PRO offset 0x1850 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80084500
 *  Symbols     DAT_80044f5c=0x80044f5c DAT_8004df98=0x8004df98
 *              EXE_80044b38=0x80044b38 EXE_8004de10=0x8004de10
 *              F0x8001ebf8=0x8001ebf8 F0x8001f648=0x8001f648
 *              TBL_80085a4c=0x80085a4c
 *  Compare     1736 bytes from 0x80084500 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDABM:0x80084500
 */

#include "common/types.h"

/*
 * PAL-SLES-03936; reviewed body-80084500, base 0x80082cb0.
 *
 * Conservative candidate: name suggests album only, semantics unconfirmed.
 *
 * Single pointer arg (offsets 0x50..0xD0 observed); two stack work areas:
 * cb[0xA0] at sp+0x10 filled by F0x8001f648, eb[0x58] at sp+0xB0 filled by
 * F0x8001ebf8. Callbacks via cb slots, EXE vectors at 0x80044f5c and
 * 0x8004df98, data table at 0x80085a4c indexed by arg+0xB0.
 */
extern void F0x8001f648(void *buf);
extern void F0x8001ebf8(void *buf);
extern int32_t (*DAT_80044f5c)(int32_t arg);
extern int32_t (*DAT_8004df98)(void);
extern int32_t TBL_80085a4c[];
extern uint8_t EXE_80044b38[];
extern uint8_t EXE_8004de10[];
typedef int32_t (*tick_t)(void);
typedef int32_t (*resource_t)(int32_t);

typedef void (*cb2_t)(int32_t a0, int32_t a1);
typedef void (*cb1_t)(int32_t a0);
typedef void (*cb0_t)(void);
typedef void (*cb3_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*cb4_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*cbret_t)(void);

void STCRDABM_func_80084500(int32_t arg) {
    uint8_t cb[0xA0];
    uint8_t eb[0x58];
    int32_t v;
    int32_t w;
    int32_t svc;
    int32_t ret;
    int32_t sel;
    int32_t t74;
    uint8_t *api;
    uint8_t *clock;

    F0x8001f648(cb);
    ((cb2_t)*(void **)(cb + 0x74))(0x280, 0);
    ((cb2_t)*(void **)(cb + 0x7C))(*(int32_t *)(arg + 0x50), *(int32_t *)(arg + 0x54));
    if (*(int32_t *)(arg + 0x5C) != 0) {
        int32_t counter;
        counter = *(int32_t *)(arg + 0x58) + 1;
        *(int32_t *)(arg + 0x58) = counter;
        w = 0;
        if (counter < 0x60) {
            w = counter;
        }
        *(int32_t *)(arg + 0x58) = w;
        *(int32_t *)(arg + 0x5C) = 0;
    } else {
        *(int32_t *)(arg + 0x5C) = 1;
    }
    api = EXE_80044b38;
    svc = (*(resource_t *)(api + 0x424))(0x06050000);
    ((cb4_t)*(void **)(cb + 0x84))(svc, 8, *(int32_t *)(arg + 0x58), *(int32_t *)(arg + 0x58));
    ((cb2_t)*(void **)(cb + 0x7C))(*(int32_t *)(arg + 0x50), *(int32_t *)(arg + 0x54) - 2);
    v = *(int32_t *)(arg + 0xC0);
    if (v != 0) {
        if (v != 0x1000) {
            ((cb3_t)*(void **)(cb + 0x8C))(v, 0x1000, 0x1000);
            ((cb2_t)*(void **)(cb + 0x94))(0, 0x20);
        }
        svc = (*(resource_t *)(api + 0x424))(0x06050000);
        ((cb4_t)*(void **)(cb + 0x84))(svc, 9, 0, 0x15);
        if (*(int32_t *)(arg + 0xC0) != 0x1000) {
            ((cb2_t)*(void **)(cb + 0x94))(0x140, 0x20);
        }
        svc = (*(resource_t *)(api + 0x424))(0x06050000);
        ((cb4_t)*(void **)(cb + 0x84))(svc, 0xF, 0xC8, 0x15);
        v = *(int32_t *)(arg + 0xC0);
        if (v != 0x1000) {
            ((cb3_t)*(void **)(cb + 0x8C))(v, v, 0x1000);
            ((cb2_t)*(void **)(cb + 0x94))(0x3A, 0xA5);
        }
        svc = (*(resource_t *)(api + 0x424))(0x06050000);
        ((cb4_t)*(void **)(cb + 0x84))(svc, 0xE, 0x22, 0x9A);
    }
    if (*(int32_t *)(arg + 0x68) != 0) {
        if (*(int32_t *)(arg + 0x64) > 1) {
            clock = EXE_8004de10;
            ret = (*(tick_t *)(clock + 0x188))();
            if (ret - *(int32_t *)(arg + 0x70) > 0x10) {
                svc = (*(tick_t *)(clock + 0x188))();
                *(int32_t *)(arg + 0x70) = svc;
                *(int32_t *)(arg + 0x6C) = 1 - *(int32_t *)(arg + 0x6C);
            }
            if (*(int32_t *)(arg + 0x6C) != 0) {
                if (*(int32_t *)(arg + 0x60) > 0) {
                    svc = DAT_80044f5c(0x06050000);
                    ((cb4_t)*(void **)(cb + 0x84))(svc, 0x1A, 0xE, 0x5A);
                }
                if (*(int32_t *)(arg + 0x60) < *(int32_t *)(arg + 0x64) - 1) {
                    svc = DAT_80044f5c(0x06050000);
                    ((cb4_t)*(void **)(cb + 0x84))(svc, 0x1B, 0x121, 0x5A);
                }
            }
        }
        if (*(int32_t *)(arg + 0x7C) != 0) {
            clock = EXE_8004de10;
            ret = (*(tick_t *)(clock + 0x188))();
            if (ret - *(int32_t *)(arg + 0xB4) > 0x10) {
                svc = (*(tick_t *)(clock + 0x188))();
                *(int32_t *)(arg + 0xB4) = svc;
                *(int32_t *)(arg + 0xB0) += 1;
                if (*(int32_t *)(arg + 0xB0) > 5) {
                    *(int32_t *)(arg + 0xB0) = 0;
                }
            }
            ((cb1_t)*(void **)(cb + 0x88))(TBL_80085a4c[*(int32_t *)(arg + 0xB0)]);
            svc = DAT_80044f5c(0x06050000);
            t74 = *(int32_t *)(arg + 0x74);
            ((cb4_t)*(void **)(cb + 0x84))(svc, 7, (t74 % 6) * 0x2A + 0x24, (t74 / 6) * 0x36 + 0x32);
            ((cb1_t)*(void **)(cb + 0x88))(0);
        }
    }
    if (*(int32_t *)(arg + 0xD0) != 0) {
        int32_t kind;
        F0x8001ebf8(eb);
        ((cb1_t)*(void **)(eb + 0x2C))(*(int32_t *)(arg + 0x78));
        if (*(int32_t *)(arg + 0xD0) != 0x1000) {
            ((cb3_t)*(void **)(cb + 0x8C))(*(int32_t *)(arg + 0xD0), 0x1000, 0x1000);
            ((cb2_t)*(void **)(cb + 0x94))(0x140, 0xA8);
        }
        kind = ((cbret_t)*(void **)(eb + 0x50))();
        if (kind == 1) {
            sel = 0x12;
        } else if (kind == 2) {
            sel = 0x13;
        } else {
            sel = (int32_t)**(uint8_t **)eb + 0x13;
        }
        api = EXE_80044b38;
        svc = (*(resource_t *)(api + 0x424))(0x06050000);
        ((cb4_t)*(void **)(cb + 0x84))(svc, sel, 0x103, 0x9F);
        svc = (*(resource_t *)(api + 0x424))(0x06050000);
        ((cb4_t)*(void **)(cb + 0x84))(svc, 0xD, 0xFC, 0x9D);
        if (*(int32_t *)(arg + 0xD0) != 0x1000) {
            ((cb2_t)*(void **)(cb + 0x94))(0x140, 0xA8);
        }
        svc = (*(resource_t *)(api + 0x424))(0x06050000);
        ((cb4_t)*(void **)(cb + 0x84))(svc, 10, 0x82, 0x9D);
        if (*(int32_t *)(arg + 0xD0) != 0x1000) {
            ((cb2_t)*(void **)(cb + 0x94))(0x140, 0xD0);
        }
        svc = (*(resource_t *)(api + 0x424))(0x06050000);
        ((cb4_t)*(void **)(cb + 0x84))(svc, 0x10, 0x103, 0xC5);
        svc = (*(resource_t *)(api + 0x424))(0x06050000);
        ((cb4_t)*(void **)(cb + 0x84))(svc, 0xD, 0xFC, 0xC3);
        if (*(int32_t *)(arg + 0xD0) != 0x1000) {
            ((cb2_t)*(void **)(cb + 0x94))(0x140, 0xC6);
        }
        if (kind != 0) {
            svc = (*(resource_t *)(api + 0x424))(0x06050000);
            ((cb4_t)*(void **)(cb + 0x84))(svc, 0xB, 0x4A, 0xB3);
        } else {
            v = *(int32_t *)(arg + 0x78);
            if (v == 0x45 || v == 0x70 || v == 0x9B || v == 0xC6 || v == 0xF1) {
                svc = DAT_80044f5c(0x06050000);
                ((cb4_t)*(void **)(cb + 0x84))(svc, 0xB, 0x4A, 0xB3);
            } else {
                svc = (*(resource_t *)(api + 0x424))(0x06050000);
                ((cb4_t)*(void **)(cb + 0x84))(svc, 0xC, 0xC7, 0xB3);
            }
        }
    }
}
