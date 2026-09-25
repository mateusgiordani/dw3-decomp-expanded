// CARDGAME:0x8009acac (size 636, 0x27c)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x17ffc
// Framed function: prologue addiu sp,-0xc0 ; sw s0,0xb0(sp) ; move s0,a1 ;
// sw ra,0xb8(sp) ; sw s1,0xb4(sp) ; move s1,a0 (in first beq delay slot).
// Epilogue lw ra,0xb8(sp) ; lw s1,0xb4(sp) ; lw s0,0xb0(sp) ;
// jr ra ; addiu sp,+0xc0. Next CARDGAME:0x8009af28 at +0x27c confirms size
// (symbols/function_labels.csv: CARDGAME,0x8009acac,function,636).
// PAL words checked against Ghidra disasm word-for-word (first8 27bdff40
// afb000b0 00a08021 afbf00b8 afb100b4 92020045 00000000 10400092, last4
// 8fb100b4 8fb000b0 03e00008 27bd00c0); table at 0x800a5b0c holds
// 6 bytes {00 01 02 03 02 01}.
// Body: if byte(a1+0x45)==0 return; if (byte(a1+0x48) & 6) run phase A;
// if (byte(a1+0x48) & 1) run phase B. Each phase: fill 40-word callback
// table at sp+0x10 via EXE 0x8001f648, select via buf.w[34](sp+0x98),
// optional buf.w[37]/w[35] pair when word(a1+0x18) != 0x10001000, then
// buf.w[31](0x100,1), buf.w[29](0x340,0), svc=(*0x80044f5c)(0x25d0003),
// buf.w[33](svc,8,w0>>8,w1>>8). Phase A select is (flags&4) ? table[]+5 : 4.
// Ghidra program CARDGAME read-only (never import): disasm 0x8009acac,
// decompile CARDGAME_F0x8009acac (hypothesis only, confirmed vs disasm),
// x-ref to (single caller CARDGAME_F0x8009ba3c+0x34 UNCONDITIONAL_CALL,
// jal 0c026b2b with (s1,s0) passthrough), x-ref from (jal EXE 0x8001f648 x2,
// DATA 0x800a5b0c x2, READ 0x80044f5c, stack-slot jalr x12); no mutation.
// Caller passes (a0,a1) straight through, so two pointer params; incoming
// a2/a3 are never read (first uses are li/lw overwrites).
// Idiom follows C_MATCHING siblings cardgame_f0x80095e58 / cardgame_f0x800961f0
// (same EXE filler, same buf.w[29/31/33/34/35/37] layout, same EXE vector).
// Phase-A select order is (flags&4)!=0 table-path first so the generator keeps
// the PAL block order (beq to the li-4 block at 0x8009ad44, table lookup in
// fall-through, j 0x8009ad4c with the +5 in its delay slot, single shared
// jalr via tail-merged buf.w[34] calls); the table-base lui sits in the beq
// delay slot.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base),
// exact_byte_match via normal codegen (no __asm__, no .word, no byte arrays).
#include <stdint.h>

extern void EXE_F0x8001f648(void *buf);
extern uint32_t (*DAT_80044f5c)(uint32_t);
extern uint8_t D0x800a5b0c[];

typedef void (*cardgame_cb1_t)(int32_t a0);
typedef void (*cardgame_cb2_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_cb3_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*cardgame_cb4_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);

typedef struct {
    uint32_t w[40];
} cardgame_acac_buf_t;

void CARDGAME_F0x8009acac(uint8_t *a0, uint8_t *a1) {
    cardgame_acac_buf_t buf;
    uint32_t svc;

    if (*(a1 + 0x45) == 0)
        return;
    if ((*(a1 + 0x48) & 6) != 0) {
        EXE_F0x8001f648(buf.w);
        if ((*(a1 + 0x48) & 4) != 0)
            ((cardgame_cb1_t)buf.w[34])(D0x800a5b0c[(*(uint32_t *)(a0 + 0x58) >> 2) % 6] + 5);
        else
            ((cardgame_cb1_t)buf.w[34])(4);
        if (*(int32_t *)(a1 + 0x18) != 0x10001000) {
            ((cardgame_cb2_t)buf.w[37])((*(int32_t *)(a1 + 0) >> 8) + 0x14, (*(int32_t *)(a1 + 4) >> 8) + 0x17);
            ((cardgame_cb3_t)buf.w[35])(*(int16_t *)(a1 + 0x18), *(int16_t *)(a1 + 0x1a), 0x1000);
        }
        ((cardgame_cb2_t)buf.w[31])(0x100, 1);
        ((cardgame_cb2_t)buf.w[29])(0x340, 0);
        svc = DAT_80044f5c(0x025D0003u);
        ((cardgame_cb4_t)buf.w[33])(svc, 8, *(int32_t *)(a1 + 0) >> 8, *(int32_t *)(a1 + 4) >> 8);
    }
    if ((*(a1 + 0x48) & 1) != 0) {
        EXE_F0x8001f648(buf.w);
        ((cardgame_cb1_t)buf.w[34])(D0x800a5b0c[(*(uint32_t *)(a0 + 0x58) >> 2) % 6]);
        if (*(int32_t *)(a1 + 0x18) != 0x10001000) {
            ((cardgame_cb2_t)buf.w[37])((*(int32_t *)(a1 + 0) >> 8) + 0x14, (*(int32_t *)(a1 + 4) >> 8) + 0x17);
            ((cardgame_cb3_t)buf.w[35])(*(int16_t *)(a1 + 0x18), *(int16_t *)(a1 + 0x1a), 0x1000);
        }
        ((cardgame_cb2_t)buf.w[31])(0x100, 1);
        ((cardgame_cb2_t)buf.w[29])(0x340, 0);
        svc = DAT_80044f5c(0x025D0003u);
        ((cardgame_cb4_t)buf.w[33])(svc, 8, *(int32_t *)(a1 + 0) >> 8, *(int32_t *)(a1 + 4) >> 8);
    }
}
