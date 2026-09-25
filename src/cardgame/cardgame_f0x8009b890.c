// CARDGAME:0x8009b890 (size 428, 0x1ac)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x18be0
// Framed function: prologue 27bdffe0 addiu sp,-0x20 ; sw s1,0x14(sp) ;
// move s1,a2 ; sw ra,0x18(sp) ; sw s0,0x10(sp) ; entry lbu v1,0x42(s1) ;
// sltiu v0,v1,0xc ; beq v0,zero -> epilogue (byte >= 12 returns) ; jump
// table at 0x80083748 (12 words: 0->0x8009b8d8, 1->0x8009ba28 epilogue,
// 2,3->0x8009b8e0, 4->0x8009b900, 5->0x8009b920, 6->0x8009b94c,
// 7->0x8009b96c, 8->0x8009b98c, 9->0x8009b9f4, 10->0x8009ba04,
// 11->0x8009b9ac) ; case-11 code precedes case-9/10 code in PAL layout
// (0x8009b9ac < 0x8009b9f4), so source order is 8, 11, 9, 10 ; epilogue
// lw ra,0x18(sp) ; lw s1,0x14(sp) ;
// lw s0,0x10(sp) ; jr ra ; addiu sp,+0x20. All 107 words match PAL
// word-for-word. Next function CARDGAME:0x8009ba3c at +0x1ac, prev
// CARDGAME:0x8009b76c ends at 0x8009b890: size 0x1ac contiguous
// (prologue/prologue and end/prologue pairs).
// Ghidra program CARDGAME read-only (project ddw3-pal-sles-03936, base
// 0x80082cb0, min_address 0x80082cb0, MIPS LE 32): disasm 107 insns,
// decompile is a switch on record byte 0x42 with shared OR-store tails
// (cases 5/9/10 converge on the 0x8009ba24 store) ; x-ref to 0x8009b890:
// caller 0x8009bac8 (CARDGAME_F0x8009bac8) UNCONDITIONAL_CALL at 0x8009bb00,
// 39-record loop stride 0x4c over the card record (a0=base, a1=pass,
// a2=record) ; callees 0x80099ffc, 0x8009a75c, 0x8009a1a0, 0x8009a62c,
// 0x8009a454, 0x8009a540, 0x8009a5b0 plus EXE data-word indirect
// 0x8004df9c (lui 0x8005 + lw -0x2064, jalr; EXE view: data, no
// instruction). Upstream cardgame.s GUIDE only (no F0x8009b890 label,
// single jal site .L0x00018e50), never copied.
// Source idiom follows matching sibling CARDGAME:0x8009b76c (same
// caller loop, int32_t-address record access, combined entry guard):
// a1 unused, cases 2-8 pre-call *(a0+0x54) |= 1, cases 9/10 call
// F0x8009a5b0 first with a2 0x24/0x1c then store |= 1, case 5 early
// return on zero with shared |= 2 tail, case 11 indirect-add with
// signed slti 11 compare setting byte 0x42 = 1.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base).
// r6 exact: case 11 uses *(a2+0x2c) += D_8004df9c() plus reload into v
// (not v = D(); v = mem+v; store): the call result stays in v0 while the
// sum lives in v1 (addu v1,v1,v0), fixing the last 5 bytes. Full 428B body
// plus 48B rodata exact_byte_match, 0 diffs.
#include <stdint.h>

extern void F0x80099ffc(int32_t a0, int32_t a1);
extern void F0x8009a75c(int32_t a0, int32_t a1);
extern int32_t F0x8009a1a0(int32_t a0, int32_t a1);
extern void F0x8009a62c(int32_t a0, int32_t a1);
extern void F0x8009a454(int32_t a0, int32_t a1);
extern void F0x8009a540(int32_t a0, int32_t a1);
extern void F0x8009a5b0(int32_t a0, int32_t a1, int32_t a2);
extern int32_t (*D_8004df9c)(void);

void CARDGAME_F0x8009b890(int32_t a0, int32_t a1, int32_t a2)
{
    uint8_t b;
    int32_t v;
    int32_t u;

    (void)a1;
    b = *(uint8_t *)(a2 + 0x42);
    if (b >= 12) {
        return;
    }
    switch (b) {
    case 0:
        *(uint8_t *)(a2 + 0x45) = 0;
        break;
    case 2:
    case 3:
        *(int32_t *)(a0 + 0x54) |= 1;
        F0x80099ffc(a0, a2);
        break;
    case 4:
        *(int32_t *)(a0 + 0x54) |= 1;
        F0x8009a75c(a0, a2);
        break;
    case 5:
        *(int32_t *)(a0 + 0x54) |= 1;
        if (F0x8009a1a0(a0, a2) == 0) {
            return;
        }
        u = *(int32_t *)(a0 + 0x54) | 2;
        goto st;
    case 6:
        *(int32_t *)(a0 + 0x54) |= 1;
        F0x8009a62c(a0, a2);
        break;
    case 7:
        *(int32_t *)(a0 + 0x54) |= 1;
        F0x8009a454(a0, a2);
        break;
    case 8:
        *(int32_t *)(a0 + 0x54) |= 1;
        F0x8009a540(a0, a2);
        break;
    case 11:
        *(int32_t *)(a0 + 0x54) |= 1;
        *(int32_t *)(a2 + 0x2c) += D_8004df9c();
        v = *(int32_t *)(a2 + 0x2c);
        if (v > 10) {
            *(uint8_t *)(a2 + 0x42) = 1;
        }
        break;
    case 9:
        F0x8009a5b0(a0, a2, 0x24);
        u = *(int32_t *)(a0 + 0x54) | 1;
        goto st;
    case 10:
        F0x8009a5b0(a0, a2, 0x1c);
        u = *(int32_t *)(a0 + 0x54) | 1;
    st:
        *(int32_t *)(a0 + 0x54) = u;
        break;
    }
}
