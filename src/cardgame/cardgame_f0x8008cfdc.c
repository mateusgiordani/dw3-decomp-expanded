// CARDGAME:0x8008cfdc (size 664, 0x298) -- EXACT_BYTE_MATCH (r6).
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xa32c (RAW, no header).
// Boundary: prologue 27bdffc8 addiu sp,-0x38, saves s2/s4/s5/ra/s3/s1/s0;
// s2=a0(p1), s4=a1(p2); epilogue lw ra/s5/s4/s3/s2/s1/s0 + jr ra +
// addiu sp,+0x38 at 0x8008d250-0x8008d270. Next framed CARDGAME:0x8008d274
// at +0x298 (27bdffb8 prologue), contiguous, no overlap.
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm 166
// words word-equal vs PAL; 1 caller CARDGAME_F0x80084320 via 0x800858f8;
// callees: EXE sys tablets (s1=0x8004b7d0 slots 0x3f4/0x408), absolute EXE
// vector *0x80055c48, card-method slots p2+0xf24/0xf18/0xec4/0xeb4.
// Shape (all portable C, no register/asm/volatile): 4-way dispatch on byte
// p1+0x422 (2 -> sysbit dispatch; 1/3 -> require-gated cases; else fail);
// value paths converge on one Lfun funnel (move v0,s5 + fallthrough
// restores at 0x8008d24c); all 7 fail paths share one Lheadfail block
// (return ret) so branch-delay fills are move v0,s5 (copy of s5 home)
// instead of rematerialized li v0,-1 (r6 R-a/R-b). Sysbit tests use unnamed
// call temps (sys0(0)>>sys1(0,imm))&1 so the save lands in the next jalr
// delay slot (r10 S); ret = 0 sits between the m2 and m1 calls (r10 C3).
// EXE sys base is an extern array (DAT_8004B7D0): plain constant would emit
// li+ori, extern yields lui+addiu. idx*76 stride is base-first addu (p2+off).
// Lineage: A2 funnel (r9) + S/C3 (r10) + shared-fail-return R-a/R-b (r6).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
// Status: C_MATCHING candidate (fn_exact_pipeline exact_byte_match 664/664,
// candidate sha256 == PAL dbac5542cf90654ccb99a5bf9d8df8421b5b8ce66887299b6c8ce66307ab4162).

#include <stdint.h>

typedef int32_t (*cardgame_sys0_t)(int32_t);
typedef int32_t (*cardgame_sys1_t)(int32_t, int32_t);
extern void (*D_80055C48)(uint32_t);
typedef void (*cardgame_m1_t)(int32_t);
typedef void (*cardgame_m2_t)(int32_t, int32_t);
typedef void (*cardgame_m5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);

extern int32_t DAT_8004B7D0[];

int32_t CARDGAME_F0x8008cfdc(int32_t p1, int32_t p2)
{
    int32_t ret = -1;
    int32_t st;
    int32_t k;
    int8_t idx;
    uint8_t idxu;

    st = *(uint8_t *)(p1 + 0x422);
    if (st == 2)
        goto Ldispatch;
    if (st < 3) {
        if (st == 1)
            goto Lcase1;
        goto Lheadfail;
    } else {
        if (st == 3)
            goto Lcase3;
    }
Lheadfail:
    return ret;
Lcase1:
    if (*(int16_t *)(p2 + 0x64) != 2)
        goto Lheadfail;
    if (*(uint8_t *)(p1 + 0x498) != 0)
        goto Lheadfail;
    *(uint8_t *)(p1 + 0x422) = 2;
    idx = *(int8_t *)(p1 + 0x575);
    if (idx == 0)
        goto Lfun;
    {
        uint8_t *u;
        u = (uint8_t *)(p2 + (idx + 11) * 76);
        u[0x150] |= 1;
    }
    goto Lfun;
Ldispatch:
    if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 13)) & 1) != 0) {
            uint8_t *u;
            if (*(int32_t *)(p1 + 0x438) != 1)
                goto Lheadfail;
            D_80055C48(0x4001c);
            idx = *(int8_t *)(p1 + 0x575);
            k = idx + 11;
            u = (uint8_t *)(p2 + k * 76);
            u[0x150] &= 0xfe;
            idxu = *(uint8_t *)(p1 + 0x575);
            *(int8_t *)(p1 + k + 0x46f) = idxu + 1;
            idx = *(int8_t *)(p1 + 0x575);
            *(int16_t *)(p1 + ((idx - 1) << 3) + 0x582) = idx;
            ret = 1;
            goto Lfun;
        } else {
            if (((((cardgame_sys0_t)DAT_8004B7D0[0xfd])(0) >> ((cardgame_sys1_t)DAT_8004B7D0[0x102])(0, 14)) & 1) != 0) {
                D_80055C48(0x800450bd);
                ((cardgame_m1_t)*(uint32_t *)(p2 + 0xec4))(p2);
                *(int32_t *)(p1 + 0x440) = 1;
                *(uint8_t *)(p1 + 0x422) = 3;
                ((cardgame_m5_t)*(uint32_t *)(p2 + 0xf24))(p2, 0xf, 4, 0, 0x1000);
                *(uint8_t *)(p1 + 0x499) = st;
                goto Lfun;
            }
            goto Lfun;
        }
    return ret;
Lcase3:
    if (*(int16_t *)(p2 + 0x64) != 0)
        goto Lheadfail;
    if (*(uint8_t *)(p1 + 0x498) != 0)
        goto Lheadfail;
    ((cardgame_m2_t)*(uint32_t *)(p2 + 0xf18))(p2, 0xf);
    ret = 0;
    ((cardgame_m1_t)*(uint32_t *)(p2 + 0xeb4))(p2);
Lfun:
    return ret;
}
