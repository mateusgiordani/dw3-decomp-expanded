// CARDGAME:0x80095e58 (size 348, 0x15c)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x131a8
// Framed function: prologue addiu sp,-0xb8 ; sw s0,0xb0(sp) ; move s0,a0 ;
// sw ra,0xb4(sp) ; epilogue lw ra/lw s0 ; jr ra ; addiu sp,+0xb8.
// Prev CARDGAME:0x80095e2c size 44 (0x2c) ends exactly at 0x80095e58 (gap 0);
// next CARDGAME:0x80095fb4 at +0x15c (gap 0) confirms size 0x15c contiguous.
// Evidence: PAL-byte disassembly (capstone, base 0x80082cb0) + raw-word decode;
// no Ghidra state change; cardgame.s GUIDE only.
// CORRECTION (continuation): trailing getter is *(0x8004df9c) — raw words
// 3c028005/8c42df9c give 0x80050000-0x2064 = 0x8004df9c; prior DAT_8005df9c
// was a one-digit typo (verified against PAL bytes, both EXE vectors now
// 0x8004xxxx-consistent: 0x80044f5c / 0x8004df9c).
// Caller: upstream guide jal 0x80095e58 at file-off 0x13474 (vaddr ~0x80096124,
// inside later CARDGAME dispatch region); guide is locator only, not authority.
// Callee: 1 direct jal EXE 0x8001f648(a0=sp+0x10 local table); 6 stack-table
// indirect jalr (lw v0,off(sp)+nop+jalr, MIPS-I load delay) at sp+0xa4/0x9c/
// 0x98/0x8c/0x84/0x94 (buf words 37/35/34/31/29/33 from sp+0x10); 1 EXE-vector
// jalr via *(0x80044f5c) with a0=0x025d0002; final *(s0+0x50) += (*0x8004df9c)().
// Body order: init(buf) ; f94(h54,h56+0x13) ; f8c(h58,h5a,0x1000) ;
// f88(mod/min select) ; f7c(0x100,1) ; f74(0x280,0) ; svc=DAT(0x025d0002) ;
// f84(svc,0x47,h54,h56) ; w50 += DAT_8004df9c().
// Mod/min select: byte+0x63 == 0 -> v=(w50>>2)%16 (bgez/bias/sra/sll/subu
// signed-modulo idiom); else v=(w50>>1), v=min(v,7) via slti/bne/li.
// v is deliberately block-scoped per branch with the slot call inside each
// branch: this keeps both lw v0,0x98(sp) copies and computes the else value
// directly in a0 (no move), which is what exact-matches.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base hypothesis).
#include "common/types.h"

typedef void (*cardgame_cb1_t)(int32_t a0);
typedef void (*cardgame_cb2_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_cb3_t)(int32_t a0, int32_t a1, int32_t a2);
typedef void (*cardgame_cb4_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*cardgame_svc_t)(uint32_t arg);
typedef int32_t (*cardgame_get_t)(void);

void EXE_F0x8001f648(void *buf);
extern cardgame_svc_t DAT_80044f5c;
extern cardgame_get_t DAT_8004df9c;

typedef struct {
    uint32_t w[40];
} cardgame_95e58_buf_t;

void CARDGAME_F0x80095e58(uint8_t *p)
{
    cardgame_95e58_buf_t buf;
    int32_t svc;

    if (*(int16_t *)(p + 0x58) == 0) {
        return;
    }
    if (*(int16_t *)(p + 0x5a) == 0) {
        return;
    }
    EXE_F0x8001f648(buf.w);
    {
        int32_t a0 = *(int16_t *)(p + 0x54);
        int32_t a1 = *(int16_t *)(p + 0x56) + 0x13;
        ((cardgame_cb2_t)buf.w[37])(a0, a1);
    }
    {
        int32_t a0 = *(int16_t *)(p + 0x58);
        int32_t a1 = *(int16_t *)(p + 0x5a);
        ((cardgame_cb3_t)buf.w[35])(a0, a1, 0x1000);
    }
    if (*(p + 0x63) == 0) {
        int32_t v = *(int32_t *)(p + 0x50) >> 2;
        v = v % 16;
        ((cardgame_cb1_t)buf.w[34])(v);
    } else {
        int32_t v = *(int32_t *)(p + 0x50) >> 1;
        if (v >= 7) {
            v = 7;
        }
        ((cardgame_cb1_t)buf.w[34])(v);
    }
    ((cardgame_cb2_t)buf.w[31])(0x100, 1);
    ((cardgame_cb2_t)buf.w[29])(0x280, 0);
    svc = DAT_80044f5c(0x025D0002u);
    {
        int32_t a2 = *(int16_t *)(p + 0x54);
        int32_t a3 = *(int16_t *)(p + 0x56);
        ((cardgame_cb4_t)buf.w[33])(svc, 0x47, a2, a3);
    }
    *(int32_t *)(p + 0x50) += DAT_8004df9c();
}