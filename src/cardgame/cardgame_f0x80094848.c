/*
 * CARDGAME:0x80094848 CARDGAME_F0x80094848
 * 816 bytes at CARDGAME.PRO offset 0x11b98 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80094848
 *  Symbols     CARDGAME_F0x80093bc4=0x80093bc4 CARDGAME_F0x80094848=0x80094848
 *  Compare     816 bytes from 0x80094848 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80094848
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Prologue 27bdffe0 addiu sp,-0x20 ; sw s0,0x10(sp) ; move s0,a0 ; sw
 * s1,0x14(sp) ; move s1,a1 ; sw s2,0x18(sp) ; sw ra,0x1c(sp)
 *
 * Body: state byte at p1+0x422 (1/2/3/4); state1 accumulates word pairs
 * 0x428/0x42c from short table +0x734/+0x736 stride 14 count *(u8*)(p1+0x72c)
 * and 0x430/0x434 from +0x7a6/+0x7a8 count *(u8*)(p1+0x79e), then compares
 * shorts 0x59c/0x59e/0x664/0x666 against words; state2 ticks (*0x8004df9c)()
 * into 0x424, overflow (>14) copies low halves to 0x59c/0x664/0x666/0x59e with
 * 4 indirect (p2+0xea0) calls then state 3, else EXE (*0x80055c48)(0x800452c6)
 * + 4x CARDGAME_F0x80093bc4 lerps with 4 indirect calls; state3 -> 4; state4 ->
 * 1; default 0.
 *
 * Epilogue lw ra,0x1c(sp) ; lw s2,0x18(sp) ; lw s1,0x14(sp) ; lw s0,0x10(sp) ;
 * jr ra ; addiu sp,+0x20 at 0x80094b60/0x80094b74; next CARDGAME:0x80094b78 at
 * +0x330 confirms size 0x330 contiguous (prologue/epilogue pair).
 */

#include <stdint.h>

typedef void (*cardgame_cb_t)(int32_t, int32_t, int32_t, int32_t);
typedef int32_t (*cardgame_tick_t)(void);
typedef void (*cardgame_exe_cb_t)(uint32_t);

int32_t CARDGAME_F0x80093bc4(int32_t a0, int32_t a1, int32_t a2, int32_t a3);

int32_t CARDGAME_F0x80094848(int32_t p1, int32_t p2)
{
    int32_t ret = 0;
    int32_t v;
    int32_t i;

    switch (*(uint8_t *)(p1 + 0x422)) {
    case 1:
        /* Bounds read in the condition: loop.c hoists them (lbu + move in PAL);
           i * 14 lets SR build the single p1 + 14 * i cursor. */
        for (i = 0; i < *(uint8_t *)(p1 + 0x72c); i++) {
            *(int32_t *)(p1 + 0x428) += *(int16_t *)(p1 + i * 14 + 0x734);
            *(int32_t *)(p1 + 0x42c) += *(int16_t *)(p1 + i * 14 + 0x736);
        }
        for (i = 0; i < *(uint8_t *)(p1 + 0x79e); i++) {
            *(int32_t *)(p1 + 0x430) += *(int16_t *)(p1 + i * 14 + 0x7a6);
            *(int32_t *)(p1 + 0x434) += *(int16_t *)(p1 + i * 14 + 0x7a8);
        }
        if (*(int16_t *)(p1 + 0x59c) != *(int32_t *)(p1 + 0x428) ||
            *(int16_t *)(p1 + 0x59e) != *(int32_t *)(p1 + 0x42c) ||
            *(int16_t *)(p1 + 0x664) != *(int32_t *)(p1 + 0x430) ||
            *(int16_t *)(p1 + 0x666) != *(int32_t *)(p1 + 0x434)) {
            *(int32_t *)(p1 + 0x424) = 0;
            *(uint8_t *)(p1 + 0x422) = 2;
        } else {
            *(uint8_t *)(p1 + 0x422) = 4;
        }
        break;
    case 2:
        *(int32_t *)(p1 + 0x424) += ((cardgame_tick_t)*(uint32_t *)0x8004df9c)();
        if (*(int32_t *)(p1 + 0x424) < 15) {
            ((cardgame_exe_cb_t)*(uint32_t *)0x80055c48)(0x800452c6u);
            v = CARDGAME_F0x80093bc4(*(int32_t *)(p1 + 0x428), *(int16_t *)(p1 + 0x59c), 15, *(int32_t *)(p1 + 0x424));
            ((cardgame_cb_t)*(uint32_t *)(p2 + 0xea0))(p2, 0, 8, v);
            v = CARDGAME_F0x80093bc4(*(int32_t *)(p1 + 0x42c), *(int16_t *)(p1 + 0x59e), 15, *(int32_t *)(p1 + 0x424));
            ((cardgame_cb_t)*(uint32_t *)(p2 + 0xea0))(p2, 0, 9, v);
            v = CARDGAME_F0x80093bc4(*(int32_t *)(p1 + 0x430), *(int16_t *)(p1 + 0x664), 15, *(int32_t *)(p1 + 0x424));
            ((cardgame_cb_t)*(uint32_t *)(p2 + 0xea0))(p2, 1, 8, v);
            v = CARDGAME_F0x80093bc4(*(int32_t *)(p1 + 0x434), *(int16_t *)(p1 + 0x666), 15, *(int32_t *)(p1 + 0x424));
            ((cardgame_cb_t)*(uint32_t *)(p2 + 0xea0))(p2, 1, 9, v);
        } else {
            *(uint16_t *)(p1 + 0x59c) = *(uint16_t *)(p1 + 0x428);
            *(uint16_t *)(p1 + 0x59e) = *(uint16_t *)(p1 + 0x42c);
            *(uint16_t *)(p1 + 0x664) = *(uint16_t *)(p1 + 0x430);
            *(uint16_t *)(p1 + 0x666) = *(uint16_t *)(p1 + 0x434);
            ((cardgame_cb_t)*(uint32_t *)(p2 + 0xea0))(p2, 0, 8, *(int16_t *)(p1 + 0x59c));
            ((cardgame_cb_t)*(uint32_t *)(p2 + 0xea0))(p2, 0, 9, *(int16_t *)(p1 + 0x59e));
            ((cardgame_cb_t)*(uint32_t *)(p2 + 0xea0))(p2, 1, 8, *(int16_t *)(p1 + 0x664));
            ((cardgame_cb_t)*(uint32_t *)(p2 + 0xea0))(p2, 1, 9, *(int16_t *)(p1 + 0x666));
            *(uint8_t *)(p1 + 0x422) = 3;
        }
        break;
    case 3:
        *(uint8_t *)(p1 + 0x422) = 4;
        break;
    case 4:
        ret = 1;
        break;
    }
    return ret;
}
