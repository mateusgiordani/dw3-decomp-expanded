// CARDGAME:0x8008ff08, 336-byte PAL body at verified base 0x80082cb0.
// File offset 0xd258 in reference/extracted/pro/cardgame.bin.
// Framed: prologue addiu sp,-0x18, sw s0,0x10(sp), sw ra,0x14(sp) (late);
// epilogue lw ra/s0, jr ra, addiu sp,+0x18. Next function at 0x80090058.
// Callers: 2x jal from CARDGAME:0x80084320 (a3=3 at 0x8008460c, a3=4 at 0x80084628).
// Ghidra CARDGAME read-only: disasm 84 insns + decompile CARDGAME_F0x8008ff08
// verified word-equal against PAL; x-ref to 0x8008ff08. No Ghidra state change.
// Hypothesis v3 (base O2/G0): v2 + pointer do/while tail clear (PAL keeps a
// decremented pointer in v0 with the decrement in the bgez delay slot;
// the index-counted for recomputed the address each iteration).
// Hypothesis v4 (W2-iter3, scaled-offset-before-base analog of 89f18):
// REFUTED as keeper (292 B, -44; control-shape change). Mechanism kept:
// a long-lived temp relocates parm homing (row->t0, p2 pushed to a0 with
// a new prologue copy; 81 prefs became {4,5}).
// Hypothesis v5 (W2-iter3, callback-arity): the indirect target is declared
// (void*,int32,int32) and the call passes (p2,idx,sel). PAL sets up ONLY
// a0 (delay slot); a1/a2 already hold idx/sel with zero setup insns, so
// the arity is codegen-free except for allocation prefs. CONFIRMED: moves
// move t0,a1 + move a1,a2 reproduced exactly, 316 -> 324 B, 17 -> 12 groups.
// Hypothesis v6 (W2-iter3, split scaled-offset temps): expand emits the
// just-computed mult operand FIRST in plus, so inline idx*200 always lands
// rs (PAL wants base rs). k=idx*200 shared by site1+arms (PAL v1 analog);
// k2=(idx*25)<<3 declared at the store site forces the PAL recompute while
// dodging CSE (different mult const). Both temps are pre-existing pseudos
// at their adds, so C order (base first) survives. CONFIRMED (20 -> 9 B
// with v5+v6+assoc combined as cardgame_ff08_w2r6assoc). v6b: address
// association rewritten ((cnt*2+k)+p1) to match PAL arm/store nesting.
// Hypothesis v7 (W2-iter3, tail addressing): q=p1+i with q[0x49e] store
// reproduces PAL addu v0,s0,v1 + sb 0x49e(v0) + counter/shared regs.
// CONFIRMED (tail words byte-equal). Hypothesis v8 (ladder polarity):
// outer-arm swap and switch form both COLLAPSED upstream equilibrium
// (312 B, moves gone) under the r6 shared-k allocation; r9 retests below.
// Hypothesis v9/H-A (r9, branch-local k; CONFIRMED 9 -> 5 diffs): arm-local
// ka/kb = idx*200 shadow top-k in each mode arm. cse2 canonicalizes both
// arms to the chain-value pseudo 93 (homed v1); top-k 87 keeps only the
// line-44 use, is renamed in place (v6 base-first order preserved) and the
// 87<-93 copy (insn 26, bne-delay move a0,v1) dies. w17+w29 fixed, 336 B kept.
// Hypothesis v9/H-B (r9, PAL-order ladder; CONFIRMED exact_byte_match):
// outer ladder swapped to cnt!=0-first fall-through (PAL layout); under the
// H-A equilibrium the 312 collapse does not occur — 336/336, zero diffs.
#include <stdint.h>

typedef void (*cardgame_ff08_cb_t)(void *obj, int32_t idx, int32_t sel);

void CARDGAME_F0x8008ff08(uint8_t *p1, void *p2, int32_t idx, int32_t mode) {
    int32_t sel;
    int32_t i;
    uint8_t *q;
    int32_t k = idx * 200;

    *(int32_t *)(p1 + 0x438) = *(int16_t *)(p1 + k + 0x5a6);
    if (mode == 4) {
        int32_t ka = idx * 200;
        sel = *(int16_t *)(*(int32_t *)(p1 + 0x440) * 2 + ka + p1 + 0x614);
    } else {
        int32_t kb = idx * 200;
        sel = *(int16_t *)(*(int32_t *)(p1 + 0x440) * 2 + kb + p1 + 0x5b0);
    }
    {
        int32_t k2 = (idx * 25) << 3;

        *(int16_t *)(*(int16_t *)(p1 + k2 + 0x5a6) * 2 + k2 + p1 + 0x600) = (int16_t)sel;
        *(int16_t *)(p1 + k2 + 0x5a6) = *(int16_t *)(p1 + k2 + 0x5a6) + 1;
    }
    if (*(int32_t *)(p1 + 0x438) != 0) {
        if (idx == 0) {
            p1[0x499] = 5;
        } else {
            p1[0x499] = 0xb;
            p1[0x49c] = 1;
        }
    } else {
        if (idx == 0) {
            p1[0x499] = 0x11;
        } else {
            p1[0x499] = 0x12;
            p1[0x49c] = 1;
        }
    }
    (*(cardgame_ff08_cb_t *)((uint8_t *)p2 + 0xec0))(p2, idx, sel);
    i = 0x27;
    q = p1 + i;
    do {
        q[0x49e] = 0;
        q--;
        i--;
    } while (i >= 0);
    p1[0x422] = 1;
}
