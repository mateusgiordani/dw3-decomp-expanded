// CARDGAME:0x8009848c (size 204, 0xCC)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x157DC
// Frame addiu sp,-0xc0; saves s0/s1/s2/ra; s0=a2 (short), s1=a3, s2=stack+0xd0 (5th arg).
// Setup jal EXE 0x8001f648(buf at sp+0x10) fills callback struct; then 6 calls:
//  (buf+0x7c)(0x100,1); (buf+0x74)(0x340,0); (buf+0x94)(0,0x86);
//  (buf+0x8c)((int)p3,0x1000,0x1000) with sll/sra sign-extend; DAT_80044f5c(0x25D0003);
//  (buf+0x84)(ret,1,p4,p5). Ends jr ra/addiu sp,0xc0; next fn at 0x80098558.
// Ghidra program CARDGAME read-only (disasm 60/decompile/x-ref to/from); upstream guide only.
// Caller: CARDGAME_F0x80098558+0x2EC jal 0x8009848c with delay clear a3 (a3=0); a0=s0(work), a1=s1, a2=(short)s2, stack+0x10=0x60.
// Callees: direct EXE 0x8001f648; indirect stack slots sp+0x8c/0x84/0xa4/0x9c/0x94; EXE vector *0x80044f5c.
// Toolchain base: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0.
#include "common/types.h"

extern void func_0x8001f648(void *buf);
extern uint32_t (*DAT_80044f5c)(uint32_t);

typedef void (*cardgame_cb2_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_cb3_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*cardgame_cb4_t)(uint32_t a0, int32_t a1, int32_t a2, int32_t a3);

typedef struct {
    uint8_t pad_00[0x74];
    cardgame_cb2_t f84;
    uint8_t pad_78[4];
    cardgame_cb2_t f8c;
    uint8_t pad_80[4];
    cardgame_cb4_t f94;
    uint8_t pad_88[4];
    cardgame_cb3_t f9c;
    uint8_t pad_90[4];
    cardgame_cb2_t fa4;
    uint8_t pad_98[8];
} cardgame_848c_cb_t;

void CARDGAME_F0x8009848c(int32_t p1, int32_t p2, int16_t p3, int32_t p4, int32_t p5)
{
    cardgame_848c_cb_t cb;
    uint32_t v;

    (void)p1;
    (void)p2;
    func_0x8001f648(&cb);
    cb.f8c(0x100, 1);
    cb.f84(0x340, 0);
    cb.fa4(0, 0x86);
    cb.f9c((int32_t)p3, 0x1000, 0x1000);
    v = DAT_80044f5c(0x25D0003u);
    cb.f94(v, 1, p4, p5);
}
