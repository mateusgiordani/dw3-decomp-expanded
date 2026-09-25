// STCRDDEK:0x8008a36c (size 108, 0x6c)
// PAL: reference/extracted/pro/stcrddek.bin base 0x80082cb0 file-off 0x76bc
// Boundary: reports/investigations/pro-310-triage/modules/stcrddek-inventory.json
//   body-8008a36c: address 2148049772, instruction_bytes 108, range [2148049772, 2148049880);
//   prev body-8008a2d8 (148 B) ends exactly at 2148049772; range ends at CODE interval end (30504).
//   NOTE: the assignment reference line naming cardgame.bin is a template error. CARDGAME bytes
//   at the same file offset are mid-function code; stcrddek.bin bytes match Ghidra word-for-word.
// Ghidra STCRDDEK (ddw3-pal-sles-03936, read-only): 27-insn disasm matches PAL bytes exactly;
//   decompile: gate word at +0xc; sum = *(+8) + *(+4) stored back to +8; if *(+4) < 1 and sum < 0
//   clamp +8 to 0 and clear +0xc; else if *(+4) >= 1 and sum > 0x1000 clamp +8 to 0x1000 and
//   clear +0xc; return 1 on gate-closed/clamp, 0 when accumulated in range.
// Xref: Ghidra x-ref to 0x8008a36c empty (indirect calls expected, no direct jal); inventory
//   proposes caller STCRDDEK 0x80085210 via static_indirect_call at 11+ sites (e.g. 0x80085298);
//   upstream stcrddek.s .L0x00008b8c holds .word 0x8008a36c (guide only, never authority).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base), exact_byte_match,
//   no alternates needed. Attempt history: (1) decompiler-mirrored nesting emitted bgtz +
//   slt-macro (32 B diff); (2) outer (p[1] > 0) fixed blez but inlined a return-0 (116 B obj);
//   (3) this single-trailing-return else-if ladder shares one RET0 epilogue and matches.
// Status: C_MATCHING (portable C, no asm; pipeline exact_byte_match, difference_count 0).
#include <stdint.h>

int32_t STCRDDEK_func_8008a36c(int32_t *p)
{
    int32_t sum;

    if (p[3] == 0)
        return 1;
    sum = p[2] + p[1];
    p[2] = sum;
    if (p[1] > 0) {
        if (sum > 0x1000) {
            p[2] = 0x1000;
            p[3] = 0;
            return 1;
        }
    } else if (sum < 0) {
        p[2] = 0;
        p[3] = 0;
        return 1;
    }
    return 0;
}
