// CARDGAME:0x80086f24 (size 384, 0x180)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x4274
// Prologue 27bdffd0 addiu sp,-0x30 ; sw s1,0x1c(sp) ; move s1,a0 ; sw s0,0x18(sp) ; move s0,a1 ; sw s2,0x20(sp) ; move s2,a2 ; sw s3,0x24(sp) ; move s3,a3 ; sw ra,0x28(sp)
// Body: indirect EXE call (*0x80055c48)(0x4001b) ; r=(**(p2+0xed8))(p3,idx) ; (**(p2+0xf0c))(p2,idx,5,r+0x1800,0x6100 stacked) ; clear bit0 at p2+idx*76+0x150 ; sh 0 at p2+idx*76+0x12e ; idx+=step ; r=(**(p2+0xed8))(p3,idx) ; (**(p2+0xf0c))(p2,idx,1,r+0x1800,0x5c00 stacked) ; set bit0 ; sh 1
// Epilogue jr ra + 27bd0030 addiu sp,+0x30 at 0x8008709c/0x800870a0; next CARDGAME:0x800870a4 at +0x180 confirms size 0x180 contiguous (prologue/epilogue pair).
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm 96 insns verified word-equal against PAL bytes @ 0x4274 (first8 27bdffd0 afb1001c 00808821 afb00018 00a08021 afb20020 00c09021 afb30024); decompile CARDGAME_F0x80086f24; x-ref to from CARDGAME_F0x80087b80 at 0x80087cf8 (a3=-1) and 0x80087d98 (a3=+1); callers graph via 0x80087edc; callees none direct (4 indirect jalr via p2+0xed8/0xf0c plus EXE *0x80055c48).
// Callers pass (s4,s5,s6,+-1): a3 is a signed step; second ed8 call reuses live a1=new idx (Ghidra decompile shows 1 arg, disasm proves 2).
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base), portable C, no register variables.
// Status: C_MATCHING (portable C, no asm; exact_byte_match base + aspsx-2.77 alternate; cc1-2.7.2 diverges as expected).
// Matching notes: explicit int off=idx*76 temp keeps PLUS base-first (addu v0,s0,v0); final p2+=off reuses dead p2 (addu s0,s0,v0) freeing v0 for li 1.
#include <stdint.h>

typedef int32_t (*cardgame_lookup_t)(int32_t, int32_t);
typedef void (*cardgame_apply_t)(int32_t, int32_t, int32_t, int32_t, int32_t);
typedef void (*cardgame_exe_cb_t)(uint32_t);

void CARDGAME_F0x80086f24(int32_t p1, int32_t p2, int32_t p3, int32_t step)
{
    int32_t r;
    int32_t off;

    ((cardgame_exe_cb_t)*(uint32_t *)0x80055c48)(0x4001b);
    r = ((cardgame_lookup_t)*(uint32_t *)(p2 + 0xed8))(p3, *(int32_t *)(p1 + 0x43c));
    ((cardgame_apply_t)*(uint32_t *)(p2 + 0xf0c))(p2, *(int32_t *)(p1 + 0x43c), 5, r + 0x1800, 0x6100);
    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(uint8_t *)(p2 + off + 0x150) &= 0xfe;
    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(int16_t *)(p2 + off + 0x12e) = 0;
    *(int32_t *)(p1 + 0x43c) += step;
    r = ((cardgame_lookup_t)*(uint32_t *)(p2 + 0xed8))(p3, *(int32_t *)(p1 + 0x43c));
    ((cardgame_apply_t)*(uint32_t *)(p2 + 0xf0c))(p2, *(int32_t *)(p1 + 0x43c), 1, r + 0x1800, 0x5c00);
    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(uint8_t *)(p2 + off + 0x150) |= 1;
    off = *(int32_t *)(p1 + 0x43c) * 76;
    p2 += off;
    *(int16_t *)(p2 + 0x12e) = 1;
}
