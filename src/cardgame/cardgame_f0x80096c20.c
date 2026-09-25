// CARDGAME:0x80096c20 (size 208, 0xD0)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x13f70
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #113
// Prologue 27bdff40 addiu sp,-0xc0 ; epilogue jr ra / 27bd00c0 addiu sp,+0xc0
// Next CARDGAME:0x80096cf0 at +0xd0 (sweep #114); ends at 0x80096cf0.
// Caller: CARDGAME_F0x80096cf0 @0x80096dfc (jal a0=s2/a1=s3/a2=s4/a3=s0, guide .L0x0001414c).
// Callee: EXE F0x8001f648 (C_MATCHING, decomp/src/exe/exe_f0x8001f648.c) fills a
//   0xa0-byte callback struct at sp+0x10; this function then issues 6 indirect
//   calls through its slots (+0x74/+0x7c/+0x84/+0x8c/+0x94 struct-relative).
// Data: CARDGAME table at 0x800a5978 (halfword pairs indexed by a2*4);
//   EXE indirect slot at 0x80044f5c (lui 0x8004 / lw 0x4f5c).
// Ghidra CARDGAME disasm/decompile/xrefs read-only (project ddw3-pal-sles-03936,
// base 0x80082cb0, no import/mutation); cardgame.s GUIDE only.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
#include <stdint.h>

extern void F0x8001f648(void *buf);
extern int16_t D0x800a5978[];
extern void *D0x80044f5c;

typedef void (*CardCb2)(int32_t a0, int32_t a1);
typedef void (*CardCb3)(int32_t a0, int32_t a1, int32_t a2);
typedef int32_t (*CardCbR)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef int32_t (*ExeSlotFn)(int32_t a0);

typedef struct {
    uint8_t pad[0x74];
    CardCb2 cb74;
    CardCb2 cb78;
    CardCb2 cb7c;
    CardCb2 cb80;
    CardCbR cb84;
    CardCb2 cb88;
    CardCb3 cb8c;
    CardCb2 cb90;
    CardCb2 cb94;
    CardCb2 cb98;
    CardCb2 cb9c;
} CardCtx96c20;

void CARDGAME_F0x80096c20(int32_t a0, int32_t a1, int32_t a2, int16_t *a3)
{
    CardCtx96c20 ctx;
    int16_t *base;
    int16_t *tbl;
    int32_t r;

    (void)a0;
    (void)a1;
    F0x8001f648(&ctx);
    ctx.cb7c(0x100, 1);
    ctx.cb74(0x340, 0);
    base = D0x800a5978;
    tbl = base + a2 * 2;
    ctx.cb94(tbl[0] + 4, tbl[1] + 0x17);
    ctx.cb8c(0x1000, a3[1], 0x1000);
    r = ((ExeSlotFn)D0x80044f5c)(0x25d0003);
    ctx.cb84(r, a3[5] + 6, tbl[0], tbl[1]);
}
