/*
 * STCRDSHP:0x800857b0 STCRDSHP_func_800857b0
 * 1020 bytes at STCRDSHP.PRO offset 0x2b00 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800857b0
 *  Symbols     D_80044B38=0x80044b38 EXE_F0x8001ebf8=0x8001ebf8
 *              EXE_F0x8001f648=0x8001f648 EXE_Glob_80044b38=0x80044b38
 *              STCRDSHP_func_800857b0=0x800857b0
 *  Compare     1020 bytes from 0x800857b0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDSHP:0x800857b0
 */

#include "common/types.h"

/*
 * Card-shop list renderer (conservative names).
 *
 * Caller: STCRDSHP_func_80085da0 (+0x5fbc) passes (obj, 0 or 1).
 *
 * Direct EXE helpers 0x8001f648/0x8001ebf8 have no Ghidra program here (no EXE
 * program in project); treated as extern imports like EXE_F0x80014504
 * elsewhere.
 *
 * Word at EXE 0x80044f5c holds a function pointer; source reaches it as field
 * +0x424 of the object at 0x80044b38 (loop.c hoists the address into s8).
 *
 * Frame: 16-byte outgoing area, the 248-byte work struct at sp+0x10, the digit
 * array at sp+0x108 and the spill slot of the hoisted &digits[0] at sp+0x120
 * (frame 0x150 total).
 *
 * Matching notes (PsyQ GCC 2.8.1 + ASPSX 2.79, -O2 -G0, base variant): - fn_b0
 * returns void: a call_value would give the fn pointer a v0 suggestion in
 * local-alloc and move the /6 copy and row temps.
 *
 * - x/x2 carry a dead initializer: loop.c then sees their first reference
 * outside the digit loop and keeps col+K in it, while flow deletes the
 * initializer and local-alloc ties each one to a2.
 *
 * - the digit loops index digits[t]; strength reduction hoists the array base
 * twice, into the outer preheader after the /10 constant.
 */

extern void EXE_F0x8001f648(void *work);
extern void EXE_F0x8001ebf8(void *buf);

typedef struct {
    char pad[0x424];
    void *(*fn)(uint32_t arg);
} EXE_Glob_80044b38;

extern EXE_Glob_80044b38 D_80044B38;

typedef struct {
    char pad0[0x54];
    int32_t unk54;
    int32_t unk58;
    char pad5c[0x8];
    int32_t count;
    char pad68[0x14];
    int32_t arr_a[6];
    int32_t arr_b[6];
} STCRDSHP_857b0_Obj;

typedef struct {
    char unk00[0x74];
    void (*fn_a0)(int32_t a0, int32_t a1);
    char pad88[4];
    void (*fn_a1)(int32_t a0, int32_t a1);
    char pad90[4];
    void *(*draw)(void *r, int32_t a1, int32_t a2, int32_t a3);
    char pad98[0x18];
    uint8_t *text[11];
    void (*fn_b0)(int32_t);
    char pade0[4];
    void (*fn_b1)(int32_t a0, int32_t a1);
    void (*fn_b2)(int32_t a0, int32_t a1);
    void (*fn_b3)(int32_t a0, int32_t a1);
    void (*fn_b4)(int32_t a0, int32_t a1);
    void (*fn_b5)(int32_t a0, int32_t a1);
    char padf8[8];
    int32_t (*fn_c0)(void);
    char pad104[4];
} STCRDSHP_857b0_Work;

void STCRDSHP_func_800857b0(STCRDSHP_857b0_Obj *obj, int32_t flag) {
    STCRDSHP_857b0_Work body;
    int32_t digits[6];
    int32_t *entry;
    void *r;
    int32_t i;
    int32_t mod;
    int32_t quot;
    int32_t col;
    int32_t row;
    int32_t v;
    int32_t bv;
    int32_t t;
    int32_t x = 0;
    int32_t x2 = 0;
    int32_t xi;

    EXE_F0x8001f648(&body);
    body.fn_a1(obj->unk54, obj->unk58);
    body.fn_a0(0x280, 0);
    EXE_F0x8001ebf8(body.text);
    body.fn_b3(0x140, 0x100);
    body.fn_b4(0x300, 0x100);
    body.fn_b2(obj->unk54, obj->unk58);

    if (flag != 0) {
        entry = obj->arr_b;
    } else {
        entry = obj->arr_a;
    }

    for (i = 0; i < obj->count; i++, entry++) {
        if ((uint32_t)(*entry - 1) >= 0x13a) {
            return;
        }
        mod = i % 6;
        quot = i / 6;
        col = mod * 0x2a;
        row = quot * 0x36;
        body.fn_b0(*entry);
        body.fn_b5(0, i);
        body.fn_b1(col + 0x27, row + 0x46);
        v = body.fn_c0();
        if (v != 0) {
            r = D_80044B38.fn(0x063e0000);
            body.draw(r, 0x1d, col + 0x27, row + 0x65);
        } else {
            bv = body.text[0][1];
            t = bv / 10;
            if (t != 0) {
                digits[0] = t + 0x1e;
            } else {
                digits[0] = 0;
            }
            digits[2] = 0x1c;
            xi = 0;
            t = bv % 10;
            digits[1] = t + 0x1e;
            t = xi;
            do {
                if (digits[t] != 0) {
                    r = D_80044B38.fn(0x063e0000);
                    x = col + 0x27;
                    body.draw(r, digits[t], xi + x, row + 0x65);
                }
                t++;
                xi += 7;
            } while (t < 3);
            bv = body.text[0][2];
            t = bv / 10;
            if (t != 0) {
                digits[0] = t + 0x1e;
            } else {
                digits[0] = 0;
            }
            xi = 0;
            t = bv % 10;
            digits[1] = t + 0x1e;
            t = xi;
            do {
                if (digits[t] != 0) {
                    r = D_80044B38.fn(0x063e0000);
                    x2 = col + 0x3a;
                    body.draw(r, digits[t], xi + x2, row + 0x65);
                }
                t++;
                xi += 7;
            } while (t < 2);
        }
        r = D_80044B38.fn(0x063e0000);
        body.draw(r, body.text[0][0] - 1, col + 0x23, row + 0x44);
    }
}
