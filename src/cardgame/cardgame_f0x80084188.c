// CARDGAME:0x80084188 (size 180, 0xb4) -- C_MATCHING (portable C, exact_byte_match)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x14d8
// Boundary: sweep reports/handoffs/cardgame-boundary-sweep.md #2; prologue
// 27bdffd0 (addiu sp,-0x30) to epilogue jr ra + addiu sp,+0x30; frame -0x30;
// prev gap leaf cluster, next CARDGAME:0x8008423c at +0xb4 contiguous
// (Ghidra disasm tail confirms 27bdffe8 prologue at 0x8008423c).
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only:
// disasm 0x80084188 (50 insns) + decompile + graph callers (1 caller:
// CARDGAME_F0x80084320 at 0x80085cdc) + callees (none; indirect jalr v0).
// cardgame.s GUIDE only, never copied. No Ghidra state change.
// Semantics: 12-slot byte flags at p+0x46f[i] cleared each pass; for i>=6
// call the 0x820 callback as fn(p, 1, 1 << (i-6)); track strictly-greatest
// return in best/best_idx, clearing the old best slot, storing i at
// p+0x440 (int) and setting the new best slot to 1.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 base, no alternates.
// Pipeline: --name CARDGAME_F0x80084188 --base 0x80082cb0 --address 0x80084188
// --size 180 --body-size 180 --opt-level O2 --variant base (no --symbol: the
// only call is indirect via the 0x820 field; no rodata/jump tables).
// Result: exact_byte_match, diff 0,
// sha a59166b42ea6080264a4afe1cccc6b95282b89ebc173484d90c5ea5cace35811.
// Matching note: base-first pointer additions (*(p + i + 0x46f)) are required;
// plain p[i + 0x46f] subscripts let gcc 2.8.1 emit addu index,base (3 words
// differ at file-offs 0x150e/0x1542/0x1566) while all other codegen matches.

typedef int (*cardgame_84188_fn_t)(unsigned char *, int, int);

void CARDGAME_F0x80084188(unsigned char *p)
{
    int best = 0;
    int best_idx = 6;
    int i = 0;
    int v;

    do {
        *(p + i + 0x46f) = 0;
        if (i >= 6) {
            v = ((cardgame_84188_fn_t)*(void **)(p + 0x820))(p, 1, 1 << (i - 6));
            if (best < v) {
                *(p + best_idx + 0x46f) = 0;
                *(int *)(p + 0x440) = i;
                *(p + i + 0x46f) = 1;
                best = v;
                best_idx = i;
            }
        }
        i++;
    } while (i < 12);
}
