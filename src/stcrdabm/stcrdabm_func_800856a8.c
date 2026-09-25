#include "common/types.h"

/* STCRDABM:0x800856a8 (248 bytes, 62 instructions)
 * PAL-SLES-03936; base 0x80082cb0, off 0x29f8.
 * Boundary 0x800856a8..0x800857a0 confirmed by Ghidra disasm:
 * next prologue addiu sp,sp,-0x20 at 0x800857a0.
 * Body: init stack record via EXE F0x8001ffa8, dispatch through its
 * slots +0x24/+0x2c, then fan out through the RAM vector at 0x80044f44
 * with fixed ids 0x7f6..0x7fa and RAM word 0x8005cca8 offsets.
 * The 0x80044b38 table base is held in a local so codegen keeps the
 * PAL lui/addiu pair and s0-relative loads.
 */
extern void F0x8001ffa8(void *buf);
extern uint8_t D0x80044b38[];
extern int32_t D0x8005cca8;

void STCRDABM_func_800856a8(void) {
    uint32_t buf[14];
    uint8_t *base;
    int32_t v;

    F0x8001ffa8(buf);
    ((void (*)(int32_t, int32_t))buf[9])(0x280, 0);
    base = D0x80044b38;
    v = ((int32_t (*)(int32_t))*(void **)(base + 0x424))(0x06060000);
    ((void (*)(int32_t))buf[11])(v);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(0x7f6);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(0x7f7);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(0x7f8);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(0x7f9);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(0x7fa);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(D0x8005cca8 + 0x16);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(D0x8005cca8 + 0x1d);
    ((void (*)(int32_t))*(void **)(base + 0x40c))(D0x8005cca8 + 0x24);
}
