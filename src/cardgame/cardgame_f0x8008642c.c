// CARDGAME:0x8008642c, PAL-SLES-03936; full body 1516 bytes, no padding.
// Raw overlay: reference/extracted/pro/cardgame.bin, base 0x80082cb0,
// file offset 0x377c. Entry follows the predecessor's jr ra/delay slot;
// our epilogue ends at 0x80086a18, the next function's entry.
// PAL body SHA-256:
// 2cc2afbc8b47a8a8ce0ea238035a46d9dd7044d1b09d47d3af5e5ffdb11b023b
// Ten-entry jump table: 0x8008326c, 40 bytes, also compared to PAL.
// Caller: CARDGAME:0x80085a54, jal with p1=a0 and p2=a1.
// Read-only Ghidra disassembly was checked word-for-word against raw PAL.
//
// Matching recipe: psyq-gcc-2.8.1-sn32-4.0.0010 / aspsx-2.79,
// -O2 -G0 -mips1 -msoft-float, variant base,
// --symbol DAT_8004B7D0=0x8004b7d0 --rodata 0x8008326c.
// Revision 7 evidence: docs/c-matching-guide/submissions/cardgame-8008642c/.
// This is address-based C for the original 32-bit target, not a host port.
//
// Keep independent values independent: the timer result is read before the
// counter, and choice/flag temporaries have separate branch-local lifetimes.
// SYSBIT's emitted polling order and unmasked srav are verified with this
// pinned compiler; do not assume C operands are evaluated left-to-right by
// every compiler. The controller callback supplies the button bit index.
// The two state-7 actions are real, mutually exclusive source paths. Late
// cross-jumping merges them; sharing them earlier changes saved-register
// allocation. Revalidate the entire body and table after any refactoring.
#include <stdint.h>

typedef int32_t (*cardgame_sys0_t)(int32_t);
typedef int32_t (*cardgame_sys1_t)(int32_t, int32_t);
typedef int32_t (*cardgame_tick_t)(void);
typedef void (*cardgame_exevec_t)(uint32_t);
typedef void (*cardgame_m5_t)(int32_t, uint32_t, int32_t, int32_t, uint32_t);
typedef void (*cardgame_m1_t)(int32_t);
typedef void (*cardgame_m2_t)(int32_t, int32_t);

extern int32_t DAT_8004B7D0[];

#define CARDGAME_SYSBIT(sys_slot, arg_slot, imm) \
    ((((cardgame_sys0_t)DAT_8004B7D0[(sys_slot)])(0) >> (((cardgame_sys1_t)DAT_8004B7D0[(arg_slot)])(0, (imm)))) & 1)

int32_t CARDGAME_F0x8008642c(int32_t p1, int32_t p2)
{
    int32_t ret = 0;
    uint8_t st;
    int32_t w;
    int32_t elapsed;
    uint8_t v;

    st = *(uint8_t *)(p1 + 0x422);
    switch (st) {
    case 0:
        elapsed = ((cardgame_tick_t)*(uint32_t *)0x8004df9c)();
        w = *(int32_t *)(p1 + 0x424);
        w += elapsed;
        *(int32_t *)(p1 + 0x424) = w;
        if (w < 0x24)
            goto L_done;
        if (*(uint8_t *)(p1 + 0x498) != 0)
            goto L_done;
        *(int32_t *)(p1 + 0x424) = 0;
        if (*(int8_t *)(p1 + 0x575) == 0 || *(int32_t *)(p1 + 0x438) != 3) {
            *(uint8_t *)(p1 + 0x422) = 1;
            ((cardgame_m5_t)*(uint32_t *)(p2 + 0xee4))(p2, *(uint32_t *)(p2 + 0xde8),
                *(int32_t *)(p1 + 0x438), (int32_t)*(int16_t *)(p2 + 0xdf4),
                *(uint32_t *)(p2 + 0xde4));
            goto L_done;
        }
        goto L_st8;
    case 2:
        if (CARDGAME_SYSBIT(0xfd, 0x102, 13) != 0) {
            ((cardgame_m1_t)*(uint32_t *)(p2 + 0xeec))(p2);
            *(uint8_t *)(p1 + 0x422) = 7;
            break;
        }
        if (CARDGAME_SYSBIT(0xfd, 0x102, 14) == 0)
            goto L_c8;
        *(int32_t *)(p1 + 0x440) = 1;
        ((cardgame_m2_t)*(uint32_t *)(p2 + 0xef0))(p2, 1);
        ((cardgame_m1_t)*(uint32_t *)(p2 + 0xeec))(p2);
        *(uint8_t *)(p1 + 0x422) = 7;
        break;
    L_c8:
        if (CARDGAME_SYSBIT(0xfd, 0x102, 4) != 0)
            goto L_6638;
        if (CARDGAME_SYSBIT(0xfd, 0x102, 6) == 0)
            goto L_fetests;
    L_6638:
        if (*(int32_t *)(p1 + 0x438) == 2) {
            uint32_t nb;
            ((cardgame_exevec_t)*(uint32_t *)0x80055c48)(0x8004513eU);
            nb = *(uint32_t *)(p1 + 0x440) ^ 1U;
            *(uint32_t *)(p1 + 0x440) = nb;
            ((cardgame_m2_t)*(uint32_t *)(p2 + 0xef0))(p2, (int32_t)nb);
            goto L_done;
        }
    L_fetests:
        if (CARDGAME_SYSBIT(0xfe, 0x102, 11) == 0) {
            if (CARDGAME_SYSBIT(0xfd, 0x102, 10) != 0)
                goto L_st6;
        }
        if (CARDGAME_SYSBIT(0xfe, 0x102, 10) != 0)
            goto L_fdctest;
        if (CARDGAME_SYSBIT(0xfd, 0x102, 11) == 0)
            goto L_fdctest;
    L_st6:
        ((cardgame_m1_t)*(uint32_t *)(p2 + 0xee8))(p2);
        *(uint8_t *)(p1 + 0x422) = 6;
        break;
    L_fdctest:
        if (CARDGAME_SYSBIT(0xfd, 0x102, 12) == 0)
            goto L_done;
        ((cardgame_m1_t)*(uint32_t *)(p2 + 0xee8))(p2);
        *(int32_t *)(p1 + 0x424) = 0;
        *(uint8_t *)(p1 + 0x422) = 3;
        break;
    case 6: {
        uint32_t nb;
        if (*(uint8_t *)(p2 + 0xdfa) != 0)
            goto L_done;
        nb = *(uint32_t *)(p2 + 0xde4) ^ 2U;
        *(uint32_t *)(p2 + 0xde4) = nb;
        ((cardgame_m5_t)*(uint32_t *)(p2 + 0xee4))(p2, *(uint32_t *)(p2 + 0xde8),
            *(int32_t *)(p1 + 0x438), (int32_t)*(int16_t *)(p2 + 0xdf4), nb);
        *(uint8_t *)(p1 + 0x422) = 1;
        break;
    }
    case 7:
        if (*(uint8_t *)(p2 + 0xdfa) != 0)
            goto L_done;
        if (*(int32_t *)(p1 + 0x440) != 0)
            goto L_st8;
        *(uint8_t *)(p1 + 0x422) = 9;
        *(uint8_t *)(p1 + 0x499) = 2;
        ((cardgame_m1_t)*(uint32_t *)(p2 + 0xec4))(p2);
        goto L_done;
    L_st8:
        *(uint8_t *)(p1 + 0x422) = 8;
        break;
    case 3:
        switch (*(int32_t *)(p1 + 0x424)) {
        case 0:
            if (*(uint8_t *)(p2 + 0xdfa) != 0)
                goto L_done;
            *(uint8_t *)(p1 + 0x499) = 2;
            ((cardgame_m1_t)*(uint32_t *)(p2 + 0xec4))(p2);
            *(int32_t *)(p1 + 0x424) = 1;
            goto L_done;
        case 1:
            if (*(int16_t *)(p2 + 0x64) != 0)
                goto L_done;
            if (*(uint8_t *)(p1 + 0x498) != 0)
                goto L_done;
            *(uint8_t *)(p1 + 0x2f4) = 3;
            *(uint8_t *)(p1 + 0x568) = 0;
            *(uint8_t *)(p1 + 0x422) = 4;
            goto L_done;
        }
        break;
    case 4:
        *(uint8_t *)(p1 + 0x49d) = 1;
        *(uint8_t *)(p1 + 0x499) = 1;
        ((cardgame_m1_t)*(uint32_t *)(p2 + 0xec8))(p2);
        *(uint8_t *)(p1 + 0x422) = 5;
        *(int32_t *)(p1 + 0x424) = 0;
        break;
    case 5:
        switch (*(int32_t *)(p1 + 0x424)) {
        case 0:
            if (*(uint8_t *)(p1 + 0x498) != 0)
                goto L_done;
            if (*(int16_t *)(p2 + 0x64) != 2)
                goto L_done;
            ((cardgame_m5_t)*(uint32_t *)(p2 + 0xee4))(p2, *(uint32_t *)(p2 + 0xde8),
                *(int32_t *)(p1 + 0x438), (int32_t)*(int16_t *)(p2 + 0xdf4),
                *(uint32_t *)(p2 + 0xde4));
            *(int32_t *)(p1 + 0x424) = 1;
            goto L_done;
        case 1:
            goto L_case1;
        }
        break;
    case 1:
    L_case1:
        v = *(uint8_t *)(p2 + 0xdfa);
        if (v != 2)
            goto L_done;
        *(uint8_t *)(p1 + 0x422) = v;
        break;
    case 9:
        if (*(int16_t *)(p2 + 0x64) != 0)
            goto L_done;
        if (*(uint8_t *)(p1 + 0x498) != 0)
            goto L_done;
        goto L_tail;
    case 8:
    L_tail:
        if (*(int32_t *)(p1 + 0x440) == 0)
            ret = 1;
        else
            ret = 2;
        break;
    }
L_done:
    return ret;
}
