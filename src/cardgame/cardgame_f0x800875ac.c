// CARDGAME:0x800875ac (size 776, 0x308)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x48fc
// Prologue 27bdffc8 addiu sp,-0x38 ; sw s4,0x20(sp) ; move s4,a0 ; sw ra,0x34(sp) ; sw s8..s0 (9 callee-saved + ra, frame 0x38)
// Body: off=idx*76 (x4+x1=x5,x4=x20,-x1=x19,x4=x76) base-first addu v0,s6,v0; lbu/ori/sb bit0 at p2+off+0x150; sh 1 at p2+off+0x12e (s8=1); pad tablets via s3=0x8004b7d0 base (lw 0x3f4=*0x8004bbc4, 0x408=*0x8004bbd8, 0x3fc=*0x8004bbcc); (b0>>(b1)&1) tests for imm 0xe/0xc; EXE (*0x80055c48)(0x800450bd) with lui+ori arg; mode byte p1+0x423 = 0xe/0xb/5; word p1+0x440 = -1/idx; limit *(short*)(p3+10); pair tests (A&(1<<B))|(C&(1<<D)) single-branch; lb test p1+idx+0x446; sb 1 at p1+idx+0x46f; jal CARDGAME_F0x800870a4(p1,p2,p3,+-1 step)
// Epilogue lw ra,0x34(sp) ... ; jr ra ; addiu sp,+0x38 at 0x800878ac/0x800878b0; next CARDGAME:0x800878b4 at +0x308 confirms size 0x308 contiguous (prologue/epilogue pair).
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm 194 insns verified word-equal against PAL bytes @ 0x48fc (first8 LE 27bdffc8 afb40020 0080a021 afbf0034 afbe0030 afb7002c afb60028 afb50024; last4 LE 8fb10014 8fb00010 03e00008 27bd0038); decompile CARDGAME_F0x800875ac; x-ref to from CARDGAME_F0x80087edc at 0x80088604; callees 2x jal 0x800870a4 (a3=+1/-1), jalr tablets, EXE *0x80055c48.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base):
// exact_byte_match 776/776 (r8 o55) with --symbol D_8004B7D0=0x8004b7d0 (PAL builds
// the pad table base with lui/addiu %hi/%lo, i.e. a symbol, not a constant).
// The two long-lived 1s: the stores use literal 1 (CSE keeps that value in s8 for the
// sh at +0x12e and the sb at +0x46f), the shift masks use their own m = 1 (s5).
#include <stdint.h>

typedef int32_t (*cardgame_pad0_t)(int32_t);
typedef int32_t (*cardgame_pad1_t)(int32_t, int32_t);
typedef void (*cardgame_exe_cb_t)(uint32_t);
void CARDGAME_F0x800870a4(int32_t p1, int32_t p2, int32_t p3, int32_t step);

extern uint32_t D_8004B7D0[];

#define TBL_SLOT253()   ((cardgame_pad0_t)D_8004B7D0[253])(0)
#define TBL_SLOT255()  ((cardgame_pad0_t)D_8004B7D0[255])(0)
#define TBL_SLOT258(n)  ((cardgame_pad1_t)D_8004B7D0[258])(0, (n))

void CARDGAME_F0x800875ac(int32_t p1, int32_t p2, int32_t p3)
{
    int32_t off;
    int32_t m;

    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(uint8_t *)(p2 + off + 0x150) |= 1;
    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(int16_t *)(p2 + off + 0x12e) = 1;
    if ((TBL_SLOT253() >> TBL_SLOT258(0xe)) & 1) {
        ((cardgame_exe_cb_t)*(uint32_t *)0x80055c48)(0x800450bd);
        *(int32_t *)(p1 + 0x440) = -1;
        *(int8_t *)(p1 + 0x423) = 0xe;
    }
    if ((TBL_SLOT253() >> TBL_SLOT258(0xc)) & 1)
        *(int8_t *)(p1 + 0x423) = 0xb;
    if (*(int16_t *)(p3 + 10) != 0) {
        m = 1;
        if ((TBL_SLOT255() & (m << TBL_SLOT258(7))) | (TBL_SLOT253() & (m << TBL_SLOT258(7)))) {
            if (0 < *(int32_t *)(p1 + 0x43c))
                CARDGAME_F0x800870a4(p1, p2, p3, -1);
        } else if ((TBL_SLOT255() & (m << TBL_SLOT258(5))) | (TBL_SLOT253() & (m << TBL_SLOT258(5)))) {
            if (*(int32_t *)(p1 + 0x43c) < *(int16_t *)(p3 + 10) - 1)
                CARDGAME_F0x800870a4(p1, p2, p3, 1);
        } else if (((TBL_SLOT253() >> TBL_SLOT258(0xd)) & 1) && *(int8_t *)(p1 + *(int32_t *)(p1 + 0x43c) + 0x446) != 0) {
            *(int8_t *)(p1 + 0x423) = 5;
            *(int32_t *)(p1 + 0x440) = *(int32_t *)(p1 + 0x43c);
            /* Index re-read from +0x440: CSE reuses the stored value, no copy. */
            *(int8_t *)(p1 + *(int32_t *)(p1 + 0x440) + 0x46f) = 1;
        }
    }
}
