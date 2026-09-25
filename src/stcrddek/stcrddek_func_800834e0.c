/* STCRDDEK:0x800834e0 (size 180, 45 words, file-off 0x830 = vaddr - 0x80082cb0).
 * PAL bytes (authority): reference/extracted/pro/stcrddek.bin base 0x80082cb0.
 * Boundary: frame addiu sp,sp,-0x18 with sw ra,0x10(sp); terminal jr ra with
 * addiu sp,sp,+0x18; next function STCRDDEK:0x80083594 (addiu sp,sp,-0x18)
 * starts immediately after, so the 45-word range is contiguous with no overlap
 * or gap. symbols/function_labels.csv already records STCRDDEK,0x800834e0,
 * function, 180. Ghidra project ddw3-pal-sles-03936, program STCRDDEK,
 * read-only: disasm 45 insns matches PAL word-for-word (Ghidra byte strings
 * are the same words byte-swapped, e.g. e8ffbd27 == 27bdffe8); decompile
 * STCRDDEK_func_800834e0 agrees on the state-dispatch + accumulate/clamp
 * shape (hypothesis only, confirmed against disasm, not copied); x-ref to:
 * none (reached via overlay dispatch/table, no direct jal in this program);
 * x-ref from: intra-function conditional jumps plus one UNCONDITIONAL_CALL to
 * STCRDDEK:0x8008339c. No Ghidra state change.
 * Behavior (conservative): dispatch on word at +0xc. State 1: if word at
 * +0x10 is zero return; else add word at +0x60 into word at +0x5c, store the
 * sum unconditionally, then clamp by direction word at +0x58: direction zero
 * with sum > 0xff00 sets state 2 and stores 0xff00; nonzero direction with
 * sum < 0 sets state 2 and stores 0; the state word is written once at the
 * join through the state local (PAL duplicates li v0,2 into both branch
 * delay slots with a single shared sw at the join); then call
 * STCRDDEK_func_8008339c. State
 * 2: call STCRDDEK_func_8008339c. State 3: return without calling. Any other
 * state: indirect call *(arg0+0x38)(arg0), then return. Struct layout beyond
 * these six words is unrecovered, so the object stays int32_t *. Source uses
 * forward gotos so the generator keeps the PAL block order (dispatch inline,
 * indirect block, state-1 block, shared tail call) including the slti <2 test
 * (PAL 28620002 in the first beq delay slot). No domain pack: the body is a
 * generic state/accumulator update with one indirect callback and one local
 * direct call; it touches no battle/camera, skill, Digimon-record, dialogue,
 * field/map, sprite/rendering, disc-I/O or overlay-loader state, so no
 * read-only domain applies.
 * Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (variant base,
 * first hypothesis, no alternates needed). The next-state word uses its own
 * local ("next"): sharing the dispatch "state" local miscolors the join
 * store into v1/a0 and inserts a nop, while a fresh short-range local takes
 * v0 and duplicates li v0,2 into both branch delay slots exactly as in PAL.
 * Status: C_MATCHING (portable C, no asm; fn_exact_pipeline exact_byte_match,
 * base O2, difference_count 0, sha 4236b8ec...d8511194). */
#include "common/types.h"

extern void STCRDDEK_func_8008339c(int32_t *arg0);

void STCRDDEK_func_800834e0(int32_t *arg0) {
    int32_t state;
    int32_t sum;
    int32_t next;

    state = arg0[3];
    if (state == 1) {
        goto state1;
    }
    if (state < 2) {
        goto indirect;
    }
    if (state == 2) {
        goto tail;
    }
    if (state == 3) {
        return;
    }
indirect:
    ((void (*)(int32_t *))arg0[14])(arg0);
    return;
state1:
    if (arg0[4] == 0) {
        return;
    }
    sum = arg0[23] + arg0[24];
    arg0[23] = sum;
    if (arg0[22] != 0) {
        goto negdir;
    }
    if (sum <= 0xFF00) {
        goto tail;
    }
    next = 2;
    arg0[23] = 0xFF00;
    goto set2;
negdir:
    if (sum >= 0) {
        goto tail;
    }
    next = 2;
    arg0[23] = 0;
set2:
    arg0[3] = next;
tail:
    STCRDDEK_func_8008339c(arg0);
}
