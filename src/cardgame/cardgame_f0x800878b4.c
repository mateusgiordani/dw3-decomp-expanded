// CARDGAME:0x800878b4 (size 716, 0x2cc)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x4c04
// Boundary: prologue 27bdffc8 (addiu sp,sp,-0x38) at 0x800878b4; epilogue jr ra + 27bd0038 at 0x80087b78/0x80087b7c; next CARDGAME:0x80087b80 (27bdffd0) at +0x2cc.
// Ghidra CARDGAME (ddw3-pal-sles-03936, read-only): 179-insn disasm in 40-insn chunks matches PAL word-for-word (first8 27bdffc8 afb50024 0080a821 afbf0030 afb7002c afb60028 afb40020 afb3001c; last4 8fb10014 8fb00010 03e00008 27bd0038); decompile CARDGAME_F0x800878b4(int param_1, int param_2, int param_3).
// Xref: 1 caller CARDGAME_F0x80087edc via 0x8008861c UNCONDITIONAL_CALL (upstream cardgame.s jal at file-off 0x596c agrees); direct callees CARDGAME:0x800870a4 (x2, a3=+-1) and CARDGAME:0x80087458 (x1, result tested); indirect jalr via EXE words 0x8004bbc4/0x8004bbcc/0x8004bbd8 through shared base 0x8004b7d0 (lui 0x8005, addiu -0x4830 into s3).
// Codegen notes: shared base is an extern array (DAT_8004B7D0, --symbol at link): a plain (int32_t *)0x8004b7d0 constant makes cc1 2.8.1 emit li 0x80040000 + ori, while extern yields lui %hi + addiu %lo (probe-verified, base toolchain). Indirect calls are nested (not temp-split) so each result is saved late in the next jalr delay slot. Status combos test '!= 0' with the single-call arm first, giving the original beq-into-else-body layout and fall-through final store (no extra j).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base), +<=2 alternates.
// Status: C_MATCHING (portable C, no asm; exact_byte_match on base toolchain, no alternates needed).
#include <stdint.h>

typedef int32_t (*cardgame_sys0_t)(int32_t);
typedef int32_t (*cardgame_sys1_t)(int32_t, int32_t);

extern int32_t DAT_8004B7D0[];
extern void CARDGAME_F0x800870a4(int32_t p1, int32_t p2, int32_t p3, int32_t step);
extern int32_t CARDGAME_F0x80087458(int32_t p1, int32_t p2, int32_t p3);

void CARDGAME_F0x800878b4(int32_t p1, int32_t p2, int32_t p3)
{
    int32_t off;

    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(uint8_t *)(p2 + off + 0x150) |= 1;
    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(int16_t *)(p2 + off + 0x12e) = 1;
    if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 15)) & 1) != 0)
        *(uint8_t *)(p1 + 0x423) = 6;
    if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 12)) & 1) != 0)
        *(uint8_t *)(p1 + 0x423) = 11;
    if (*(int16_t *)(p3 + 10) != 0) {
        if (((((cardgame_sys0_t)DAT_8004B7D0[0xff])(0) & (1 << ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 7))) |
             (((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) & (1 << ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 7)))) != 0) {
            if (0 < *(int32_t *)(p1 + 0x43c))
                CARDGAME_F0x800870a4(p1, p2, p3, -1);
        } else if (((((cardgame_sys0_t)DAT_8004B7D0[0xff])(0) & (1 << ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 5))) |
                    (((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) & (1 << ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 5)))) != 0) {
            if (*(int32_t *)(p1 + 0x43c) < *(int16_t *)(p3 + 10) - 1)
                CARDGAME_F0x800870a4(p1, p2, p3, 1);
        } else if ((((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 13)) & 1) != 0) &&
                   (CARDGAME_F0x80087458(p1, p2, p3) != 0)) {
            *(uint8_t *)(p1 + 0x423) = 4;
        }
    }
}
