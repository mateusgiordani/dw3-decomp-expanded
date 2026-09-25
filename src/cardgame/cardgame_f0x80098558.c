// CARDGAME:0x80098558 (size 780, 0x30C)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x158A8
// Frame addiu sp,-0x28; saves s0/a0(work) s1/a1(list) s2(short arg) ra.
// State = lh(work+0xe0a): 0 -> return; 2 -> o0.f84 path; 4 -> div20 path;
// 5 -> shop path; else generic ratio path. Tail jal CARDGAME_F0x8009848c
// (work, list, (short)s, 0, 0x60) with sll/sra sign-extend and sw 0x60,0x10(sp).
// Ghidra program CARDGAME read-only (disasm 195/decompile/x-ref to/from); upstream guide only.
// Caller: CARDGAME_F0x8009bba8+0x238 jal 0x80098558 (a0=work,a1=list).
// Callees: EXE 0x8002abcc; indirect *list+0x80/0x84/0x8c, list[12]+0x114/0x134/0x144,
// work+0xef8; EXE vectors *0x8004df9c, *0x80044f4c; data 0x8005cca8.
// Toolchain base: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base),
// --strip-div-guard (overlay divides carry no ASPSX zero-divisor guard);
// exact_byte_match over the full 780-byte range.
// Matching-critical shapes (evidence: submissions/cardgame-80098558/strategy-r8-o55.md):
// - Dispatch: `if (state == 0) return;` then a switch whose case 1 shares the
//   default (generic) body; gcc's case tree (root 2, left {1}) collapses to
//   PAL's beq 2 / bne <3 / beq 4 / beq 5 chain.
// - Generic ratio as a conditional expression: its label/barrier ends the CSE
//   path from the dispatch, so the case constant 2 is not reused (and kept in
//   a saved register) by the later `= 2` store; it also gives PAL's div homes.
// - Timer update as `nv = *(e00) -= tick()` (subtract in the loaded register).
// - Case 4: `q = 10 - f(...) / 512` (signed divide in v0: bgez/addiu 511/sra 9).
#include "common/types.h"

extern int32_t func_0x8002abcc(int32_t arg);
extern int32_t (*DAT_8004df9c)(void);
extern uint32_t (*DAT_80044f4c)(uint32_t);
extern uint32_t DAT_8005cca8;
extern void CARDGAME_F0x8009848c(int32_t p1, int32_t p2, int16_t p3, int32_t p4, int32_t p5);

typedef void (*cardgame_cb2_t)(int32_t a0, int32_t a1);
typedef void (*cardgame_cb3_t)(int32_t a0, int32_t a1, int32_t a2);

typedef struct {
    uint8_t pad_00[0x80];
    cardgame_cb2_t f80;
    cardgame_cb3_t f84;
    uint8_t pad_88[4];
    cardgame_cb2_t f8c;
} cardgame_8558_o0_t;

typedef struct {
    uint8_t pad_00[0x114];
    cardgame_cb3_t f114;
    uint8_t pad_118[0x1c];
    cardgame_cb3_t f134;
    uint8_t pad_138[0x0c];
    cardgame_cb2_t f144;
} cardgame_8558_oc_t;

void CARDGAME_F0x80098558(int32_t p1, int32_t *p2)
{
    cardgame_8558_o0_t *o0;
    cardgame_8558_oc_t *oc;
    void (*fn_ef8)(int32_t);
    int32_t state;
    int16_t s;
    int32_t q;
    int32_t nv;
    uint32_t u;

    state = *(int16_t *)(p1 + 0xe0a);
    if (state == 0) {
        return;
    }
    switch (state) {
    case 1:
    default:
        /* generic ratio path (states 1, 3, 6+) */
        q = ((int32_t)*(int16_t *)(p1 + 0xe00)) << 12;
        s = 0x1000 - ((*(int16_t *)(p1 + 0xe02) != 0) ? q / *(int16_t *)(p1 + 0xe02) : q);
        nv = *(int16_t *)(p1 + 0xe00) -= DAT_8004df9c();
        if (((int16_t)nv) > 0) {
            goto tail;
        }
        *(int16_t *)(p1 + 0xe0a) = 2;
        o0 = (cardgame_8558_o0_t *)p2[0];
        o0->f80((int32_t)o0, 1);
        o0 = (cardgame_8558_o0_t *)p2[0];
        o0->f84((int32_t)o0, 10, (int32_t)(*(int16_t *)(p1 + 0xe04) * 14 + 0x65));
        u = DAT_80044f4c(DAT_8005cca8 + 0xf);
        oc = (cardgame_8558_oc_t *)p2[12];
        oc->f114((int32_t)oc, u, 0x20);
        oc = (cardgame_8558_oc_t *)p2[12];
        oc->f134((int32_t)oc, 0x18, 0x65);
        goto tail;
    case 2:
        s = 0x1000;
        o0 = (cardgame_8558_o0_t *)p2[0];
        o0->f84((int32_t)o0, 10, (int32_t)(*(int16_t *)(p1 + 0xe04) * 14 + 0x65));
        goto tail;
    case 4:
        s = 0x1000;
        q = 10 - func_0x8002abcc((((int32_t)*(int16_t *)(p1 + 0xe00)) << 12) / 20) / 512;
        o0 = (cardgame_8558_o0_t *)p2[0];
        o0->f84((int32_t)o0, q, (int32_t)(*(int16_t *)(p1 + 0xe04) * 14 + 0x65));
        nv = *(int16_t *)(p1 + 0xe00) -= DAT_8004df9c();
        if (((int16_t)nv) > 0) {
            goto tail;
        }
        o0 = (cardgame_8558_o0_t *)p2[0];
        o0->f80((int32_t)o0, 0);
        fn_ef8 = *(void (**)(int32_t))(p1 + 0xef8);
        fn_ef8(p1);
        o0 = (cardgame_8558_o0_t *)p2[0];
        o0->f8c((int32_t)o0, 0x20);
        *(int16_t *)(p1 + 0xe0a) = 5;
        s = 0x1000;
        goto tail;
    case 5:
        o0 = (cardgame_8558_o0_t *)p2[0];
        o0->f80((int32_t)o0, 0);
        oc = (cardgame_8558_oc_t *)p2[12];
        oc->f144((int32_t)oc, 0);
        nv = *(int16_t *)(p1 + 0xe00) -= DAT_8004df9c();
        if (((int16_t)nv) <= 0) {
            *(int16_t *)(p1 + 0xe0a) = 0;
        }
        s = (int16_t)((((int32_t)*(int16_t *)(p1 + 0xe00)) << 12) / (int32_t)*(int16_t *)(p1 + 0xe02));
    }
tail:
    CARDGAME_F0x8009848c(p1, (int32_t)p2, s, 0, 0x60);
}
