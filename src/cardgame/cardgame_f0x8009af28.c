// CARDGAME:0x8009af28 (size 256, 0x100)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x18278
// Framed function from boundary sweep (symbols/function_labels.csv):
// prologue 27bdff48 addiu sp,-0xb8, sw s0,0xb0(sp), move s0,a1, sw ra,0xb4(sp);
// epilogue lw ra,0xb4(sp), lw s0,0xb0(sp), jr ra, addiu sp,+0xb8.
// Ghidra CARDGAME (ddw3-pal-sles-03936, read-only): disasm 64 insns at
// 0x8009af28, decompile CARDGAME_F0x8009af28(param_1,param_2): early return
// when byte at param_2+0x45 or +0x46 is zero; func_0x8001f648(stack buf at
// sp+0x10); if word at param_2+0x18 != 0x10001000, two calls via table slots
// sp+0xa4/sp+0x9c; then calls via sp+0x8c (0x100,1), sp+0x84 (0x280,0),
// r = (*0x80044f5c)(0x25d0002), final 4-arg call via sp+0x94.
// Xref: 1 caller CARDGAME_F0x8009ba3c via 0x8009ba88 jal; 1 direct callee
// EXE 0x8001f648 plus 6 computed jalr (5 stack-table slots + EXE data word).
// cardgame.s is GUIDE only, never copied as source. No Ghidra state change.
// Stack table family matches FIGHTSTG_F0x80094878 portable pattern:
// F0x8001f648(buf) fills uint8_t buf[0xa0] at sp+0x10, fn slots at
// buf+0x74/0x7c/0x84/0x8c/0x94 (sp+0x84/0x8c/0x94/0x9c/0xa4).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
// Status: C_MATCHING (portable C, no asm, exact_byte_match base O2, no alternates needed).
#include <stdint.h>

extern int32_t (*D_80044f5c)(int32_t a0);
extern void F0x8001f648(void *a0);

void CARDGAME_F0x8009af28(int32_t p1, int32_t p2)
{
    uint8_t buf[0xa0];
    int32_t r;

    (void)p1;
    if (*(uint8_t *)(p2 + 0x45) == 0) {
        return;
    }
    if (*(uint8_t *)(p2 + 0x46) == 0) {
        return;
    }
    F0x8001f648(buf);
    if (*(int32_t *)(p2 + 0x18) != 0x10001000) {
        ((void (*)(int32_t, int32_t))*(void **)(buf + 0x94))((*(int32_t *)p2 >> 8) + 0x14, (*(int32_t *)(p2 + 4) >> 8) + 0x17);
        ((void (*)(int32_t, int32_t, int32_t))*(void **)(buf + 0x8c))(*(int16_t *)(p2 + 0x18), *(int16_t *)(p2 + 0x1a), 0x1000);
    }
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x7c))(0x100, 1);
    ((void (*)(int32_t, int32_t))*(void **)(buf + 0x74))(0x280, 0);
    r = D_80044f5c(0x25d0002);
    ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(buf + 0x84))(r, *(uint8_t *)(p2 + 0x46) + 0x33, (*(int32_t *)p2 >> 8) + 3, (*(int32_t *)(p2 + 4) >> 8) + 2);
}
