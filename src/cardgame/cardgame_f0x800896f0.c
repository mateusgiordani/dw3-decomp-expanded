/*
 * CARDGAME:0x800896f0 CARDGAME_F0x800896f0
 * 364 bytes at CARDGAME.PRO offset 0x6a40 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float
 *  Variant     base
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x800896f0
 *  Symbols     CARDGAME_F0x80085fd0=0x80085fd0 CARDGAME_F0x80089974=0x80089974
 *              D0x800a58d4=0x800a58d4 DAT_80055c48=0x80055c48
 *              D_80055c48=0x80055c48
 *  Compare     364 bytes from 0x800896f0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800896f0
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffd8 addiu sp,-0x28 ; sw s0,0x18(sp) ; move s0,a0 ; sw
 * s1,0x1c(sp) ; move s1,a1 ; sw ra,0x24(sp) (+ sw s2 in jalr delay slot).
 *
 * Epilogue lw ra,0x24(sp) ; lw s2,0x20(sp) ; lw s1,0x1c(sp) ; lw s0,0x18(sp) ;
 * jr ra + addiu sp,+0x28 at 0x80089844-0x80089858.
 *
 * Next function CARDGAME:0x8008985c at +0x16c confirms size 0x16c; prev
 * CARDGAME:0x80089580 size 368 ends at 0x800896f0 exactly.
 *
 * C below written from disassembly). x-ref to 0x800896f0: 1 caller,
 * CARDGAME_F0x80089974 @ 0x80089bcc (jal 0x800896f0, delay move a1,s5; guarded
 * by state machine case 2). x-ref from: READ 0x80055c48 (EXE-resident vector),
 * DATA 0x800a58d4 (CARDGAME pair table), stack/outgoing-arg writes only.
 *
 * Table CARDGAME 0x800a58d4: PAL words {0x7400,0x6100},{0xa400,0x6100},...
 * (8-byte entries; second call passes w1-0x500, i.e. 0x5c00, matching the
 * 0x7400/0x5c00 pair also issued by caller case 1).
 *
 * Slot stride 0x4c (76): sll2/addu/sll2/subu/sll2 strength reduction.
 *
 * Signatures conservative: widths from load/store widths (lw/lbu/sb/sh);
 * offsets raw, no invented structs for st/ctx. EXE vector idiom shared with
 * siblings 0x8009c054/0x8009c0dc/0x8009c124/0x8009c174/0x8009c1cc/0x8009c214.
 *
 * Attempt 2 (this file): inline indexed pair accesses so CSE keeps the table
 * address in v0 for both word loads, plus explicit int off temp so the ctx+off
 * addition keeps base-first order (same idiom as the sibling
 * CARDGAME_F0x80085fd0 off=idx<<1 fix).
 */

#include <stdint.h>

extern void (*D_80055c48)(uint32_t);

typedef struct {
    uint32_t w0;
    uint32_t w1;
} CardPair896f0;

extern CardPair896f0 D0x800a58d4[];

typedef void (*CardSlotFn896f0)(void *ctx, int32_t idx, int32_t kind, uint32_t w0, uint32_t w1);

void CARDGAME_F0x800896f0(void *st, void *ctx) {
    int32_t idx;
    int32_t off;

    D_80055c48(0x4001b);
    (*(CardSlotFn896f0 *)((char *)ctx + 0xf0c))(
        ctx, *(int32_t *)((char *)st + 0x43c), 5,
        (D0x800a58d4 + *(int32_t *)((char *)st + 0x43c))->w0,
        (D0x800a58d4 + *(int32_t *)((char *)st + 0x43c))->w1);
    off = *(int32_t *)((char *)st + 0x43c) * 76;
    *(uint8_t *)((char *)ctx + off + 0x150) &= 0xfe;
    off = *(int32_t *)((char *)st + 0x43c) * 76;
    *(int16_t *)((char *)ctx + off + 0x12e) = 0;
    idx = *(int32_t *)((char *)st + 0x43c) ^ 1;
    *(int32_t *)((char *)st + 0x43c) = idx;
    (*(CardSlotFn896f0 *)((char *)ctx + 0xf0c))(
        ctx, *(int32_t *)((char *)st + 0x43c), 1,
        (D0x800a58d4 + idx)->w0, (D0x800a58d4 + idx)->w1 - 0x500);
    off = *(int32_t *)((char *)st + 0x43c) * 76;
    *(uint8_t *)((char *)ctx + off + 0x150) |= 1;
    off = *(int32_t *)((char *)st + 0x43c) * 76;
    *(int16_t *)((char *)ctx + off + 0x12e) = 1;
}
