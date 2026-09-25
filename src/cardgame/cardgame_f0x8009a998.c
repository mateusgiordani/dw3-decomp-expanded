// CARDGAME:0x8009a998 (size 788, 0x314)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x17ce8
// Framed function: prologue addiu sp,-0xc0 ; sw s0,0xb0(sp) ; move s0,a1 ;
// sw ra,0xb8(sp) ; sw s1,0xb4(sp) ; move s1,a0 (in first beq delay slot).
// Epilogue lw ra,0xb8(sp) ; lw s1,0xb4(sp) ; lw s0,0xb0(sp) ;
// jr ra ; addiu sp,+0xc0. Next CARDGAME:0x8009acac at +0x314 confirms size
// (dispatcher body_size 788).
// PAL words checked against Ghidra disasm word-for-word (first8 27bdff40
// afb000b0 00a08021 afbf00b8 afb100b4 92020045 00000000 104000b8, last4
// 8fb100b4 8fb000b0 03e00008 27bd00c0); byte table at 0x800a5b04 holds
// {00 01 02 03 00 01 02 03 00 01 02 03 02 01 00 00} (case1 window +0, case2 +4).
// Body: if byte(a1+0x45)==0 return; if byte(a1+0x47)==0 return; fill 40-word
// callback table at sp+0x10 via EXE 0x8001f648; if word(a1+0x18) != 0x10001000
// call buf.w[37]((w0>>8)+0x14,(w1>>8)+0x17) and buf.w[35]((short)w6,
// (short)(a1+0x1a),0x1000); then buf.w[31](0x100,1), buf.w[29](0x340,0),
// svc=(*0x80044f5c)(0x25d0003), and one buf.w[33] call selected by a switch
// on a fresh lbu of byte(a1+0x47) (signed int temp, like PAL v1/slti; gcc
// lowers the 4-case switch to a beq decision tree testing ==2, <3, ==1,
// ==3, ==4, which is the PAL block order):
// 1 -> D0x800a5b04[(a0->0x58>>2)&3]+0x28 then return (no counter/finalizer);
// 2 -> D0x800a5b08[(cnt>>2)%4]+0x2c, then counter+=finalizer() unless
//      counter>=8 (then byte 0x47=0, counter=0, then finalizer);
// 3 -> (cnt>>1)%9+0x3c, counter++ (old=cnt++ idiom in case-local temps so
//      the increment store sinks into the branch delay slot, like PAL;
//      if/else shape keeps a dedicated bne to the finalizer, like PAL)
//      then counter+=finalizer() unless old counter>=0x24 (then byte 0x47=0,
//      counter=0, then finalizer);
// 4 -> (cnt>>1)%7+0x46, then counter+=finalizer() unless counter>=0x1c
//      (then byte 0x47=0, counter=0, then finalizer); default returns.
// Cases 2-4 break out of the switch into a shared counter=0 store, then the
// finalizer (*0x8004df9c, an EXE data word holding a function pointer,
// lui 0x8005 + lw -0x2064 + jalr); its return value is added to
// word(a1+0x34) after the call returns.
// DAT_80044f5c/DAT_8004df9c are EXE data words holding function pointers
// (lui+lw+jalr); EXE_F0x8001f648 is a direct jal target.
// Ghidra program CARDGAME read-only (never import): disasm 0x8009a998 x200 +
// 0x8009abc8 x120 (full 197 words + epilogue), decompile CARDGAME_F0x8009a998
// (hypothesis only, confirmed vs disasm), x-ref to (single caller
// CARDGAME_F0x8009ba3c+0x58 UNCONDITIONAL_CALL, jal 0c026a66 with (s1,s0)
// passthrough), x-ref from (jal EXE 0x8001f648, DATA 0x800a5b04/0x800a5b08,
// READ 0x80044f5c/0x8004df9c, stack-slot jalr x7); no mutation.
// Caller passes (a0,a1) straight through, so two pointer params; incoming
// a2/a3 are never read.
// Idiom follows C_MATCHING siblings cardgame_f0x8009acac / cardgame_f0x8009af28
// (same EXE filler, same buf.w[29/31/33/35/37] layout, same EXE vector).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base),
// exact_byte_match via normal codegen (no __asm__, no .word, no byte arrays).
#include <stdint.h>

extern void EXE_F0x8001f648(void *buf);
extern int32_t (*DAT_80044f5c)(int32_t a0);
extern int32_t (*DAT_8004df9c)(void);
extern uint8_t D0x800a5b04[];
extern uint8_t D0x800a5b08[];

typedef void (*cardgame_cb1_t)(int32_t a0);
typedef void (*cardgame_cb2_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_cb3_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*cardgame_cb4_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);

typedef struct {
    uint32_t w[40];
} cardgame_a998_buf_t;

void CARDGAME_F0x8009a998(uint8_t *a0, uint8_t *a1) {
    cardgame_a998_buf_t buf;
    int32_t sel;
    int32_t svc;
    int32_t cnt;

    if (*(a1 + 0x45) == 0)
        return;
    if (*(a1 + 0x47) == 0)
        return;
    EXE_F0x8001f648(buf.w);
    if (*(int32_t *)(a1 + 0x18) != 0x10001000) {
        ((cardgame_cb2_t)buf.w[37])((*(int32_t *)(a1 + 0) >> 8) + 0x14, (*(int32_t *)(a1 + 4) >> 8) + 0x17);
        ((cardgame_cb3_t)buf.w[35])(*(int16_t *)(a1 + 0x18), *(int16_t *)(a1 + 0x1a), 0x1000);
    }
    ((cardgame_cb2_t)buf.w[31])(0x100, 1);
    ((cardgame_cb2_t)buf.w[29])(0x340, 0);
    sel = *(a1 + 0x47);
    switch (sel) {
    case 1:
        svc = DAT_80044f5c(0x025D0003);
        ((cardgame_cb4_t)buf.w[33])(svc, D0x800a5b04[(*(uint32_t *)(a0 + 0x58) >> 2) & 3] + 0x28, *(int32_t *)(a1 + 0) >> 8, *(int32_t *)(a1 + 4) >> 8);
        return;
    case 2:
        svc = DAT_80044f5c(0x025D0003);
        ((cardgame_cb4_t)buf.w[33])(svc, D0x800a5b08[(*(int32_t *)(a1 + 0x34) >> 2) % 4] + 0x2c, *(int32_t *)(a1 + 0) >> 8, *(int32_t *)(a1 + 4) >> 8);
        if (*(int32_t *)(a1 + 0x34) < 8)
            goto fin;
        *(a1 + 0x47) = 0;
        break;
    case 3:
        {
            int32_t c3;
            int32_t o3;
            svc = DAT_80044f5c(0x025D0003);
            ((cardgame_cb4_t)buf.w[33])(svc, (*(int32_t *)(a1 + 0x34) >> 1) % 9 + 0x3c, *(int32_t *)(a1 + 0) >> 8, *(int32_t *)(a1 + 4) >> 8);
            c3 = *(int32_t *)(a1 + 0x34);
            o3 = c3++;
            *(int32_t *)(a1 + 0x34) = c3;
            if (o3 < 0x24) {
                goto fin;
            } else {
                *(a1 + 0x47) = 0;
            }
        }
        break;
    case 4:
        svc = DAT_80044f5c(0x025D0003);
        ((cardgame_cb4_t)buf.w[33])(svc, (*(int32_t *)(a1 + 0x34) >> 1) % 7 + 0x46, *(int32_t *)(a1 + 0) >> 8, *(int32_t *)(a1 + 4) >> 8);
        if (*(int32_t *)(a1 + 0x34) < 0x1c)
            goto fin;
        *(a1 + 0x47) = 0;
        break;
    default:
        return;
    }
    *(int32_t *)(a1 + 0x34) = 0;
fin:
    cnt = DAT_8004df9c();
    *(int32_t *)(a1 + 0x34) += cnt;
}
