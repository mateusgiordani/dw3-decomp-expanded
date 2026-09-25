#include "common/types.h"

/* CARDGAME:0x800915d4 (312B, 0x138) -- portable C recovery (rev 7).
 * Hypothesis 7: early exits return ret (0 on those paths), so GCC presets
 * v0 from s1 (move v0,s1) rather than $0 in the three shared-epilogue
 * delay slots at 0x80091608/0x8009161c/0x80091694.
 * PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x0e924.
 * Boundary sweep reports/handoffs/cardgame-boundary-sweep.md #78: prologue
 * 27bdffe0 addiu sp,-0x20, epilogue 03e00008 jr ra + 27bd0020 addiu sp,+0x20,
 * frame -0x20/+0x20 HIGH confidence; next CARDGAME:0x80091858 at +0x384.
 * Ghidra ddw3-pal-sles-03936/CARDGAME read-only: disasm 78 insns matches PAL
 * words; decompile reviewed (hypothesis only). x-ref to: 2 callers in
 * CARDGAME_F0x80084320 @ 0x80085728 (a2=0) and 0x80085748 (a2=1).
 * x-ref from: 2 callees 0x8008daf4 (a2==0 path) and 0x8008dd38 (a2!=0 path),
 * both (a0, a1, *(+0x42c), *(+0x430)); on nonzero return state 2->1, ++idx.
 * State machine on byte@+0x422: 1 = scan signed bytes @+0x46f+idx for first
 * nonzero while idx<12 (lb => int8_t); found => state 2, f424=0, f428=idx,
 * f42c=(idx<6)^1, f430=idx (idx-6 when idx>=6); exhausted => state 3.
 * Returns 1 only when entry state==3, else 0.
 * Hypothesis 6 (decisive probe evidence): a branch-probe compiled with the
 * exact pinned toolchain shows GCC 2.8.1 lowers switch(state) {1,2,3} to
 * exactly PAL's dispatch chain -- beq for ==2, slt+beq for the <3 split,
 * beq plus inline shared-epilogue jump for ==1/==3 -- while every if/else
 * and goto spelling normalizes to bne (hyps 1-5, constant 296 B). The
 * switch decision tree (middle case first) matches PAL's test order with
 * case bodies emitted in source order. This also explains the 120 failed
 * engine trials: the generator only tried if/else shapes.
 * Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 base. */
extern int32_t CARDGAME_F0x8008daf4(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
extern int32_t CARDGAME_F0x8008dd38(int32_t a0, int32_t a1, int32_t a2, int32_t a3);

int32_t CARDGAME_F0x800915d4(int32_t a0, int32_t a1, int32_t a2)
{
    int32_t ret = 0;
    int32_t idx;
    uint8_t state;

    state = *(uint8_t *)(a0 + 0x422);
    switch (state) {
    case 1:
        idx = *(int32_t *)(a0 + 0x428);
        if (idx < 12) {
            int32_t flag;

            do {
                if (*(int8_t *)(a0 + idx + 0x46f) != 0) {
                    flag = idx < 6;
                    *(uint8_t *)(a0 + 0x422) = 2;
                    *(int32_t *)(a0 + 0x424) = 0;
                    *(int32_t *)(a0 + 0x428) = idx;
                    *(int32_t *)(a0 + 0x42c) = flag ^ 1;
                    *(int32_t *)(a0 + 0x430) = idx;
                    if (flag == 0)
                        *(int32_t *)(a0 + 0x430) = idx - 6;
                    break;
                }
                idx++;
            } while (idx < 12);
            if (idx < 12)
                return ret;
        }
        *(uint8_t *)(a0 + 0x422) = 3;
        break;
    case 2: {
        int32_t r;

        if (a2 == 0)
            r = CARDGAME_F0x8008daf4(a0, a1, *(int32_t *)(a0 + 0x42c), *(int32_t *)(a0 + 0x430));
        else
            r = CARDGAME_F0x8008dd38(a0, a1, *(int32_t *)(a0 + 0x42c), *(int32_t *)(a0 + 0x430));
        if (r != 0) {
            *(uint8_t *)(a0 + 0x422) = 1;
            *(int32_t *)(a0 + 0x428) = *(int32_t *)(a0 + 0x428) + 1;
        }
        break;
    }
    case 3:
        ret = 1;
        break;
    default:
        return ret;
    }
    return ret;
}
