// CARDGAME:0x8009d0b0 (size 144, 0x90)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1a400
// Boundary: sweep #176; prologue 27bdffe0 to epilogue jr ra + addiu sp,+0x20;
// frame -0x20/+0x20; prev CARDGAME:0x8009cfbc size 0xf4 ends exactly at
// 0x8009d0b0; next CARDGAME:0x8009d15c at +0x90; contiguous, no overlap.
// PAL words (36, MIPS LE) match upstream cardgame.s .L0x0001a400 1:1
// (27bdffe0 afb10014 00808821 ... 03e00008 27bd0020).
// Ghidra CARDGAME (project ddw3-pal-sles-03936, base 0x80082cb0, read-only):
// disasm matches PAL; decompile shows (param_1+0x24)((q)*0x10+0x140,
// (param_3+q*-8)*0x20+0x100), (param_1+0x28)(0x300,param_3+0x100),
// (param_1+0x20)(param_2) with q=(idx<0?idx+7:idx)>>3.
// Callers: 4x jal 0x8009d0b0 (0x0c02742c) at upstream 0x1a514/0x1a56c/0x1a5c4/
// 0x1a614; 0 direct callees, 3 indirect jalr via s1 table (+0x24/+0x28/+0x20).
// Semantics: q = idx / 8; r = idx % 8; f24(q*16+320, r*32+256);
// f28(768, idx+256); f20(a1). Third slot takes a single arg (a0=s2);
// its a1 is the stale t from the second call, so the C prototype is 1-arg.
// That single-arg reading is the match key: a 2-arg f20 keeps t live across
// the second jal and grows the object to 148 B; the 1-arg form reuses a1
// and yields 144 B exact.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base),
// exact_byte_match, no alternates needed.
typedef void (*cardgame_fn2_t)(int a0, int a1);
typedef void (*cardgame_fn1_t)(int a0);

typedef struct {
    char pad[0x20];
    cardgame_fn1_t f20;
    cardgame_fn2_t f24;
    cardgame_fn2_t f28;
} cardgame_ctx_t;

void CARDGAME_F0x8009d0b0(cardgame_ctx_t *ctx, int a1, int idx)
{
    int q = idx / 8;
    int r = idx % 8;

    ctx->f24(q * 16 + 320, r * 32 + 256);
    ctx->f28(768, idx + 256);
    ctx->f20(a1);
}
