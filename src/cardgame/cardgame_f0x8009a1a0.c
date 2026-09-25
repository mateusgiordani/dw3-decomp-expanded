// CARDGAME:0x8009a1a0 (size 692, 0x2b4)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x174f0
// Framed function: prologue addiu sp,-0x20 ; sw s1,0x14(sp) ; move s1,a1 ;
// sw ra,0x1c(sp) ; sw s2,0x18(sp) ; sw s0,0x10(sp). Epilogue lw ra,0x1c(sp) ;
// lw s2,0x18(sp) ; lw s1,0x14(sp) ; lw s0,0x10(sp) ; jr ra ; addiu sp,+0x20.
// Next CARDGAME:0x8009a454 at +0x2b4 (contiguous), size 692 self-contained
// (symbols/function_labels.csv: CARDGAME,0x8009a1a0,function,692).
// PAL words checked against Ghidra disasm word-for-word (first16 27bdffe0
// afb10014 00a08821 afbf001c afb20018 afb00010 8e220030 00000000 18400028
// 00009021 3c028005 8c42df9c 00000000 0040f809 24101c00 8e230030, tail
// a6250022 02401021 8fbf001c 8fb20018 8fb10014 8fb00010 03e00008 27bd0020).
// Ghidra program CARDGAME (project ddw3-pal-sles-03936, base 0x80082cb0)
// read-only, no mutation: disasm 0x8009a1a0 (64+80+40 insns), decompile
// CARDGAME_F0x8009a1a0 (hypothesis only, confirmed vs disasm), x-ref to
// (single caller CARDGAME_F0x8009b890+0xa0 UNCONDITIONAL_CALL, jal 0c026868
// with a0=s0 a1=s1, v0=flags|1 stored to 0x54(s0) in the delay slot),
// x-ref from (jalr via pointer at 0x8004df9c x2, jalr via pointer at
// 0x80055c48 x1, jal EXE 0x8002abcc x1, intra-function branches only).
// Incoming a0 is never read (first a0 use is lui overwrite at 0x8009a21c);
// s1=a1 is the context pointer; s0 is the constant 0x1c00 (li in the first
// jalr delay slot); s2 is the 0/1 return flag. Upstream ddw3 guide not
// checked out in this worktree (submodule uninitialized); PAL bytes are
// authoritative per source hierarchy.
// Semantic: two-counter timed interpolation updater over context fields
// +0x00..+0x30 (lerp of int pairs with divisor +0x2c, short pairs with
// signed lh quotient but lhu store base), byte state +0x42/+0x48.
// Returns 1 only on the swap/reset path (counter +0x28 decremented to <=0),
// else 0.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base,
// --strip-div-guard): exact_byte_match 692/692 via normal codegen
// (no __asm__, no .word).
#include <stdint.h>

extern int32_t (*DAT_8004df9c)(void);
extern void (*DAT_80055c48)(uint32_t arg);
extern int32_t EXE_F0x8002abcc(int32_t arg);

typedef struct {
    int32_t w00;      /* +0x00 */
    int32_t w04;      /* +0x04 */
    int32_t w08;      /* +0x08 */
    int32_t w0c;      /* +0x0c */
    int32_t w10;      /* +0x10 */
    int32_t w14;      /* +0x14 */
    int16_t h18;      /* +0x18 */
    int16_t h1a;      /* +0x1a */
    int16_t h1c;      /* +0x1c */
    int16_t h1e;      /* +0x1e */
    int16_t h20;      /* +0x20 */
    int16_t h22;      /* +0x22 */
    uint8_t pad24[4]; /* +0x24 unknown */
    int32_t w28;      /* +0x28 countdown A */
    int32_t w2c;      /* +0x2c divisor */
    int32_t w30;      /* +0x30 countdown B */
    uint8_t pad34[14];/* +0x34 unknown */
    uint8_t b42;      /* +0x42 state */
    uint8_t pad43[5]; /* +0x43 unknown */
    uint8_t b48;      /* +0x48 flags */
} cardgame_a1a0_ctx_t;

int32_t CARDGAME_F0x8009a1a0(void *unused_a0, cardgame_a1a0_ctx_t *ctx) {
    int32_t t;
    int32_t t2;
    int32_t dy;
    int32_t adj;
    int32_t x0;
    int32_t y0;
    int32_t x1;
    int32_t y1;
    int32_t dh0;
    int32_t dh1;
    int16_t h0;
    int16_t h1;
    int32_t ret;

    (void)unused_a0;
    ret = 0;
    if (ctx->w30 > 0) {
        /* Compound update: the second 0x1c00 - w30 * 192 is recomputed,
           not CSE'd, as in PAL. */
        ctx->w30 -= DAT_8004df9c();
        ctx->h18 = 0x1c00 - ctx->w30 * 192;
        ctx->h1a = 0x1c00 - ctx->w30 * 192;
        if (ctx->w30 <= 0) {
            ctx->b48 |= 5;
            DAT_80055c48(0x8004603cu);
            ctx->h1c = 0x1200;
            ctx->h1e = 0x1200;
            ctx->h18 = 0x1c00;
            ctx->h20 = 0x1c00;
            ctx->h1a = 0x1c00;
            ctx->h22 = 0x1c00;
            ctx->w30 = 0;
        }
        return 0;
    } else {
        t = ctx->w28 - DAT_8004df9c();
        ctx->w28 = t;
        if (t > 0) {
            /* The store to +0x00 kills CSE's memory equivalences: w0c and w2c
               are reloaded and the ratio reads w28 from memory, as in PAL. */
            dy = ctx->w0c - ctx->w14;
            ctx->w00 = ctx->w08 - ((ctx->w08 - ctx->w10) * t) / ctx->w2c;
            ctx->w04 = ctx->w0c - (dy * t) / ctx->w2c;
            adj = EXE_F0x8002abcc((ctx->w28 << 12) / (ctx->w2c * 2)) * 0x1c00;
            ctx->w04 += ((dy > 0) ? -adj : adj) / 4096;
            /* Separate local: reusing t would make it a multi-set global. */
            t2 = ctx->w28;
            /* Divisor loaded into the quotient local first (a direct
               ctx->w2c divisor changes allocation: control k1). */
            dh0 = ctx->w2c;
            dh0 = ((ctx->h1c - ctx->h20) * t2) / dh0;
            dh1 = ctx->w2c;
            dh1 = ((ctx->h1e - ctx->h22) * t2) / dh1;
            ctx->h18 = (uint16_t)ctx->h1c - dh0;
            ctx->h1a = (uint16_t)ctx->h1e - dh1;
        } else {
            x1 = ctx->w10;
            y1 = ctx->w14;
            x0 = ctx->w08;
            y0 = ctx->w0c;
            h0 = ctx->h1c;
            h1 = ctx->h1e;
            ctx->b42 = 2;
            ctx->h1c = 0x1000;
            ctx->h1e = 0x1000;
            ret = 1;
            ctx->w10 = x0;
            ctx->w00 = x0;
            ctx->w14 = y0;
            ctx->w04 = y0;
            ctx->w08 = x1;
            ctx->w0c = y1;
            ctx->h18 = h0;
            ctx->h1a = h1;
            ctx->b48 &= 0xfa;
            ctx->w28 = ctx->w2c = ctx->w2c * 2 + ctx->w2c / 2;
            /* Re-read through the just-stored fields (PAL copies the values). */
            ctx->h20 = ctx->h18;
            ctx->h22 = ctx->h1a;
        }
    }
    return ret;
}
