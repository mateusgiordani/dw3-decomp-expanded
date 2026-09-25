// CARDGAME:0x8009518c (size 124, 0x7c) -- C_MATCHING (exact_byte_match, rev3)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x124dc (RAW overlay, no header)
// Boundary: paired prologue 27bdffe8 / epilogue 03e00008+27bd0018, frame -0x18/+0x18;
//   prev CARDGAME:0x80095030 ends at 0x8009518c, next framed CARDGAME:0x80095274 (gap 0x6c leaf/data).
//   Sweep reports/handoffs/cardgame-boundary-sweep.md #99: 2 callers, 0 direct callees (indirect jalr).
// Callers (upstream guide only, cardgame.s): jal from L0x189c (a2=0) and L0x18b4 (a2=1), both
//   with a0=s1, a1=s0 then j to common continuation -- v0 return ignored at both sites.
// Ghidra: project ddw3-pal-sles-03936 program CARDGAME read-only, unanalyzed at this address
//   (No instruction at 0x8009518c, 0 seeded functions); no import/analysis mutation. PAL bytes authoritative.
// Upstream cardgame.s is GUIDE only (jal refs 0x0c025463, not body); no recomp/upstream C copied.
// Prior provenance (preserved): rev1 ASM_MATCHING exact via noreorder asm wrapper
//   (commit 8229d82f, candidate==reference sha256 1589de5dac2e631b3b8f7fe95ca5d0006c35d73a11810f7c7e2fdad795e527a1,
//   psyq-gcc-2.8.1-sn32-4.0.0010/aspsx-2.79 -O2 -G0). Per loop handover s5, rev2 recovered portable C
//   (C_NONMATCHING, 47 diffs); rev3 (this file) closes it to exact portable C, same toolchain/variant.
// Match recipe (one hypothesis per change, all base psyq-gcc-2.8.1-sn32-4.0.0010/aspsx-2.79 -O2 -G0):
//   H1 goto-funnel (47->46 diffs): two forward gotos give PAL beq/beq layout targets; dual-view
//     count loads (lb index temp c, hoisted lbu decrement source d) reproduce the preloaded lbu v1.
//   H2 base-first address temporary (46->44): tab=(u8*)ctx+off then tab[0x584] emits PAL addu $2,$16,$2
//     (base-first) instead of index-first; first 44 bytes exact.
//   H3 PAL-layout block order (44->10): call body, then ret2, then shared keeps branch2 as beq a2,0->ret2
//     with nop delay and puts li ret in the j delay slot.
//   H4 drop ret temp, duplicated constant stores (10->0, EXACT): call path stores 2 to ctx[0x499] then
//     1 to ctx[0x422] and returns; ret2 stores 2 to ctx[0x422]. GCC cross-jumps the common sb into a
//     single shared store with j-over-li (delay li v0,1), nop in jalr delay, everything in $2.
// Semantics (conservative, uncertainties noted):
//   - ctx (a0, saved to s0) is a byte-addressed context; 0x575 is a small count, signed for the
//     index calc (PAL lb) and unsigned for the decrement source (PAL lbu) -- both views kept.
//     Table at ctx+0x584 indexed as ((count-1)<<3), i.e. 8-byte entries, flag byte at entry offset 0
//     (struct layout UNKNOWN -- stride/field roles unconfirmed).
//   - If table flag != 0 and a2 == 0: store 2 to ctx[0x422] and skip the indirect call.
//     Otherwise: ctx[0x575]--, call *(void(**)(void*))((char*)a1+0xEC4) with a1, store 2 to
//     ctx[0x499], store 1 to ctx[0x422].
//   - Declared void because both known callers ignore v0; PAL leaves 1/2 in v0 as scratch.
//     Whether the original returned int (1/2) is UNCERTAIN (not tested: void is exact).
//   - Indirect callee arity UNCERTAIN: at least (void*) with a0=a1; a1/a2/a3 regs pass through.
//   - Field roles for 0x422/0x499/0x575/0x584/0xEC4 are offsets only, no struct invented.
// Status: C_MATCHING -- exact_byte_match, candidate sha256 == reference sha256
//   1589de5dac2e631b3b8f7fe95ca5d0006c35d73a11810f7c7e2fdad795e527a1, reloc 1, same 124 B size.

void CARDGAME_F0x8009518c(signed char *ctx, void *a1, int a2) {
    void (*fn)(void *);
    int c = ctx[0x575];
    int off = (c - 1) << 3;
    unsigned char *tab = (unsigned char *)ctx + off;
    int flag = tab[0x584];
    unsigned char d = ((unsigned char *)ctx)[0x575];
    if (flag == 0)
        goto call;
    if (a2 == 0)
        goto ret2;
call:
    ctx[0x575] = d - 1;
    fn = *(void (**)(void *))((char *)a1 + 0xEC4);
    fn(a1);
    ctx[0x499] = 2;
    ctx[0x422] = 1;
    return;
ret2:
    ctx[0x422] = 2;
}
