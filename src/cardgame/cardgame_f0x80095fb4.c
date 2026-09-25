// CARDGAME:0x80095fb4 (size 400, 0x190)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x13304
// Framed function per symbols/function_labels.csv (boundary sweep): prologue
// addiu sp,sp,-0x20 / sw s0,0x10(sp) / move s0,a0 / sw ra,0x1c(sp) /
// sw s2,0x18(sp) / sw s1,0x14(sp); epilogue lw ra/s2/s1/s0 / jr ra /
// addiu sp,sp,+0x20. Next CARDGAME:0x80096144 (gap leaves), prev
// CARDGAME_F0x80095e58 ends at 0x80095fb4. No overlap.
// Ghidra CARDGAME read-only: disasm 100 insns 0x80095fb4..0x80096140, first
// word 27bdffe0 == PAL file-off 0x13304 word 0, last word 27bd0020 == PAL
// file-off 0x13492 word 99 (sha256 d85f24b9...7569f, 400B). Decompile shows
// state dispatch on *(a0+0xc) with cases 1/2/3/default, sub-dispatch on byte
// *(a0+0x62), two ((lh +0x64 << 12) / lh +0x66) div blocks, two tick calls
// through EXE data word 0x8004df9c (lui 0x8005 + lw -0x2064), indirect slots
// +0x28(obj,int)/+0x38(obj), and tail jal CARDGAME_F0x80095e58 (hypothesis
// only; confirmed against disasm, not copied as source).
// Xrefs read-only: one PARAM caller CARDGAME_F0x80096180+0x18 (0x80096198)
// installs this function as the 0x74-byte object template via EXE 0x80014504
// (cf. cardgame_f0x80096180.c); tail callee CARDGAME_F0x80095e58; EXE data
// word 0x8004df9c also read by EXE FUN_80019224 (tick callback slot).
// Upstream guide (GUIDE only): upstream/ddw3/asm/dw2003/pro/cardgame.s.
// Object layout stays untyped raw offsets (same 0x74-byte object as the
// 0x80096180 allocator: +0xc state, +0x54/+0x56 params, +0x58/+0x5a fixed,
// +0x62 mode, +0x63 flag, +0x64/+0x66 counters, +0x68/+0x6c/+0x70 handlers).
// The 0x1000 constant is 1.0 in 12-bit fractional fixed point: remaining =
// 0x1000 - ((cur << 12) / max); init uses 10/10; completion sets 0x5a.
// Register evidence kept as control flow: PAL compares the mode byte against
// the state register (beq v1,s1, no li) and stores the state register into
// the mode byte (sb s1), so the source reads mode == state / stores state;
// the value is provably 1 on that path (state == 1 dominates). The two
// slot-0x28 calls share one jalr site (second jumps in with its own a1),
// hence the fn/cmd + goto docall shape.
// Block layout mirrors PAL address order (dispatch, init, case-1 with
// chk2/calcA/cb2, case-2, shared jalr, tail) so branch senses match; mode is
// a signed int local (PAL uses slti, not sltiu, on the lbu value).
// PAL div sites are bare div/mflo with no break guards: build with the
// pinned psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 variant base with
// --strip-div-guard (supported div normalization: 0/196 overlay division
// sites guarded). The two slot-0x28
// calls are one fn(obj, N) call per path (commands 2 and 3); cross-jumping
// merges them into the single shared jalr with PAL delay-slot scheduling.
// Full-range exact_byte_match 400/400 (see strategy-r7).
// Toolchain base attempted first: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79
// -O2 -G0 (div guards emitted; rejected -mno-check-zero-division).
#include <stdint.h>

extern void CARDGAME_F0x80095e58(void *obj);
extern int (*EXE_D_8004DF9C)(void);

void CARDGAME_F0x80095fb4(uint8_t *obj) {
    int32_t state = *(int32_t *)(obj + 0x0c);
    void (*fn)(uint8_t *, int32_t);

    if (state == 1)
        goto doCase1;
    if (state < 2)
        goto doInit;
    if (state == 2)
        goto doCase2;
    if (state == 3)
        goto tail;
doInit:
    (*(void (**)(uint8_t *))(obj + 0x38))(obj);
    *(int16_t *)(obj + 0x66) = 10;
    *(int16_t *)(obj + 0x64) = 10;
    *(obj + 0x62) = 0;
    *(int16_t *)(obj + 0x58) = 0x1000;
    *(int16_t *)(obj + 0x5a) = 0;
    goto tail;
doCase1: {
        int mode = *(obj + 0x62);

        if (mode == state)
            goto tail;
        if (mode >= 2)
            goto chk2;
        if (mode == 0)
            goto doA;
        goto tail;
    chk2:
        if (mode == 2)
            goto docb2;
        goto tail;
    doA: {
            int32_t q = ((int32_t)*(int16_t *)(obj + 0x64) << 12) / (int32_t)*(int16_t *)(obj + 0x66);
            int32_t left;

            *(int16_t *)(obj + 0x5a) = (int16_t)(0x1000 - q);
            left = EXE_D_8004DF9C();
            left = (int32_t)*(uint16_t *)(obj + 0x64) - left;
            *(int16_t *)(obj + 0x64) = (int16_t)left;
            if (left * 0x10000 > 0)
                goto tail;
            *(int16_t *)(obj + 0x5a) = 0x1000;
            *(obj + 0x62) = (uint8_t)state;
            goto tail;
        }
    docb2:
        fn = *(void (**)(uint8_t *, int32_t))(obj + 0x28);
        fn(obj, 2);
        goto tail;
    }
doCase2: {
        int32_t q = ((int32_t)*(int16_t *)(obj + 0x64) << 12) / (int32_t)*(int16_t *)(obj + 0x66);
        int32_t left;

        *(int16_t *)(obj + 0x5a) = (int16_t)q;
        left = EXE_D_8004DF9C();
            left = (int32_t)*(uint16_t *)(obj + 0x64) - left;
        *(int16_t *)(obj + 0x64) = (int16_t)left;
        if (left * 0x10000 > 0)
            goto tail;
        fn = *(void (**)(uint8_t *, int32_t))(obj + 0x28);
        *(int16_t *)(obj + 0x5a) = 0;
        fn(obj, 3);
    }
tail:
    CARDGAME_F0x80095e58(obj);
}
