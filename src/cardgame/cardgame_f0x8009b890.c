/*
 * CARDGAME:0x8009b890 CARDGAME_F0x8009b890
 * 428 bytes at CARDGAME.PRO offset 0x18be0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009b890, jump table (.rodata) at 0x80083748
 *  Symbols     CARDGAME_F0x8009b890=0x8009b890 D_8004df9c=0x8004df9c
 *              F0x80099ffc=0x80099ffc F0x8009a1a0=0x8009a1a0
 *              F0x8009a454=0x8009a454 F0x8009a540=0x8009a540
 *              F0x8009a5b0=0x8009a5b0 F0x8009a62c=0x8009a62c
 *              F0x8009a75c=0x8009a75c
 *  Compare     428 bytes from 0x8009b890 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009b890
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue 27bdffe0 addiu sp,-0x20 ; sw s1,0x14(sp) ; move
 * s1,a2 ; sw ra,0x18(sp) ; sw s0,0x10(sp) ; entry lbu v1,0x42(s1) ; sltiu
 * v0,v1,0xc ; beq v0,zero -> epilogue (byte >= 12 returns) ; jump table at
 * 0x80083748 (12 words: 0->0x8009b8d8, 1->0x8009ba28 epilogue, 2,3->0x8009b8e0,
 * 4->0x8009b900, 5->0x8009b920, 6->0x8009b94c, 7->0x8009b96c, 8->0x8009b98c,
 * 9->0x8009b9f4, 10->0x8009ba04, 11->0x8009b9ac) ; case-11 code precedes
 * case-9/10 code in PAL layout (0x8009b9ac < 0x8009b9f4), so source order is 8,
 * 11, 9, 10 ; epilogue lw ra,0x18(sp) ; lw s1,0x14(sp) ; lw s0,0x10(sp) ; jr ra
 * ; addiu sp,+0x20. All 107 words match PAL word-for-word. Next function
 * CARDGAME:0x8009ba3c at +0x1ac, prev CARDGAME:0x8009b76c ends at 0x8009b890:
 * size 0x1ac contiguous (prologue/prologue and end/prologue pairs).
 *
 * EXE view: data, no instruction).
 *
 * Source idiom follows matching sibling CARDGAME:0x8009b76c (same caller loop,
 * int32_t-address record access, combined entry guard): a1 unused, cases 2-8
 * pre-call *(a0+0x54) |= 1, cases 9/10 call F0x8009a5b0 first with a2 0x24/0x1c
 * then store |= 1, case 5 early return on zero with shared |= 2 tail, case 11
 * indirect-add with signed slti 11 compare setting byte 0x42 = 1.
 *
 * r6 exact: case 11 uses *(a2+0x2c) += D_8004df9c() plus reload into v (not v =
 * D(); v = mem+v; store): the call result stays in v0 while the sum lives in v1
 * (addu v1,v1,v0), fixing the last 5 bytes.
 */

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
