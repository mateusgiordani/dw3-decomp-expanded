// STCRDDEK:0x80088874 (size 12, 3 words)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x5bc4 (RAW, no header)
// Boundary: starts immediately after STCRDDEK_func_800887d4 epilogue
// (jr ra + addiu sp,0x20 at 0x8008886c-0x80088873); body sw a1,0x58(a0) /
// jr ra / sw a2,0x60(a0) in the delay slot; next reviewed function
// STCRDDEK_func_80088880 (li v0,1 ...) is contiguous, no overlap.
// Ghidra program STCRDDEK (project ddw3-pal-sles-03936) read-only: disasm 3 words
// matches PAL bytes exactly; decompile: *(param_1 + 0x58) = param_2;
// *(param_1 + 0x60) = param_3. x-ref to/from 0x80088874: empty (no known
// callers/callees in cache). Upstream stcrddek.s is GUIDE only (carries no
// 0x80088874 label); never copied. No Ghidra state change.
// Semantics unconfirmed: deck-menu candidate per dispatcher; conservative
// p1/p2/p3 + offsets only, no struct promotion (single-function evidence).
// Same setter family as neighbours 0x80088880/0x80088894 (store a1/a2 pair).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
// Status: C_MATCHING (portable C, no asm, no explicit register variables).
// sha256 4139fc3c4238b5db0df0657a202b4cc5569d4908e6fc5689f111ca65687870a3 (12B).
#include <stdint.h>

void STCRDDEK_func_80088874(int32_t p1, int32_t p2, int32_t p3)
{
    *(uint32_t *)(p1 + 0x58) = p2;
    *(uint32_t *)(p1 + 0x60) = p3;
}
