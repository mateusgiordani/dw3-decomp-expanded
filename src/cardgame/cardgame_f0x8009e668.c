// CARDGAME:0x8009e668 (size 1892, 0x764)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1bbb8
// Boundary: prologue addiu sp,sp,-0x30 (saves s0/s1/s2/s3/ra) at 0x8009e668;
//   epilogue lw ra/s3/s2/s1/s0 + jr ra/addiu sp,+0x30 at 0x8009edb0..0x8009edc8;
//   next CARDGAME:0x8009edcc (+0x764); prev FUN_8009e62c (60B) ends at +0x0.
//   functions.csv agrees (CARDGAME_F0x8009e668 DISASSEMBLED, [0x8009e668,0x8009edcc)).
// Ghidra CARDGAME (ddw3-pal-sles-03936, read-only): 473-insn disasm matches PAL
//   word-for-word (first8 d0ffbd27 1800b0af 21808000 2000b2af 2190a000 1c00b1af
//   98041126 2800bfaf; last4 1c00b18f 1800b08f 0800e003 3000bd27);
//   decompile CARDGAME_F0x8009e668(int param_1, int param_2), void return.
// Xref: 1 caller FUN_800a2df8 via 0x800a2e28 UNCONDITIONAL_CALL (in-overlay).
//   Direct callees CARDGAME:0x8009e024, CARDGAME:0x8009dbe8, FUN_8009df68.
//   Computed jumps via jt1 0x80083800 / jt2 0x80083848 (18 words each, in-overlay
//   rodata; entries verified against cardgame.bin). Indirect jalr via engine
//   slots +0xf24/+0xea4/+0xea8 off *(p2+0x18), and EXE word 0x8004df9c reached as
//   0x8004de10-base +0x18c (lui 0x8005 + addiu -0x21f0, extern array + index 0x63).
// Semantics: two-phase card-state machine. Phase 1 dispatches on *(p1+0x499):
//   1 = refresh via e024 + max(0x72c,0x79e)*8 budget + 40-byte copy loop,
//   2/3/4 = budget init (max*5, s8(0x575)*10, s8(0x575)*5+22),
//   5..0xf chain = halfword-pair select from table 0x800a5d2c via dbe8,
//   6..0x10 = mode pair via df68. Old/new state bytes shuffle at 0x498/0x499/0x49b.
//   Phase 2 dispatches on *(p1+0x498) and issues engine f24 queue calls plus EXE
//   accumulator calls, advancing 0x4cc/0x4d0 and setting follow-up states.
// Codegen notes: p1+0x498 family is accessed through a dedicated base (s1) while
//   p1 bytes (0x575/0x584/0x72c/0x79e/0x49e) use p1 (s0) directly; both bases are
//   kept in callee-saved regs across the direct calls. The EXE vector is read
//   through an extern array base (DAT_8004DE10, --symbol at link): a plain
//   0x8004df9c fptr constant would emit lui+lw, while extern array + 0x63 index
//   yields the observed lui/addiu base + lw 0x18c pair (same device as 878b4).
//   The 0x800a5d2c halfword table is likewise an extern symbol (lui 0x800a +
//   addiu 0x5d2c), not a numeric constant. f24 call sites pass *(p2+0x18) with
//   two textual derefs (slot base + a0 arg), matching the observed double lw;
//   only the 5/6-group sites reuse the cached eng value (see handoff readings).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base),
//   alt aspsx-2.86.
// Status: C_MATCHING (exact_byte_match, 1892/1892 bytes, rodata 144/144 bytes).
#include <stdint.h>

typedef int32_t (*cardgame_exe0_t)(void);
typedef void (*cardgame_eng2_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_eng3_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*cardgame_eng5_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3, int32_t a4);

extern int32_t DAT_8004DE10[];
extern int16_t DAT_800A5D2C[];
extern void CARDGAME_F0x8009e024(int32_t p1, int32_t p2);
extern void CARDGAME_F0x8009dbe8(int32_t p1, int32_t p2, int32_t a2, int32_t a3);
extern void FUN_8009df68(int32_t p1, int32_t p2, int32_t a2, int32_t a3);

void CARDGAME_F0x8009e668(int32_t p1, int32_t p2)
{
    int32_t base;
    int32_t eng;
    int32_t k;
    int32_t r;
    uint8_t c;

    base = p1 + 0x498;
    eng = *(int32_t *)(p2 + 0x18);
    k = 0;
    c = *(uint8_t *)(base + 1);
    if (c == 0)
        goto second;
    switch (c) {
    case 1:
        CARDGAME_F0x8009e024(p1, p2);
        *(int32_t *)(base + 0x30) = 3;
        *(int32_t *)(base + 0x34) = 0;
        *(int32_t *)(base + 0x38) = 4;
        {
            uint32_t m = *(uint8_t *)(p1 + 0x72c) > *(uint8_t *)(p1 + 0x79e) ? *(uint8_t *)(p1 + 0x72c) : *(uint8_t *)(p1 + 0x79e);
            *(uint32_t *)(base + 0x40) = m << 3;
        }
        {
            int32_t a = 0;
            int32_t b = 0;
            do {
                *(uint8_t *)(*(int32_t *)(p2 + 0x18) + b + 0x151) = *(uint8_t *)(p1 + a + 0x49e);
                a += 1;
                b += 0x4c;
            } while (a < 0x28);
        }
        break;
    case 3:
        *(int32_t *)(base + 0x30) = 3;
        *(int32_t *)(base + 0x34) = 0;
        *(int32_t *)(base + 0x38) = 4;
        *(int32_t *)(base + 0x40) = *(int8_t *)(p1 + 0x575) * 10;
        break;
    case 2:
        *(int32_t *)(base + 0x30) = 3;
        *(int32_t *)(base + 0x34) = 0;
        *(int32_t *)(base + 0x38) = 4;
        {
            uint32_t m = *(uint8_t *)(p1 + 0x72c) > *(uint8_t *)(p1 + 0x79e) ? *(uint8_t *)(p1 + 0x72c) : *(uint8_t *)(p1 + 0x79e);
            *(uint32_t *)(base + 0x40) = m * 5;
        }
        break;
    case 4:
        *(int32_t *)(base + 0x30) = 3;
        *(int32_t *)(base + 0x34) = 0;
        *(int32_t *)(base + 0x38) = 4;
        *(int32_t *)(base + 0x40) = *(int8_t *)(p1 + 0x575) * 5 + 0x16;
        break;
    case 5:
    case 0x11:
        k += 1;
    case 7:
        k += 1;
    case 9:
        k += 1;
    case 0xb:
    case 0x12:
        k += 1;
    case 0xd:
        k += 1;
    case 0xf:
        k += 1;
        {
            int32_t base_tbl = (int32_t)DAT_800A5D2C;
            int16_t *ptr = (int16_t *)(base_tbl + (k << 2));
            CARDGAME_F0x8009dbe8(p1, p2, ptr[0], ptr[1]);
        }
        break;
    case 6:
        FUN_8009df68(p1, p2, 0, 0);
        break;
    case 0xa:
        FUN_8009df68(p1, p2, 0, 1);
        break;
    case 8:
        FUN_8009df68(p1, p2, 0, 2);
        break;
    case 0x10:
        FUN_8009df68(p1, p2, 1, 1);
        break;
    case 0xe:
        FUN_8009df68(p1, p2, 1, 2);
        break;
    case 0xc:
        FUN_8009df68(p1, p2, 1, 0);
        break;
    }
shuffle:
    *(uint8_t *)(base + 3) = *(uint8_t *)(base + 1) + 1;
    *(uint8_t *)base = *(uint8_t *)(base + 1);
    *(uint8_t *)(base + 1) = 0;
second:
    switch (*(uint8_t *)base) {
    case 1:
        if (3 < *(int32_t *)(base + 0x38)) {
            if (*(int32_t *)(base + 0x34) < *(uint8_t *)(p1 + 0x72c))
                ((cardgame_eng5_t)*(int32_t *)(*(int32_t *)(p2 + 0x18) + 0xf24))(*(int32_t *)(p2 + 0x18), *(int32_t *)(base + 0x34), 10, 0x1000, 0x1000);
            if (*(int32_t *)(base + 0x34) < *(uint8_t *)(p1 + 0x79e))
                ((cardgame_eng5_t)*(int32_t *)(*(int32_t *)(p2 + 0x18) + 0xf24))(*(int32_t *)(p2 + 0x18), *(int32_t *)(base + 0x34) + 6, 10, 0x1000, 0x1000);
            *(int32_t *)(base + 0x34) += 1;
            *(int32_t *)(base + 0x38) -= 4;
        }
        r = ((cardgame_exe0_t)DAT_8004DE10[0x63])();
        *(int32_t *)(base + 0x30) += r;
        r = ((cardgame_exe0_t)DAT_8004DE10[0x63])();
        *(int32_t *)(base + 0x38) += r;
        if (*(int32_t *)(base + 0x40) < *(int32_t *)(base + 0x30))
            *(uint8_t *)(base + 1) = 3;
        break;
    case 3:
        if (3 < *(int32_t *)(base + 0x38)) {
            if (*(int32_t *)(base + 0x34) < *(int8_t *)(p1 + 0x575))
                ((cardgame_eng5_t)*(int32_t *)(*(int32_t *)(p2 + 0x18) + 0xf24))(*(int32_t *)(p2 + 0x18), *(int32_t *)(base + 0x34) + 0xc, 10, 0x1000, 0x1000);
            {
                int32_t q = *(int32_t *)(base + 0x34) - 1;
                if (q < *(int8_t *)(p1 + 0x575) && q >= 0)
                    ((cardgame_eng3_t)*(int32_t *)(*(int32_t *)(p2 + 0x18) + 0xea4))(*(int32_t *)(p2 + 0x18), q, *(uint8_t *)(p1 + (q << 3) + 0x584));
            }
            *(int32_t *)(base + 0x34) += 1;
            *(int32_t *)(base + 0x38) -= 4;
        }
        r = ((cardgame_exe0_t)DAT_8004DE10[0x63])();
        *(int32_t *)(base + 0x30) += r;
        r = ((cardgame_exe0_t)DAT_8004DE10[0x63])();
        *(int32_t *)(base + 0x38) += r;
        if (*(int32_t *)(base + 0x40) < *(int32_t *)(base + 0x30))
            *(uint8_t *)base = 0;
        break;
    case 2:
        if (3 < *(int32_t *)(base + 0x38)) {
            if (*(int32_t *)(base + 0x34) < *(uint8_t *)(p1 + 0x72c))
                ((cardgame_eng5_t)*(int32_t *)(*(int32_t *)(p2 + 0x18) + 0xf24))(*(int32_t *)(p2 + 0x18), *(int32_t *)(base + 0x34), 5, 0, 0x1000);
            if (*(int32_t *)(base + 0x34) < *(uint8_t *)(p1 + 0x79e))
                ((cardgame_eng5_t)*(int32_t *)(*(int32_t *)(p2 + 0x18) + 0xf24))(*(int32_t *)(p2 + 0x18), *(int32_t *)(base + 0x34) + 6, 5, 0, 0x1000);
            *(int32_t *)(base + 0x34) += 1;
            *(int32_t *)(base + 0x38) -= 4;
        }
        r = ((cardgame_exe0_t)DAT_8004DE10[0x63])();
        *(int32_t *)(base + 0x30) += r;
        r = ((cardgame_exe0_t)DAT_8004DE10[0x63])();
        *(int32_t *)(base + 0x38) += r;
        if (*(int32_t *)(base + 0x40) < *(int32_t *)(base + 0x30))
            *(uint8_t *)(base + 1) = 4;
        break;
    case 4:
        if (3 < *(int32_t *)(base + 0x38)) {
            if (*(int32_t *)(base + 0x34) < *(int8_t *)(p1 + 0x575)) {
                ((cardgame_eng5_t)*(int32_t *)(*(int32_t *)(p2 + 0x18) + 0xf24))(*(int32_t *)(p2 + 0x18), *(int32_t *)(base + 0x34) + 0xc, 5, 0, 0x1000);
                ((cardgame_eng2_t)*(int32_t *)(*(int32_t *)(p2 + 0x18) + 0xea8))(*(int32_t *)(p2 + 0x18), *(int32_t *)(base + 0x34));
            }
            *(int32_t *)(base + 0x34) += 1;
            *(int32_t *)(base + 0x38) -= 4;
        }
        r = ((cardgame_exe0_t)DAT_8004DE10[0x63])();
        *(int32_t *)(base + 0x30) += r;
        r = ((cardgame_exe0_t)DAT_8004DE10[0x63])();
        *(int32_t *)(base + 0x38) += r;
        if (*(int32_t *)(base + 0x40) < *(int32_t *)(base + 0x30))
            *(uint8_t *)base = 0;
        break;
    case 5:
    case 7:
    case 9:
    case 0xb:
    case 0xd:
    case 0xf:
        {
            int32_t v = *(int32_t *)(base + 0x3c);
            if (v == 0)
                v = 1;
            if (*(int32_t *)(base + 0x34) < v && 3 < *(int32_t *)(base + 0x38)) {
                ((cardgame_eng5_t)*(int32_t *)(eng + 0xf24))(eng, *(int32_t *)(base + 0x34), 10, 0x1000, 0x1000);
                *(int32_t *)(base + 0x34) += 1;
                *(int32_t *)(base + 0x38) -= 4;
            }
        }
        goto tail56;
    case 6:
    case 8:
    case 0xa:
    case 0xc:
    case 0xe:
    case 0x10:
        {
            int32_t v = *(int32_t *)(base + 0x3c);
            if (v == 0)
                v = 1;
            if (*(int32_t *)(base + 0x34) < v && 3 < *(int32_t *)(base + 0x38)) {
                ((cardgame_eng5_t)*(int32_t *)(eng + 0xf24))(eng, *(int32_t *)(base + 0x34), 5, 0, 0x1000);
                *(int32_t *)(base + 0x34) += 1;
                *(int32_t *)(base + 0x38) -= 4;
            }
        }
tail56:
        if (*(int32_t *)(base + 0x30) > *(int32_t *)(base + 0x40))
            *(uint8_t *)base = 0;
        r = ((cardgame_exe0_t)DAT_8004DE10[0x63])();
        *(int32_t *)(base + 0x30) += r;
        r = ((cardgame_exe0_t)DAT_8004DE10[0x63])();
        *(int32_t *)(base + 0x38) += r;
        break;
    case 0x11:
        r = 6;
        goto state_store;
    case 0x12:
        r = 0xc;
state_store:
        *(uint8_t *)base = 0;
        *(uint8_t *)(base + 3) = (uint8_t)r;
        break;
    }
}
