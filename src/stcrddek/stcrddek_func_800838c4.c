// STCRDDEK:0x800838c4 (size 112, 0x70)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0xc14 (RAW, no header)
// Boundary: prologue 27bdffe8 addiu sp,sp,-0x18 + sw s0,0x10(sp) + sw ra,0x14(sp);
// epilogue lw ra,0x14(sp) + lw s0,0x10(sp) + jr ra + addiu sp,sp,+0x18 at 0x80083924-0x80083930.
// Previous framed STCRDDEK:0x800837b4 (272B) ends exactly at 0x800838c4; next framed
// STCRDDEK:0x80083934 starts exactly at +0x70: size 0x70 contiguous, no overlap.
// Ghidra project ddw3-pal-sles-03936, program STCRDDEK, read-only: disasm 28 words matches PAL
// word-for-word; decompile shows dispatch on *(param_1 + 0xc): 1 -> 0x800837b4 then falls
// through to 0x800835d8 (shared label LAB_8008391c), 2 -> 0x800835d8, 3 -> return,
// default/0/negative -> (*(param_1 + 0x38))(param_1) then 0 at +0x5c.
// Branch chain is a linear decision tree (beq ==1, slti <2 -> default, beq ==2, beq ==3,
// fallthrough to default); a switch lowers to a balanced tree (checks ==2 first, 136B),
// a structured if/else chain inverts branches (bne) and threads kind<2 into kind<1.
// Independent forward-goto conditions reproduce the exact tree with no threading.
// Callers: none direct (bin-wide jal scan for 0x0c200e31 empty; Ghidra x-ref to 0x800838c4
// empty) - reachable via table or another module; no caller promoted.
// Callees: 2 direct intra-overlay jals to STCRDDEK 0x800837b4 and STCRDDEK 0x800835d8;
// 1 indirect jalr via pointer at arg + 0x38.
// stcrddek.s is GUIDE only; never copied as source. No Ghidra state change.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base), exact_byte_match.
// Status: C_MATCHING (portable C, no asm, no explicit register variables).
// Semantics unconfirmed: deck-menu role is name-based only; offsets kept raw, no struct promoted.
#include "common/types.h"

extern void STCRDDEK_F0x800837b4(uint8_t *arg);
extern void STCRDDEK_F0x800835d8(uint8_t *arg);

void STCRDDEK_F0x800838c4(uint8_t *arg)
{
    int32_t kind;
    void (*fn)(uint8_t *);

    kind = *(int32_t *)(arg + 0x0c);
    if (kind == 1)
        goto L1;
    if (kind < 2)
        goto Ldef;
    if (kind == 2)
        goto L2;
    if (kind == 3)
        goto Lepi;
Ldef:
    fn = *(void (**)(uint8_t *))(arg + 0x38);
    fn(arg);
    *(int32_t *)(arg + 0x5c) = 0;
    goto Lepi;
L1:
    STCRDDEK_F0x800837b4(arg);
L2:
    STCRDDEK_F0x800835d8(arg);
Lepi: ;
}
