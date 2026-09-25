/* STCRDDEK:0x80083314 (136B, 34 words, file-off 0x664 = vaddr - 0x80082cb0).
 * PAL bytes (authority): reference/extracted/pro/stcrddek.bin base 0x80082cb0.
 * All 34 words match Ghidra disasm word-for-word (prologue 27bdffe0 at
 * 0x80083314, terminal jr ra + delay-slot addiu sp at 0x80083394/0x80083398).
 * Boundary: entry + 136 = 0x8008339c, which is the next function's prologue
 * (addiu sp,sp,-0x20 at 0x8008339c); previous function STCRDDEK:0x800832e8
 * ends with jr ra + delay-slot addiu sp at 0x8008330c/0x80083310, so the
 * range is contiguous with no overlap or gap.
 * Ghidra project ddw3-pal-sles-03936, program STCRDDEK, read-only: disasm
 * 34 insns matches PAL; decompile agrees on the indirect-call + init +
 * direction-dependent divide shape; x-ref to 0x80083314 empty (no direct
 * in-overlay callers; likely table/callback-driven, not proven). No Ghidra
 * state change.
 * Behavior (conservative): call the function pointer at obj+0x28 as
 * fp(obj, 1), store 1 at obj+0x10, store arg1 at obj+0x58, then if arg1
 * is 0 store 0 at obj+0x5c and (0xff00 / arg2) at obj+0x60, else store
 * 0xff00 at obj+0x5c and -(0xff00 / arg2) at obj+0x60. The arg1==0 case
 * is written first so the zero path stays on the fall-through path, as in
 * the original branch layout (bne to the nonzero block). The 0xff00 / arg2
 * division is the signed MIPS div (div + mflo). Struct layout beyond these
 * five words is unrecovered, so the object stays int32_t. The indirect
 * call is preserved as-is per R15 (owning table unknown; slot +0x28).
 * upstream stcrddek.s is GUIDE-only and was never copied. No domain pack:
 * the body is a generic callback + word-init + integer divide with no
 * battle/camera/skill/record/dialogue/field/sprite/disc/overlay evidence.
 * Toolchain attempts (all -O2, portable C, no asm, no register variables):
 * (base) psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79: object text 208 > 136;
 *   the two 3-operand div macros expand with zero/overflow guards absent
 *   from PAL (build/match/.../STCRDDEK_func_80083314.s shows div $2,$2,$18
 *   and div $3,$2,$18; the .obj carries break-7 guard words).
 * (alt1) psyq-gcc-2.95.2-4.0.0030 + aspsx-2.86 + o2-g2952-no-div-check
 *   (-mno-check-zero-division): size 136 exact, difference_count 18
 *   (candidate b448c99e...2512e99af vs PAL 53517e69...a19b9195): the bare
 *   div+mflo form is produced, but g2952 allocates the slot-0x28 target to
 *   v1 (lw $3 / jalr $3 vs PAL v0) and schedules both post-div stores ahead
 *   of mflo (sw/mflo swap in each arm). Precedent: fightstg-800a17c8
 *   (a286 preserves g2952's div-$0 form unchecked; a279 does not).
 * (alt2) psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.86: still 208 bytes; a286
 *   also expands the rd!=0 3-operand div macro with guards, so 2.8.1 cannot
 *   reach the bare div+mflo pair with any pinned assembler.
 * Status: C_NONMATCHING (recovered, reviewed, compilable; exact attempted,
 * blocker explicit). Next ranked hypotheses for a follow-up revision (not
 * attempted; two-alternate budget spent): g281 + older aspsx (2.67/2.56,
 * if their div-macro lacks guards), or g2952 at O1/nosched to keep the
 * 2.8.1 allocation with bare divs. */
#include <stdint.h>

void STCRDDEK_func_80083314(int32_t arg0, int32_t arg1, int32_t arg2)
{
    (*(void (**)(int32_t, int32_t))(arg0 + 0x28))(arg0, 1);
    *(int32_t *)(arg0 + 0x10) = 1;
    *(int32_t *)(arg0 + 0x58) = arg1;
    if (arg1 == 0) {
        *(int32_t *)(arg0 + 0x5c) = 0;
        *(int32_t *)(arg0 + 0x60) = 0xff00 / arg2;
    } else {
        *(int32_t *)(arg0 + 0x5c) = 0xff00;
        *(int32_t *)(arg0 + 0x60) = -(0xff00 / arg2);
    }
}
