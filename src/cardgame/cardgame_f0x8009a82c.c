// CARDGAME:0x8009a82c (size 364, 0x16c)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x17b7c
// Framed function from symbols/function_labels.csv (boundary sweep):
// prologue 27bdff40 addiu sp,-0xc0, sw s0,0xb0(sp), move s0,a0, sw ra,0xb8(sp),
// sw s1,0xb4(sp); epilogue lw ra,0xb8(sp), lw s1,0xb4(sp), lw s0,0xb0(sp),
// jr ra, addiu sp,+0xc0. Next CARDGAME:0x8009a998 contiguous, size 0x16c exact.
// Ghidra CARDGAME (ddw3-pal-sles-03936, read-only): disasm 91 insns at 0x8009a82c,
// decompile CARDGAME_F0x8009a82c(param_1); 1 caller CARDGAME_F0x8009b168 via
// 0x8009b4f8 jal; 1 direct callee EXE 0x8001f648 (jal, a0=sp+0x10) plus 6 computed
// jalr via stack table sp+0x84/0x88/0x8c/0x94/0x9c/0xa4 filled by that helper.
// Words compared with reference bin at off 0x17b7c: head 27bdff40 afb000b0
// 00808021 ... tail 8fbf00b8 8fb100b4 8fb000b0 03e00008 27bd00c0, all equal.
// cardgame.s / upstream ddw3 GUIDE only, never copied. No Ghidra state change.
// Stack table family matches C_MATCHING CARDGAME_F0x8009af28 pattern:
// F0x8001f648(buf) fills uint8_t buf[0xa0] at sp+0x10, fn slots at
// buf+0x74/0x78/0x7c/0x84/0x8c/0x94 (sp+0x84/0x88/0x8c/0x94/0x9c/0xa4).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
// Symbols: F0x8001f648=0x8001f648, CARDGAME_F0x8009a82c=0x8009a82c,
#include <stdint.h>

extern uint8_t D_800A5AD8[];

extern void F0x8001f648(void *a0);

void CARDGAME_F0x8009a82c(int32_t p)
{
    uint8_t buf[0xA0];
    int32_t t0;
    int32_t t1;
    uint8_t *base;
    int16_t *P;
    uint8_t *Q;
    int32_t off0;
    int32_t off1;
    int32_t q;
    int32_t q32;
    int32_t rem;
    uint16_t u;

    t0 = *(int16_t *)(p + 0x3A);
    t1 = t0;
    if (t1 < 0) {
        t1 += 7;
    }
    base = D_800A5AD8;
    off1 = *(int32_t *)(base + 8);
    P = (int16_t *)(base + off1);
    P[0] = 1;
    P[2] = -1;
    u = *(uint16_t *)(p + 0x3A);
    P[4] = 4;
    P[5] = 2;
    off0 = *(int32_t *)base;
    P[3] = 0;
    P[1] = u;
    q = t1 >> 3;
    q32 = q * 32;
    *(int16_t *)(base + off0) = (int16_t)q32;
    Q = base + off0;
    rem = t0 - q * 8;
    *(int16_t *)(Q + 2) = (int16_t)rem * 32;
    *(uint16_t *)(Q + 4) = 0x20;
    *(uint16_t *)(Q + 6) = 0x20;
    *(uint16_t *)(Q + 8) = 0;
    *(uint16_t *)(Q + 10) = 0x100;
    *(uint16_t *)(Q + 12) = 1;
    F0x8001f648(buf);
    if (*(int32_t *)(p + 0x18) != 0x10001000) {
        ((void (*)(int32_t, int32_t))*(void **)(buf + 0x94))(((*(int32_t *)p) >> 8) + 0x14, ((*(int32_t *)(p + 4)) >> 8) + 0x17);
        ((void (*)(int32_t, int32_t, int32_t))*(void **)(buf + 0x8c))(*(int16_t *)(p + 0x18), *(int16_t *)(p + 0x1a), 0x1000);
    }
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x7C))(0x100, 1);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x74))(0x140, 0x100);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x78))(0x300, 0x100);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(buf + 0x84))((int32_t)base, 0, (*(int32_t *)p) >> 8, (*(int32_t *)(p + 4)) >> 8);
}
