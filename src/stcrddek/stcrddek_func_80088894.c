// STCRDDEK:0x80088894 (size 20, 0x14)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x5be4 (RAW, no header)
// Boundary: frameless leaf, 5 instructions; prev STCRDDEK:0x80088880 (20B) ends exactly at
// 0x80088894; next STCRDDEK:0x800888a8 at +0x14. No prologue/epilogue, no stack frame.
// Body words: 01000224 (li v0,1) ; 6c0085ac (sw a1,0x6c(a0)) ; 700086ac (sw a2,0x70(a0)) ;
// 0800e003 (jr ra) ; 680082ac (sw v0,0x68(a0) in delay slot). All 20 bytes byte-match PAL.
// Ghidra project ddw3-pal-sles-03936, program STCRDDEK, read-only: disasm 5 insns matches PAL;
// decompile stores param_2 -> +0x6c, param_3 -> +0x70, 1 -> +0x68 (decompiler says void, but
// v0=1 proves int return 1); x-ref to 0x80088894 empty (no direct in-program callers; likely
// table-driven like the sibling setters). upstream stcrddek.s is a GUIDE-only raw word dump
// at placeholder base with no function labels; never copied. No Ghidra state change.
// Family: sibling STCRDDEK:0x80088880 has the identical shape with offsets +0x78/+0x7c/+0x80;
// STCRDDEK:0x800888a8 is a single-store (a1 -> +0x74) tail of the same cluster.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base hypothesis).
// Status: C_MATCHING (portable C, no asm, no explicit register variables) on exact_byte_match;
// sha256 36a396dab23bb4bb8329651f98254fb84dd5c3581db181a2813f8d8aed017331 (20B).
#include <stdint.h>

int32_t STCRDDEK_func_80088894(int32_t p1, int32_t p2, int32_t p3)
{
    *(int32_t *)(p1 + 0x6c) = p2;
    *(int32_t *)(p1 + 0x70) = p3;
    *(int32_t *)(p1 + 0x68) = 1;
    return 1;
}
