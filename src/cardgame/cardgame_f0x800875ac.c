/*
 * CARDGAME:0x800875ac CARDGAME_F0x800875ac
 * 776 bytes at CARDGAME.PRO offset 0x48fc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800875ac
 *  Symbols     CARDGAME_F0x800870a4=0x800870a4 D_8004B7D0=0x8004b7d0
 *  Compare     776 bytes from 0x800875ac against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800875ac
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffc8 addiu sp,-0x38 ; sw s4,0x20(sp) ; move s4,a0 ; sw
 * ra,0x34(sp) ; sw s8..s0 (9 callee-saved + ra, frame 0x38)
 *
 * Body: off=idx*76 (x4+x1=x5,x4=x20,-x1=x19,x4=x76) base-first addu v0,s6,v0;
 * lbu/ori/sb bit0 at p2+off+0x150; sh 1 at p2+off+0x12e (s8=1); pad tablets via
 * s3=0x8004b7d0 base (lw 0x3f4=*0x8004bbc4, 0x408=*0x8004bbd8,
 * 0x3fc=*0x8004bbcc); (b0>>(b1)&1) tests for imm 0xe/0xc; EXE
 * (*0x80055c48)(0x800450bd) with lui+ori arg; mode byte p1+0x423 = 0xe/0xb/5;
 * word p1+0x440 = -1/idx; limit *(short*)(p3+10); pair tests
 * (A&(1<<B))|(C&(1<<D)) single-branch; lb test p1+idx+0x446; sb 1 at
 * p1+idx+0x46f; jal CARDGAME_F0x800870a4(p1,p2,p3,+-1 step)
 *
 * Epilogue lw ra,0x34(sp) ... ; jr ra ; addiu sp,+0x38 at
 * 0x800878ac/0x800878b0; next CARDGAME:0x800878b4 at +0x308 confirms size 0x308
 * contiguous (prologue/epilogue pair).
 *
 * The two long-lived 1s: the stores use literal 1 (CSE keeps that value in s8
 * for the sh at +0x12e and the sb at +0x46f), the shift masks use their own m =
 * 1 (s5).
 */

#include <stdint.h>

typedef int32_t (*cardgame_pad0_t)(int32_t);
typedef int32_t (*cardgame_pad1_t)(int32_t, int32_t);
typedef void (*cardgame_exe_cb_t)(uint32_t);
void CARDGAME_F0x800870a4(int32_t p1, int32_t p2, int32_t p3, int32_t step);

extern uint32_t D_8004B7D0[];

#define TBL_SLOT253()   ((cardgame_pad0_t)D_8004B7D0[253])(0)
#define TBL_SLOT255()  ((cardgame_pad0_t)D_8004B7D0[255])(0)
#define TBL_SLOT258(n)  ((cardgame_pad1_t)D_8004B7D0[258])(0, (n))

void CARDGAME_F0x800875ac(int32_t p1, int32_t p2, int32_t p3)
{
    int32_t off;
    int32_t m;

    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(uint8_t *)(p2 + off + 0x150) |= 1;
    off = *(int32_t *)(p1 + 0x43c) * 76;
    *(int16_t *)(p2 + off + 0x12e) = 1;
    if ((TBL_SLOT253() >> TBL_SLOT258(0xe)) & 1) {
        ((cardgame_exe_cb_t)*(uint32_t *)0x80055c48)(0x800450bd);
        *(int32_t *)(p1 + 0x440) = -1;
        *(int8_t *)(p1 + 0x423) = 0xe;
    }
    if ((TBL_SLOT253() >> TBL_SLOT258(0xc)) & 1)
        *(int8_t *)(p1 + 0x423) = 0xb;
    if (*(int16_t *)(p3 + 10) != 0) {
        m = 1;
        if ((TBL_SLOT255() & (m << TBL_SLOT258(7))) | (TBL_SLOT253() & (m << TBL_SLOT258(7)))) {
            if (0 < *(int32_t *)(p1 + 0x43c))
                CARDGAME_F0x800870a4(p1, p2, p3, -1);
        } else if ((TBL_SLOT255() & (m << TBL_SLOT258(5))) | (TBL_SLOT253() & (m << TBL_SLOT258(5)))) {
            if (*(int32_t *)(p1 + 0x43c) < *(int16_t *)(p3 + 10) - 1)
                CARDGAME_F0x800870a4(p1, p2, p3, 1);
        } else if (((TBL_SLOT253() >> TBL_SLOT258(0xd)) & 1) && *(int8_t *)(p1 + *(int32_t *)(p1 + 0x43c) + 0x446) != 0) {
            *(int8_t *)(p1 + 0x423) = 5;
            *(int32_t *)(p1 + 0x440) = *(int32_t *)(p1 + 0x43c);
            /* Index re-read from +0x440: CSE reuses the stored value, no copy. */
            *(int8_t *)(p1 + *(int32_t *)(p1 + 0x440) + 0x46f) = 1;
        }
    }
}
