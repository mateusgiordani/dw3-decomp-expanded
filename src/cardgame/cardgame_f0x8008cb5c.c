/*
 * CARDGAME:0x8008cb5c CARDGAME_F0x8008cb5c
 * 688 bytes at CARDGAME.PRO offset 0x9eac (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008cb5c
 *  Symbols     DAT_8004B7D0=0x8004b7d0 D_80055c48=0x80055c48
 *  Compare     688 bytes from 0x8008cb5c against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008cb5c
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Framed function: prologue 27bdffc8 addiu sp,-0x38; saves s4/s2/s3/ra/s5/s1/s0
 * at 0x28/0x20/0x24/0x30/0x2c/0x1c/0x18(sp); s4=a0 (state p1), s2=a1 (table
 * p2); epilogue lw ra/s5/s4/s3/s2/s1/s0 + jr ra + addiu sp,+0x38 at
 * 0x8008cde8-0x8008ce08.
 *
 * Next framed CARDGAME:0x8008ce0c at +0x2b0 (27bdff78 prologue), size 0x2b0
 * contiguous.
 *
 * Behavior (conservative): mode byte p1+0x422 selects: 1 -> require halfword
 * p2+100==2 and byte p1+0x498==0, store 2 at p1+0x422, return -1; 3 -> require
 * p2+100==0 and p1+0x498==0, call (p2+0xf18)(p2,15) + (p2+0xeb4)(p2), return 0;
 * else return -1. Mode 2 path: two rounds of bit test (A(0) >> B(0,k)) & 1 with
 * k=0xd then k=0xe; first set -> switch on word p1+0x438 (0x1400/0x400->half
 * p1+0x5a2, 0x1000->0x5a4, 0x4000->0x5a6, 0x2000->0x66c, 0x8000->0x66e;
 * unmatched or half!=0 -> (*0x80055c48)(0x4001c), return 1; matched half==0 ->
 * return -1); first clear + second set -> (*0x80055c48)(0x800450bd),
 * (p2+0xec4)(p2), u32 p1+0x440=1, byte p1+0x422=3, (p2+0xf24)(p2,15,4,0,0x1000,
 * 5th arg at sp+0x10), byte p1+0x499=entry mode (2), return -1; both clear ->
 * return -1.
 *
 * - Each bit poll is one expression (A(0) >> B(0,k)) & 1 through the shared
 * DAT_8004B7D0 base (no explicit & 31: srav masks).
 *
 * - The EXE vector goes through the symbol D_80055c48, so its lui is a separate
 * insn that fills the beq delay slot.
 *
 * - The p2 callbacks return int: with a void type the EXE(0x4001c) call and the
 * case-3 (p2+0xeb4) call cross-jump into one jalr (676 B).
 */

#include <stdint.h>

typedef int32_t (*cardgame_exe_fn0_t)(int32_t);
typedef int32_t (*cardgame_exe_fn1_t)(int32_t, int32_t);
typedef void (*cardgame_exe_vec_t)(int32_t);
typedef int32_t (*cardgame_cb1_t)(int32_t);
typedef void (*cardgame_cb2_t)(int32_t, int32_t);
typedef void (*cardgame_cb5_t)(int32_t, int32_t, int32_t, int32_t, int32_t);

extern uint32_t DAT_8004B7D0[];
extern cardgame_exe_vec_t D_80055c48;

int32_t CARDGAME_F0x8008cb5c(int32_t p1, int32_t p2)
{
    int32_t ret;
    int32_t mode;
    uint32_t *tbl;

    ret = -1;
    mode = *(uint8_t *)(p1 + 0x422);
    switch (mode) {
    case 1:
        if (*(int16_t *)(p2 + 100) == 2 && *(uint8_t *)(p1 + 0x498) == 0)
            *(uint8_t *)(p1 + 0x422) = 2;
        break;
    case 2:
        tbl = DAT_8004B7D0;
        if (((((cardgame_exe_fn0_t)tbl[253])(0) >> ((cardgame_exe_fn1_t)tbl[258])(0, 13)) & 1) != 0) {
            switch (*(int32_t *)(p1 + 0x438)) {
            case 0x400:
            case 0x1400:
                if (*(int16_t *)(p1 + 0x5a2) != 0)
                    ret = 1;
                break;
            case 0x1000:
                if (*(int16_t *)(p1 + 0x5a4) != 0)
                    ret = 1;
                break;
            case 0x4000:
                if (*(int16_t *)(p1 + 0x5a6) != 0)
                    ret = 1;
                break;
            case 0x2000:
                if (*(int16_t *)(p1 + 0x66c) != 0)
                    ret = 1;
                break;
            case 0x8000:
                if (*(int16_t *)(p1 + 0x66e) != 0)
                    ret = 1;
                break;
            default:
                ret = 1;
                break;
            }
            if (ret == 1)
                D_80055c48(0x4001c);
            break;
        }
        if (((((cardgame_exe_fn0_t)tbl[253])(0) >> ((cardgame_exe_fn1_t)tbl[258])(0, 14)) & 1) != 0) {
            D_80055c48(0x800450bd);
            ((cardgame_cb1_t)*(uint32_t *)(p2 + 0xec4))(p2);
            *(uint32_t *)(p1 + 0x440) = 1;
            *(uint8_t *)(p1 + 0x422) = 3;
            ((cardgame_cb5_t)*(uint32_t *)(p2 + 0xf24))(p2, 0xf, 4, 0, 0x1000);
            *(uint8_t *)(p1 + 0x499) = mode;
        }
        break;
    case 3:
        if (*(int16_t *)(p2 + 100) == 0 && *(uint8_t *)(p1 + 0x498) == 0) {
            ((cardgame_cb2_t)*(uint32_t *)(p2 + 0xf18))(p2, 0xf);
            ret = 0;
            ((cardgame_cb1_t)*(uint32_t *)(p2 + 0xeb4))(p2);
        }
        break;
    }
    return ret;
}
