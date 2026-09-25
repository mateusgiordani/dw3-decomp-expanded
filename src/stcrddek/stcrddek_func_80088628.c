// STCRDDEK:0x80088628 (size 12)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x5978 (22904)
// Boundary: reports/investigations/pro-310-triage/modules/stcrddek-inventory.json
//   reviewed_functions: address 2148042280, range [22904, 22916), body-80088628.
//   prev body-800884e8 ends exactly at 22904 (jr ra + addiu sp,sp,0x58);
//   next body-80088634 starts at 22916 (addiu sp,sp,-0x30). Size 12 = 3 words.
//   NOTE: do NOT derive size from cached Ghidra Function body; authority is
//   inventory size + disassembly (jr ra with delay-slot store ends the body).
// Ghidra STCRDDEK (ddw3-pal-sles-03936, read-only, no state change):
//   disasm 0x80088628 (10 insns incl. next-body lookahead):
//     80088628  ac85005c  sw   a1,0x5c(a0)
//     8008862c  03e00008  jr   ra
//     80088630  ac860060  _sw  a2,0x60(a0)   (delay slot, executes before return)
//   matches PAL bytes 5c0085ac 0800e003 600086ac word-for-word.
//   decompile: void STCRDDEK_func_80088628(int param_1, u4 param_2, u4 param_3)
//     { *(u4 *)(param_1 + 0x5c) = param_2; *(u4 *)(param_1 + 0x60) = param_3; }
//   callees: empty (leaf). function list entry 0x80088628 size 12 agrees but is
//   advisory only; boundary above is the authority.
// Xref: one direct UNCONDITIONAL_CALL from STCRDDEK_func_800887d4 at 0x80088854
//   (jal 0x80088628; delay-slot li a2,0x100); callers graph depth-2 agrees.
//   Semantics of +0x5c/+0x60 fields and of the 0x100 constant are unconfirmed;
//   deck-menu role is a name-based hypothesis only.
// Upstream guide only (never authority, never copied):
//   upstream/ddw3/asm/dw2003/pro/stcrddek.s .L0x00005ba4 jal 0x80088628.
//   FIGHTSTG .L0x80088628 (sw $v0,($s3)) is a different module at the same
//   runtime address; never merged (overlay identity is MODULE:address).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base),
//   exact_byte_match. Alternates only if base fails (max 2).
// Status: C_MATCHING (portable C, no asm, no register variables;
//   pipeline exact_byte_match, difference_count 0).
#include <stdint.h>

void STCRDDEK_func_80088628(int32_t *p, int32_t arg1, int32_t arg2)
{
    p[23] = arg1;
    p[24] = arg2;
}
