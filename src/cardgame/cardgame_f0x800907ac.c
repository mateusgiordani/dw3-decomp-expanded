/*
 * CARDGAME:0x800907ac CARDGAME_F0x800907ac
 * 704 bytes at CARDGAME.PRO offset 0xdafc (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x800907ac
 *  Symbols     DAT_8004DE10=0x8004de10
 *  Compare     704 bytes from 0x800907ac against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x800907ac
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue: addiu sp,sp,-0x28 ; sw s1,0x14(sp) ; move s1,a0 ; sw s3,0x1c(sp) ;
 * move s3,a1 ; sw s2,0x18(sp) ; move s2,a2 ; sw s4,0x20(sp) ; sw ra,0x24(sp) ;
 * sw s0,0x10(sp).
 *
 * Epilogue at 0x80090a48: lw ra,0x24(sp) ; lw s4,0x20(sp) ; lw s3,0x1c(sp) ; lw
 * s2,0x18(sp) ; lw s1,0x14(sp) ; lw s0,0x10(sp) ; jr ra ; addiu sp,sp,0x28.
 *
 * Next function CARDGAME:0x80090a6c at +0x2C0 confirms the 704B boundary.
 *
 * Caller: CARDGAME_F0x80084320 at 0x80085614 UNCONDITIONAL_CALL.
 *
 * Callees (all indirect jalr): 2x via word at 0x8004df9c (lui s0,0x8005 / addiu
 * s0,-0x21f0 / lw v0,0x18c(s0)); 2x via word at p2+0xea0 with (a0=p2, a1=p3,
 * a2=7 / a2=5); 1x via word at p1+0x810 with (a0=p1, a1/a2=h4/h8).
 *
 * Ghidra byte stream (d8ffbd27 1400b1af ...) matches PAL little-endian words.
 */

#include "common/types.h"

typedef struct { int8_t b[2]; } cardgame_pair_t;
typedef int32_t (*cardgame_get_t)(void);
typedef void (*cardgame_cback_t)(int32_t a0, int32_t a1, int32_t a2, int32_t a3);
typedef void (*cardgame_stat_t)(int32_t a0, int32_t a1, int32_t a2);

/* Positional EXE-side word table: PAL words lui s0,0x8005 / addiu s0,-0x21f0 give
   base 0x8004DE10, and both getter calls use lw v0,0x18c(s0) (slot index 0x63).
   The base address is link-time positional only, not a promoted symbol. */
extern uint32_t DAT_8004DE10[];

int32_t CARDGAME_F0x800907ac(int32_t p1, int32_t p2, int32_t p3)
{
    int32_t ret = 0;
    uint8_t mode = *(uint8_t *)(p1 + 0x422);

    /* B1 (r9, dispatch-shape): PAL tests mode with two forward beqs to
       out-of-line bodies plus a default jump to the restore (0x800907dc+
       0x800907e4/0x800907e8/0x800907f0), the compare-chain layout of a
       two-case switch, instead of a bne with the mode-1 body inline. */
    switch (mode) {
    case 1: {
        uint32_t *slot = DAT_8004DE10;
        int32_t count;
        *(int32_t *)(p1 + 0x424) += ((cardgame_get_t)slot[0x63])();
        count = *(int32_t *)(p1 + 0x434) + ((cardgame_get_t)slot[0x63])();
        *(int32_t *)(p1 + 0x434) = count;
        if (count > 6) {
            int32_t flag = 0;
            if (*(int32_t *)(p1 + 0x428) > 0) {
                flag = 1;
                *(int32_t *)(p1 + 0x428) -= 1;
                *(int32_t *)(p1 + 0x42c) += flag;
            }
            if (!flag) {
                *(uint8_t *)(p1 + 0x422) = 2;
            }
            ((cardgame_cback_t)*(uint32_t *)(p2 + 0xea0))(p2, p3, 7, *(int32_t *)(p1 + 0x428));
            ((cardgame_cback_t)*(uint32_t *)(p2 + 0xea0))(p2, p3, 5, *(int32_t *)(p1 + 0x42c));
            *(int32_t *)(p1 + 0x434) -= 7;
        }
        /* FAMILY-1 (iter8): no redefinition of ret here; the top ret=0 stays
           live across the four indirect calls above, forcing the compiler to
           hold the return value in a callee-saved register (PAL uses s4,
           zeroed in the first branch delay slot, moved to v0 at return). */
        break;
    }
    case 2: {
        uint8_t *t0 = (uint8_t *)p1 + (p3 * 200 + 0x59c);
        uint16_t *dst_base = (uint16_t *)(t0 + 0x14);
        uint16_t *src_base = (uint16_t *)(t0 + 0x78);
        /* One counter and one index for the whole case: PAL keeps the counter
           in $7 in all three loops and the index in $6 in both branches;
           the extra sets also drop the sched1 single-set boost that would
           otherwise move the [+6] load ahead of [+4]. */
        int32_t a3;
        int32_t a2;
        if (p3 == 0) {
            int32_t x;
            int32_t y;
            x = *(int16_t *)(t0 + 4);
            y = *(int16_t *)(t0 + 6);
            a2 = x - 1;
            a3 = y - 1;
            if (a3 >= 0) {
                uint16_t *src = (uint16_t *)(a3 * 2 + (int32_t)src_base);
                uint16_t *dst = (uint16_t *)(a2 * 2 + (int32_t)dst_base);
                do {
                    *dst = *src;
                    src--;
                    a3--;
                    dst--;
                    *(int16_t *)(t0 + 4) -= 1;
                    *(int16_t *)(t0 + 8) += 1;
                } while (a3 >= 0);
            }
            *(int16_t *)(t0 + 6) = 0;
            ((cardgame_stat_t)*(uint32_t *)(p1 + 0x810))(p1, *(int16_t *)(t0 + 4), *(int16_t *)(t0 + 8));
        } else {
            a3 = *(int16_t *)(t0 + 6) - 1;
            if (a3 >= 0) {
                do {
                    *(int16_t *)(t0 + 4) -= 1;
                    *(int16_t *)(t0 + 8) += 1;
                    {
                        a2 = *(int16_t *)(t0 + 4);
                        /* n is copied back into a2 (addiu v1 / move a2,v1 in PAL):
                           the copy keeps loop.c from treating a2 as a biv, so no
                           givs are created for this loop. */
                        while (a2 < 40) {
                            int32_t a1 = a2 * 2;
                            uint16_t *d = (uint16_t *)(a1 + (int32_t)dst_base);
                            int32_t n = a2 + 1;
                            int32_t b;
                            *d = *(d + 1);
                            b = n * 2;
                            *(cardgame_pair_t *)(p1 + a1 + 0x30a) = *(cardgame_pair_t *)(p1 + b + 0x30a);
                            a2 = n;
                        }
                    }
                    *(uint8_t *)(p1 + 0x41b) -= 1;
                    *(uint8_t *)(p1 + 0x41c) -= 1;
                    *(uint16_t *)((uint8_t *)dst_base + 78) = src_base[a3];
                    a3--;
                    *(uint8_t *)(p1 + 0x359) = 7;
                } while (a3 >= 0);
            }
            {
                int32_t fp;
                a3 = 0x27;
                fp = p1 + 0x4e;
                do {
                    *(int8_t *)(fp + 0x30a) = (int8_t)a3;
                    a3--;
                    fp -= 2;
                } while (a3 >= 0);
            }
            *(int16_t *)(t0 + 6) = 0;
        }
        ret = 1;
        break;
    }
    }
    return ret;
}
