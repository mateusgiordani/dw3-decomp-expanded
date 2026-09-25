// CARDGAME:0x8008d938 (size 244, 0xF4)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xac88
// Framed function from boundary sweep (symbols/function_labels.csv:
// CARDGAME,0x8008d938,function,CARDGAME_F0x8008d938,244).
// Prologue 27bdffd8 addiu sp,-0x28 ; sw s1,0x1c(sp) ; move s1,a0 ; move a0,a1 ;
// move a1,a2 ; sw s2,0x20(sp) ; sw ra,0x24(sp) ; sw s0,0x18(sp).
// Epilogue lw ra,0x24(sp) ; lw s2,0x20(sp) ; lw s1,0x1c(sp) ; lw s0,0x18(sp) ;
// jr ra ; addiu sp,+0x28. Next CARDGAME:0x8008da2c at +0xF4 confirms size.
// State machine on *(int *)(arg0 + 0x424): state 1 checks the table byte and,
// on match, stores 2 and calls slot *(arg1 + 0xf24) with (arg1,arg2,5,0,0x1000)
// (5th word at sp+0x10); state 2 checks the table byte and returns 1 on match;
// any other state calls slot *(arg1 + 0xf30) with (arg1,arg2), stores 1,
// returns 0. The source dispatches with forward gotos so the generator keeps
// the PAL block order (dispatch inline, other-state block, state-1 block,
// state-2 block) including the slti <2 test (PAL 28620002/14400003/1062001c).
// Table byte is t[0x14a] with t = arg1 + arg2 * 76 (lbu; *76 via the PAL
// sll2/addu/sll2/subu/sll2 strength reduction whose first sll sits in the
// preceding beq delay slot). The named byte-pointer temp in its own statement
// keeps the base-first addu order (sibling C_MATCHING cardgame_f0x8009c9a0
// idiom: p = (char *)a0 + a1 * 76); an inline int-arithmetic address folds to
// the swapped addu order instead. Incoming a3 is never read; its stale value
// flows to the +0xf30 slot, reproduced by declaring only three parameters.
// Ghidra program CARDGAME read-only (never import): disasm matches PAL
// word-for-word; decompile CARDGAME_F0x8008d938(param_1,param_2,param_3) shows
// this if/else shape (hypothesis only, confirmed against disasm, not copied).
// Callers (x-ref read-only): CARDGAME_F0x8008daf4+0x140 (0x8008db34) and
// CARDGAME_F0x8008f790+0x6c (0x8008f7fc), both UNCONDITIONAL_CALL with jal word
// 0c02364e; upstream cardgame.s .L0x0000ae84/.L0x0000cb4c GUIDE only.
// Frame keeps s0=1 constant, s1=arg0 (live across the indirect calls), s2=ret.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base),
// exact_byte_match via normal codegen (no __asm__, no .word, no byte arrays).
#include <stdint.h>

int32_t CARDGAME_F0x8008d938(int32_t arg0, uint8_t *arg1, int32_t arg2) {
    int32_t state;
    int32_t ret;
    uint8_t *t;

    state = *(int32_t *)(arg0 + 0x424);
    ret = 0;
    if (state == 1) {
        goto st1;
    }
    if (state < 2) {
        goto other;
    }
    if (state == 2) {
        goto st2;
    }
other:
    ((void (*)(int32_t, int32_t))*(void **)(arg1 + 0xf30))(arg1, arg2);
    *(int32_t *)(arg0 + 0x424) = 1;
    goto out;
st1:
    t = arg1 + arg2 * 76;
    if (t[0x14a] == 1) {
        *(int32_t *)(arg0 + 0x424) = 2;
        ((void (*)(int32_t, int32_t, int32_t, int32_t, int32_t))*(void **)(arg1 + 0xf24))(arg1, arg2, 5, 0, 0x1000);
    }
    goto out;
st2:
    t = arg1 + arg2 * 76;
    if (t[0x14a] == 1) {
        ret = 1;
    }
out:
    return ret;
}
