/* STCRDDEK:0x8008687c (size 108, 27 words, file-off 0x3bcc = vaddr - 0x80082cb0).
 * PAL bytes (authority): reference/extracted/pro/stcrddek.bin base 0x80082cb0.
 * Boundary: leaf, no frame; entry 0x8008687c (addu a1,a0,zero), terminal jr ra
 * words at 0x800868bc / 0x800868d0 / 0x800868d8 / 0x800868e0; next function
 * STCRDDEK:0x800868e8 (addiu sp,sp,-0x30) starts immediately after, so the
 * 27-word range is contiguous with no overlap or gap.
 * Ghidra project ddw3-pal-sles-03936, program STCRDDEK, read-only: disasm
 * 27 words matches PAL word-for-word (Ghidra byte strings are the same words;
 * e.g. 21288000 == LE 00802821 addu a1,a0,zero); decompiler agrees on the
 * clamp/accumulate shape; x-ref to: 4 direct jal callers at 0x800877f8,
 * 0x800883b0, 0x800883e4, 0x800884a8 (UNCONDITIONAL_CALL); x-ref from: intra-
 * function conditional jumps only, no callees. No Ghidra state change.
 * Behavior (conservative): if word at +0xc == 0 return 1; else add word at +0x4
 * into word at +0x8, store the sum, then clamp: delta > 0 with sum > 0x1000, or
 * delta <= 0 with sum < 0, clears the accumulator (and the +0xc word on the
 * taken clamp path) and returns 1; otherwise returns 0. The delta > 0 case is
 * written first so the upper-bound check stays on the fall-through path, as in
 * the original branch layout (blez to the negative-side check). Struct layout
 * beyond these three words is unrecovered, so the object stays int32_t *.
 * Toolchain hypothesis: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0.
 * Status: C_MATCHING candidate (portable C, no asm, no register variables). */
#include "common/types.h"

int32_t STCRDDEK_func_8008687c(int32_t *arg0) {
    int32_t sum;

    if (arg0[3] == 0) {
        return 1;
    }
    sum = arg0[2] + arg0[1];
    arg0[2] = sum;
    if (arg0[1] > 0) {
        if (sum > 0x1000) {
            arg0[2] = 0x1000;
            arg0[3] = 0;
            return 1;
        }
    } else if (sum < 0) {
        arg0[2] = 0;
        arg0[3] = 0;
        return 1;
    }
    return 0;
}
