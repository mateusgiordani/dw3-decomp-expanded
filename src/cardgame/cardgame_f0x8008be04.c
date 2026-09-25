// CARDGAME:0x8008be04 (size 576, 0x240)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x9154
// Framed function: prologue 27bdffd0 addiu sp,-0x30, saves s0-s6/ra;
// s5=a0 (state), s6=a1 (table base); s3 = 0x8004b7d0 EXE vector base
// (lui v0,0x8005 + addiu s3,v0,-0x4830 = 0x8004b7d0, via extern D_8004B7D0);
// slots 0x3fc->*0x8004bbcc, 0x3f4->*0x8004bbc4, 0x408->*0x8004bbd8
// (all EXE-resident, READ-only xrefs). Absolute addresses do NOT reproduce the
// hoist (each access re-emits li+ori and spills two extra saved regs, +16 B),
// so the source addresses the table through the 0x8004b7d0 base pointer.
// Epilogue lw ra/s6/s5/s4/s3/s2/s1/s0 + jr ra + addiu sp,+0x30 at 0x8008c01c-40.
// Next framed CARDGAME:0x8008c044 at +0x240, size contiguous, no overlap.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm 144 words
// match PAL; decompile hypothesis matches the control flow below.
// Callers: 1 direct jal from CARDGAME_F0x8008c044 (0x8008c400).
// Callees: 2 direct jal to CARDGAME_F0x8008bc8c (0x8008bf14 a2=-1, 0x8008bfb0 a2=+1);
// 10 indirect jalr via EXE vectors (BBCC/BBC4/BBD8 slots, results shuffled into
// s2/s1/s0 in subsequent jalr delay slots, last result shifted in v0).
// Body: mark record idx=state[0x43c] active in table (stride 76: byte+0x150 |= 1,
// half+0x12e = 1; same idx*76 sll/addu/sll/subu/sll address idiom as 0x8008bc8c).
// Then bit-test chain: (BBCC(0) & 1<<BBD8(0,7)) | (BBC4(0) & 1<<BBD8(0,7)) set and
// idx>0 -> step -1 via bc8c; else (BBCC(0) & 1<<BBD8(0,5)) | (BBC4(0) & 1<<BBD8(0,5))
// set and idx<max-1 (state[0x434]-1, signed slt) -> step +1 via bc8c;
// else if ((BBC4(0)>>BBD8(0,13))&1 (srav) and state byte [0x446+idx] != 0 (lb),
// state[0x423] = 3. All paths set state[0x445] = 0 (delay-slot sb on taken paths).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
// Status: exact_byte_match candidate r3 (portable C, no asm, no explicit
// register variables). EXE base via extern D_8004B7D0 (lui/addiu, hoisted
// after the record stores); per-arm distinct call-result variables keep three
// values live so saves fill subsequent jalr delay slots (s2/s1/s0) with the
// last shift in v0; masks/shifts/ands computed after all four calls.
// Reference sha256 over the 576 B body @0x9154:
// 35ff5737f586d24bef5fe003bff990888c46065369c0c2c5990ecefefbf47bfa.
// Exact pipeline attempted (all with --symbol CARDGAME_F0x8008bc8c=0x8008bc8c):
// - base 2.8.1, absolute slot addresses: object 592 > 576 (each access re-emits
//   li 0x80040000 + ori and spills two extra saved regs; frame 0x38).
// - base 2.8.1, exe-base-pointer C (this file): object 560 != 576 (-16 B).
// - alt1 2.7.2-cygnus-970404 + aspsx-2.79, same C: object 564 != 576.
// History: r2 blocker (literal 0x8004b7d0 -> li/ori, 560/564 B) resolved in
// r3 by extern D_8004B7D0 (lui/addiu, 568 B) plus per-arm distinct result
// variables (third live value -> 8th saved reg, 576 B exact). Prior cc1
// microprobe of a bare `return (uint32_t *)0x8004b7d0` is not representative:
// through the extern symbol the same cc1 emits PAL's lui/addiu pair.
#include <stdint.h>

typedef uint32_t (*cardgame_exe0_t)(int32_t);
typedef uint32_t (*cardgame_exe1_t)(int32_t, int32_t);

extern void CARDGAME_F0x8008bc8c(int32_t p1, int32_t p2, int32_t p3);
extern uint32_t D_8004B7D0[];

void CARDGAME_F0x8008be04(int32_t p1, int32_t p2)
{
    uint32_t *exe;
    uint32_t s2;
    int32_t s0;
    uint8_t *t1, *t2;

    t1 = (uint8_t *)p2 + *(int32_t *)(p1 + 0x43c) * 76;
    t1[0x150] |= 1;
    t2 = (uint8_t *)p2 + *(int32_t *)(p1 + 0x43c) * 76;
    *(uint16_t *)(t2 + 0x12e) = 1;
    exe = D_8004B7D0;
    {
        uint32_t r0 = ((cardgame_exe0_t)exe[0xff])(0);
        uint32_t q1 = ((cardgame_exe1_t)exe[0x102])(0, 7);
        uint32_t r1 = ((cardgame_exe0_t)exe[0xfd])(0);
        uint32_t q2 = ((cardgame_exe1_t)exe[0x102])(0, 7);
        s2 = (r0 & (1u << q1)) | (r1 & (1u << q2));
    }
    if (s2 != 0) {
        if (*(int32_t *)(p1 + 0x43c) > 0)
            CARDGAME_F0x8008bc8c(p1, p2, -1);
    } else {
        uint32_t r0 = ((cardgame_exe0_t)exe[0xff])(0);
        uint32_t q1 = ((cardgame_exe1_t)exe[0x102])(0, 5);
        uint32_t r1 = ((cardgame_exe0_t)exe[0xfd])(0);
        uint32_t q2 = ((cardgame_exe1_t)exe[0x102])(0, 5);
        s2 = (r0 & (1u << q1)) | (r1 & (1u << q2));
        if (s2 != 0) {
            if (*(int32_t *)(p1 + 0x43c) < *(int32_t *)(p1 + 0x434) - 1)
                CARDGAME_F0x8008bc8c(p1, p2, 1);
        } else {
            int32_t r3 = (int32_t)((cardgame_exe0_t)exe[0xfd])(0);
            int32_t q3 = (int32_t)((cardgame_exe1_t)exe[0x102])(0, 13);
            s0 = (r3 >> q3) & 1;
            if ((s0 != 0) && (*(int8_t *)(p1 + *(int32_t *)(p1 + 0x43c) + 0x446) != 0))
                *(uint8_t *)(p1 + 0x423) = 3;
        }
    }
    *(uint8_t *)(p1 + 0x445) = 0;
}
