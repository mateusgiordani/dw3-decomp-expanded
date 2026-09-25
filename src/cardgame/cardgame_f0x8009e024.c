// CARDGAME:0x8009e024 (size 1340, 0x53c)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1b374
// Boundary: prologue 27bdffc0 (addiu sp,sp,-0x40) at 0x8009e024; epilogue jr ra + 27bd0040 at
// 0x8009e558/0x8009e55c; next CARDGAME:0x8009e560 (d0ffbd27) at +0x53c. Size 1340 contiguous.
// Ghidra CARDGAME (ddw3-pal-sles-03936, read-only, no mutation): disasm in 24-160 insn chunks
// matches PAL word-for-word (head 27bdffc0 afbe0038 0080f021 afb30024; tail 8fb10014 8fb00010
// 03e00008 27bd0040); decompile CARDGAME_F0x8009e024(int param_1, int param_2).
// Xrefs: 1 caller CARDGAME_F0x8009e668 via 0x8009e6cc UNCONDITIONAL_CALL (upstream cardgame.s
// jal at file-off 0x1ba1c agrees); indirect callees via card slots *(s1+0xf14)/(s1+0xf3c)/(s1+0xf40).
// Jump tables (PAL bytes, BitConverter-verified): JT1 @0x800837c0, 10 entries
// (8009e278,8009e304,8009e35c,8009e3b4,8009e3e0 x5,8009e4fc) under sltiu 0xa check, so the source
// switch has cases 0-9 (case 9 is a no-op falling to the loop end) plus default; JT2 @0x800837e8,
// 5 entries (8009e444,8009e464,8009e488,8009e4a0,8009e4c4) under addiu -4 / sltiu 5 check for the
// inner switch on cases 4-8. Tables are contiguous (0x800837c0+40 == 0x800837e8).
// Externs: DAT_8005CCB0 (EXE word, lui 0x8006 + lw -0x3350) is the table index; DAT_800A5958
// (lui 0x800a + addiu 0x5958) is the 16-byte-entry table (fields 0/4 in group A, 8/12 in group B).
// Codegen notes: DAT loads stay inside the loops (calls block hoisting); F14 args nest
// (table[idx]+acc) so the addu schedules into the jalr delay slot; acc += 0x2900 schedules into
// the F3C jalr delay slot; case1/case2 init v1 = 0 then conditional = 6 (clear in branch delay).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, variant o2-g0-no-strength-reduce.
// r7 (o55/s0923f): table base as integer local set right after i = 0 (lui before the loop
// inits); one inner counter j shared by every case (saved across the case 4-8 calls); case 4-8
// constant 1 is a local set before its loop (PAL li s4,1 there, not hoisted out of the outer
// loop); case 0 selects the element pointer in the branches and does one compare/store.
// exact_byte_match 1340/1340. Details: strategy-r7-o55/attempts-r7.
#include <stdint.h>

typedef int32_t (*cardgame_f14_t)(int32_t, int32_t, int32_t, int32_t);
typedef int32_t (*cardgame_f3c_t)(int32_t, int32_t, int32_t);
typedef int32_t (*cardgame_f40_0_t)(int32_t, int32_t);
typedef int32_t (*cardgame_f40_1_t)(int32_t, int32_t);

extern int32_t DAT_8005CCB0;
extern int32_t DAT_800A5958[];

void CARDGAME_F0x8009e024(int32_t p1, int32_t p2)
{
    int32_t s1 = *(int32_t *)(p2 + 0x18);
    int i;
    int32_t acc;
    int32_t q;
    int32_t r;
    uint8_t u8;
    int32_t off30;
    int32_t off2c;
    int32_t row;
    int j;
    int32_t tb;

    i = 0;
    tb = (int32_t)DAT_800A5958;
    r = p1;
    acc = 0;
    q = s1;
    for (; i < 6; i++) {
        int32_t *tp;

        if (i >= *(uint8_t *)(p1 + 0x72c))
            break;
        tp = (int32_t *)(DAT_8005CCB0 * 16 + tb);
        ((cardgame_f14_t)*(int32_t *)(s1 + 0xf14))(s1, i, tp[0] + acc, tp[1]);
        ((cardgame_f3c_t)*(int32_t *)(s1 + 0xf3c))(s1, i, *(int16_t *)(r + 0x72e));
        acc += 0x2900;
        *(uint8_t *)(q + 0x14b) = *(uint8_t *)(r + 0x734);
        u8 = *(uint8_t *)(r + 0x736);
        r += 0xe;
        *(uint16_t *)(q + 0x120) = 0;
        *(uint8_t *)(q + 0x14c) = u8;
        q += 0x4c;
    }

    {
        int32_t base;
        int32_t tb2;

        i = 0;
        tb2 = (int32_t)DAT_800A5958;
        base = 0x1c8;
        r = p1;
        acc = 0;
        for (; i < 6; i++) {
            int32_t v1;
            int32_t *tp;
            int32_t k;

            if (i >= *(uint8_t *)(p1 + 0x79e))
                break;
            k = i + 6;
            tp = (int32_t *)(DAT_8005CCB0 * 16 + tb2);
            ((cardgame_f14_t)*(int32_t *)(s1 + 0xf14))(s1, k, tp[2] + acc, tp[3]);
            ((cardgame_f3c_t)*(int32_t *)(s1 + 0xf3c))(s1, k, *(int16_t *)(r + 0x7a0));
            v1 = s1 + base;
            *(uint8_t *)(v1 + 0x14b) = *(uint8_t *)(r + 0x7a6);
            u8 = *(uint8_t *)(r + 0x7a8);
            *(uint16_t *)(v1 + 0x120) = 0;
            *(uint8_t *)(v1 + 0x14c) = u8;
            if (*(uint8_t *)(p1 + 0x49c) != 0)
                *(uint8_t *)(v1 + 0x14d) = 2;
            base += 0x4c;
            r += 0xe;
            acc += 0x2900;
        }
    }

    *(uint8_t *)(p1 + 0x49c) = 0;
    i = 0;
    if (*(int8_t *)(p1 + 0x575) <= 0)
        return;

    row = p1;
    off30 = 0x390;
    off2c = 0x5100;
    do {
        int32_t v1 = s1 + off30;
        int32_t s0 = i + 12;

        ((cardgame_f14_t)*(int32_t *)(s1 + 0xf14))(s1, s0, off2c, 0x6100);
        ((cardgame_f3c_t)*(int32_t *)(s1 + 0xf3c))(s1, s0, *(int16_t *)(row + 0x580));
        *(uint16_t *)(v1 + 0x120) = 0;
        *(uint8_t *)(v1 + 0x14e) = (uint8_t)(i + 1);
        *(uint8_t *)(v1 + 0x146) = 0;
        *(uint8_t *)(v1 + 0x147) = 0;
        *(uint8_t *)(v1 + 0x148) = 0;
        *(uint16_t *)(v1 + 0x12e) = 0;
        if (*(int16_t *)(row + 0x582) != 0)
            *(uint8_t *)(s1 + (*(int16_t *)(row + 0x582) + off30) + 0x146) = 1;
        switch (*(uint8_t *)(row + 0x585)) {
        case 0: {
            int32_t rw;
            int32_t s2x;
            int32_t a1;
            int32_t a0;

            j = 0;
            rw = row;
            s2x = i;
            a1 = 0x74c;
            a0 = 0x72e;
            do {
                int32_t e;

                if (j < 6) {
                    if (j >= *(uint8_t *)(p1 + 0x72c))
                        goto next0;
                    e = p1 + a0;
                } else {
                    if (j - 6 >= *(uint8_t *)(p1 + 0x79e))
                        goto next0;
                    e = p1 + a1;
                }
                if (*(uint8_t *)(e + 0xc) == *(uint8_t *)(rw + 0x586))
                    *(uint8_t *)(s1 + s2x + 0x146) = 1;
            next0:
                s2x += 0x4c;
                a1 += 0xe;
                j++;
                a0 += 0xe;
            } while (j < 12);
            break;
        }
        case 1: {
            int32_t v1b = 0;
            int32_t base;

            if (*(uint8_t *)(row + 0x584) != 0)
                v1b = 6;
            j = 0;
            {
                int32_t o = v1b * 76 + 0x108;

                base = s1 + o;
            }
            do {
                *(uint8_t *)(base + i + 0x3e) = 1;
                base += 0x4c;
                j++;
            } while (j < 6);
            break;
        }
        case 2: {
            int32_t v1b = 0;
            int32_t base;

            if (*(uint8_t *)(row + 0x584) == 0)
                v1b = 6;
            j = 0;
            {
                int32_t o = v1b * 76 + 0x108;

                base = s1 + o;
            }
            do {
                *(uint8_t *)(base + i + 0x3e) = 1;
                base += 0x4c;
                j++;
            } while (j < 6);
            break;
        }
        case 3: {
            int32_t v1b;

            j = 0;
            v1b = i;
            while (j < 12) {
                *(uint8_t *)(s1 + v1b + 0x146) = 1;
                j++;
                v1b += 0x4c;
            }
            break;
        }
        case 4:
        case 5:
        case 6:
        case 7:
        case 8: {
            int32_t s2x;
            int32_t one;
            int32_t k6;
            int32_t rp;

            j = 0;
            one = 1;
            s2x = i;
            k6 = -0x54;
            rp = p1;
            do {
                int32_t a1v;

                if (j < 6)
                    a1v = *(int16_t *)(rp + 0x72e);
                else
                    a1v = *(int16_t *)(p1 + k6 + 0x7a0);
                switch (*(uint8_t *)(row + 0x585)) {
                case 4:
                    if (((cardgame_f40_0_t)*(int32_t *)(s1 + 0xf40))(s1, a1v) != one)
                        *(uint8_t *)(s1 + s2x + 0x146) = one;
                    break;
                case 5:
                    if (((cardgame_f40_0_t)*(int32_t *)(s1 + 0xf40))(s1, a1v) != 2)
                        *(uint8_t *)(s1 + s2x + 0x146) = one;
                    break;
                case 6:
                    if (((cardgame_f40_0_t)*(int32_t *)(s1 + 0xf40))(s1, a1v) == 3)
                        *(uint8_t *)(s1 + s2x + 0x146) = one;
                    break;
                case 7:
                    if (((cardgame_f40_0_t)*(int32_t *)(s1 + 0xf40))(s1, a1v) != 4)
                        *(uint8_t *)(s1 + s2x + 0x146) = one;
                    break;
                case 8:
                    if (((cardgame_f40_1_t)*(int32_t *)(s1 + 0xf40))(s1, a1v) == 6)
                        *(uint8_t *)(s1 + s2x + 0x146) = one;
                    break;
                }
                s2x += 0x4c;
                k6 += 0xe;
                j++;
                rp += 0xe;
            } while (j < 12);
            break;
        }
        case 9:
        default:
            break;
        }
        off30 += 0x4c;
        off2c += 0x3200;
        row += 8;
        i++;
    } while (i < *(int8_t *)(p1 + 0x575));
}
