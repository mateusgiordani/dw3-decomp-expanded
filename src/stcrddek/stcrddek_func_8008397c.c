/*
 * STCRDDEK:0x8008397c STCRDDEK_func_8008397c
 * 940 bytes at STCRDDEK.PRO offset 0xccc (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x8008397c
 *  Symbols     F0x8001ae38=0x8001ae38 F0x8001b364=0x8001b364
 *              STCRDDEK_func_8008397c=0x8008397c
 *  Compare     940 bytes from 0x8008397c against the PAL overlay
 *  Verify      python tools/card_verify.py --only STCRDDEK:0x8008397c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Semantics (conservative): deck-menu object-table init. p1 carries a signed
 * halfword id at +0x54 and an int count at +0x58. Every EXE helper result is
 * stored into the out pointer table (51 slots, indexes 0x00-0x32 fully
 * covered); most objects then receive an indirect method call with (*(p1+0x58)
 * - 1). Loop 1 builds 6 entries (fixed-point key 0x34+i*0x23, kind 0x29) at
 * out[3..8]; loop 2 builds 8x3 entries (key 0x27+i*0x0e, kinds
 * 0xa3/0x111/0x120) at out[0x19..0x30]. One entry uses helper 0x8001b364 + slot
 * +0x80(obj, 0); the last uses slot +0x160(obj, 2).
 *
 * Caller: 1 direct jal from STCRDDEK 0x800865c0 (STCRDDEK_func_80086574,
 * UNCONDITIONAL_CALL); that caller passes its own (param_1, param_2) through.
 *
 * Callees: 24 direct cross-module jals to EXE 0x8001ae38 + 1 to EXE 0x8001b364
 * (absolute, out-of-overlay; relocation_count 25); 8 indirect jalr via object
 * slots +0x15c (6x with count-1), +0x80 (arg 0), +0x160 (arg 2). Straight-store
 * sites keep sw v0 (no method call).
 *
 * Codegen notes (all from PAL bytes, verified against candidate codegen): -
 * every F0x8001ae38 jal is preceded by li a1,1, and both loop counters advance
 * via addu s3,s3,a1 in a jal delay slot (never addiu): modeled with a shared
 * unit variable k (k = 1 at each call, i += k after it).
 *
 * - every stored-then-called object passes through move a0,v0 first (store from
 * a0, slot load lw v0,off(a0)): modeled with the object temporary homed in a0
 * and an explicit method-pointer temporary m (homed in v0, forcing the early
 * copy).
 *
 * No Ghidra state change.
 *
 * - the argument setters after a method call only schedule a1/a2 before lh a0
 * when no loop note follows the call: STCRDDEK_CALL_N and the +0x80 call are
 * plain blocks (a do { } while (0) leaves NOTE_INSN_LOOP_END on the next insn
 * and sched2 turns it into a barrier). Loop 1 is goto-built.
 */

#include "common/types.h"

extern void *F0x8001ae38(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
extern void *F0x8001b364(int32_t a0, int32_t a1, int32_t a2, int32_t a3);

typedef void (*STCRDDEK_Method)(void *obj, int32_t arg);

#define STCRDDEK_CALL_N(o, off) \
    { \
        int32_t na_ = *(int32_t *)(p1 + 0x58); \
        STCRDDEK_Method m = *(STCRDDEK_Method *)((uint8_t *)(o) + (off)); \
        m((o), na_ - 1); \
    }

#define STCRDDEK_CALL_C(o, off, arg) \
    do { \
        STCRDDEK_Method m = *(STCRDDEK_Method *)((uint8_t *)(o) + (off)); \
        m((o), (arg)); \
    } while (0)

void STCRDDEK_func_8008397c(uint8_t *p1, void **out)
{
    int32_t i;
    int32_t k;
    int32_t fx1;
    int32_t fx2;
    void *obj;
    void *t;
    obj = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0xc1, 0x17);
    i = 0;
    fx1 = 0x340000;
    t = obj;
    out[0] = t;
    STCRDDEK_CALL_N(t, 0x15c);

    obj = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0x15, 0x18);
    t = obj;
    out[2] = t;
    STCRDDEK_CALL_N(t, 0x15c);

    {
    void **p;
    p = out;
loop1:
    {
        obj = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, fx1 >> 16, 0x29);
        i += k;
        t = obj;
        p[3] = t;
        p += 1;
        STCRDDEK_CALL_N(t, 0x15c);
        fx1 += 0x230000;
    }
    if (i < 6)
        goto loop1;
    }

    out[1] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0x96, 0xcc);
    out[9] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0x96, 0xbd);
    out[10] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0x115, 0x26);
    out[11] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0x126, 0x26);
    out[16] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0x50, 0x17);
    out[12] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0xce, 0x17);
    out[13] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0xf0, 0x17);
    out[14] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0xce, 0x24);
    out[15] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0xf0, 0x24);

    i = 0;
    fx2 = 0x270000;
    {
    void **p;
    int32_t sh;
    p = out;
    do {
        sh = fx2 >> 16;
        obj = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0xa3, sh);
        i += k;
        t = obj;
        p[0x19] = t;
        STCRDDEK_CALL_N(t, 0x15c);
        obj = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0x111, sh);
        t = obj;
        p[0x1a] = t;
        STCRDDEK_CALL_N(t, 0x15c);
        obj = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0x120, sh);
        t = obj;
        p[0x1b] = t;
        p += 3;
        STCRDDEK_CALL_N(t, 0x15c);
        fx2 += 0xe0000;
    } while (i < 8);
    }

    obj = F0x8001b364(*(int16_t *)(p1 + 0x54), *(int32_t *)(p1 + 0x58) - 1, 0x89, 0x27);
    t = obj;
    out[0x31] = t;
    {
        STCRDDEK_Method m = *(STCRDDEK_Method *)((uint8_t *)t + 0x80);
        m(t, 0);
    }

    out[0x11] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0x88, 0xa1);
    out[0x12] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0x115, 0xa1);
    out[0x13] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0x126, 0xa1);
    out[0x18] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0x50, 0xb8);
    out[0x14] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0xce, 0xb8);
    out[0x15] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0xf0, 0xb8);
    out[0x16] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0xce, 0xc5);
    out[0x17] = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0xf0, 0xc5);
    obj = F0x8001ae38(*(int16_t *)(p1 + 0x54), k = 1, 0x3e, 0x6b);
    t = obj;
    out[0x32] = t;
    STCRDDEK_CALL_C(t, 0x160, 2);
}
