// STCRDDEK:0x80088880 (20 B, 5 words)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x5bd0 (RAW, no header)
// Boundary: leaf; no prologue/frame; epilogue jr ra + delay-slot store.
// Next function STCRDDEK:0x80088894 at +0x14 (li v0,1 prologue), contiguous, no overlap.
// Ghidra program STCRDDEK (project ddw3-pal-sles-03936) read-only: disasm 5 words matches PAL;
// decompile: *(a0+0x7c)=a1; *(a0+0x80)=a2; *(a0+0x78)=1. No Ghidra state change.
// stcrddek.s is GUIDE only; never copied as source. No Ghidra state change.
// Xrefs: no direct xrefs/callers in Ghidra; no callees (leaf). Sibling setters at
// 0x80088874 (12 B, stores a1/a2 only) and 0x80088894 (same shape, offsets 0x68/0x6c/0x70).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
// Status: C_MATCHING (portable C, no asm, no explicit register variables).
// sha256 dad0f2e3aac60fcaab0769d9a75ec15ceed3cc433e4051affbc8ea251d3afacd (20B).
#include <stdint.h>

void STCRDDEK_func_80088880(int32_t a0, int32_t a1, int32_t a2)
{
    *(int32_t *)(a0 + 0x7c) = a1;
    *(int32_t *)(a0 + 0x80) = a2;
    *(int32_t *)(a0 + 0x78) = 1;
}
