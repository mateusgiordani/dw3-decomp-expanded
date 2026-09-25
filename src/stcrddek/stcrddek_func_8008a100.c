#include "common/types.h"

/* STCRDDEK:0x8008a100 (284B, 71 instructions)
 * PAL bytes: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x7450 (29776)
 * Boundary: reviewed inventory body-8008a100; prologue addiu sp,-0x58, epilogue jr ra / addiu sp,0x58;
 * frame 0x58 with ra@0x50 s1@0x4c s0@0x48, stack buffer at sp+0x10 (56 bytes).
 * Next function STCRDDEK:0x8008a21c starts immediately after (no gap/overlap).
 * Ghidra: project ddw3-pal-sles-03936 program STCRDDEK, disasm/decompile read-only (no cache mutation).
 * File name does not prove a deck-menu role; semantics unconfirmed, names conservative.
 */
extern void F0x8001ffa8(void *buf);
extern struct {
    char pad[0x40c];
    void (*fn40c)(int);
    char pad2[0x14];
    void (*fn424)(int);
} D_80044B38;
extern int D_8005CCA8;

void STCRDDEK_func_8008a100(void) {
    char buf[56];
    int ret;

    F0x8001ffa8(buf);
    ((void (*)(int, int))*(void **)(buf + 0x24))(640, 0);
    ret = ((int (*)(int))D_80044B38.fn424)(0x06420000);
    ((void (*)(int))*(void **)(buf + 0x2c))(ret);
    ((void (*)(int))D_80044B38.fn40c)(0x7F6);
    ((void (*)(int))D_80044B38.fn40c)(0x7F7);
    ((void (*)(int))D_80044B38.fn40c)(0x7F8);
    ((void (*)(int))D_80044B38.fn40c)(0x7F9);
    ((void (*)(int))D_80044B38.fn40c)(0x7FA);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x16);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x1D);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x32);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x86);
    ((void (*)(int))D_80044B38.fn40c)(0x771);
}
