#include "common/types.h"

/* STCRDSHP:0x80088e24 (208B, 52 instructions)
 * PAL bytes: reference/extracted/pro/stcrdshp.bin base 0x80082cb0 file-off 0x6174 (24948)
 * Boundary: prologue addiu sp,-0x58, epilogue jr ra / addiu sp,0x58;
 * frame 0x58 with ra@0x50 s1@0x4c s0@0x48, stack buffer at sp+0x10 (56 bytes).
 * Next function STCRDSHP:0x80088ef4 starts immediately after (no gap/overlap).
 * Ghidra: project ddw3-pal-sles-03936 program STCRDSHP, disasm/decompile/x-ref
 * read-only (no cache mutation). STCRDSHP bytes authoritative: Ghidra words
 * match the reference bin word-for-word, so the campaign program stands.
 * Shape matches C_MATCHING sibling STCRDDEK_func_8008a100 (char buf[56],
 * slots buf+0x24/buf+0x2c, EXE vectors via D_80044B38 fn40c/fn424); fresh
 * immediates, not copied from upstream/recomp. Shop role unclaimed.
 */
extern void F0x8001ffa8(void *buf);
extern struct {
    char pad[0x40c];
    void (*fn40c)(int);
    char pad2[0x14];
    void (*fn424)(int);
} D_80044B38;
extern int D_8005CCA8;

void STCRDSHP_func_80088e24(void) {
    char buf[56];
    int ret;

    F0x8001ffa8(buf);
    ((void (*)(int, int))*(void **)(buf + 0x24))(640, 0);
    ret = ((int (*)(int))D_80044B38.fn424)(0x06420000);
    ((void (*)(int))*(void **)(buf + 0x2c))(ret);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x32);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x16);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x1D);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x94);
    ((void (*)(int))D_80044B38.fn40c)(D_8005CCA8 + 0x6A);
}
