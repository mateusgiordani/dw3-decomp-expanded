/*
 * CARDGAME:0x8008f2ec CARDGAME_F0x8008f2ec
 * 580 bytes at CARDGAME.PRO offset 0xc63c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8008f2ec
 *  Symbols     CARDGAME_F0x8008423c=0x8008423c
 *  Compare     580 bytes from 0x8008f2ec against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008f2ec
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * No Ghidra state change.
 *
 * Caller CARDGAME_F0x8008423c passes (ctx, card, row byte, col 0..4).
 *
 * Semantics: pending byte ctx+0x423 dispatches card slot calls, then moves to
 * active byte ctx+0x422; active==1 runs a paced cell update on the byte grid at
 * ctx+0x5a8 indexed by (col + row*200) with dir counter ctx+0x434 and limit
 * ctx+0x430; active==2 reports done (returns 1, else 0).
 *
 * Matching notes (all portable C, no register variables): the pending dispatch
 * tests ctx+0x423 twice (if + switch subject) so CSE keeps the move v1,v0 copy;
 * the switch subject is the MEM load itself; the dir/counter cells reload the
 * MEM fields per arm (byte-store aliasing blocks CSE); the cell address is
 * grouped as col + row*200; each dir arm stages the EXE-call segment.
 */

#include <stdint.h>

typedef void (*cardgame_f2ec_eb8_t)(void *card, unsigned int mask);
typedef void (*cardgame_f2ec_eb4_t)(void *card);
typedef void (*cardgame_f2ec_ea0_t)(void *card, unsigned int row, unsigned int col, unsigned int val);
typedef void (*cardgame_exe_vec_t)(uint32_t arg);

int CARDGAME_F0x8008f2ec(void *ctx, void *card, unsigned int row, unsigned int col)
{
    int ret = 0;
    int c;
    unsigned char *cell;
    unsigned int cellv;
    unsigned int exeseg;

    if (((unsigned char *)ctx)[0x423] != 0) {
        switch (((unsigned char *)ctx)[0x423]) {
        case 1:
            ((cardgame_f2ec_eb8_t)*(uint32_t *)((unsigned char *)card + 0xeb8))(card, (1u << (col * 2)) << row);
            *(int *)((unsigned char *)ctx + 0x424) = 0;
            *(int *)((unsigned char *)ctx + 0x428) = 0;
            break;
        case 2:
            ((cardgame_f2ec_eb4_t)*(uint32_t *)((unsigned char *)card + 0xeb4))(card);
            break;
        }
        {
            int pending = ((unsigned char *)ctx)[0x423];
            ((unsigned char *)ctx)[0x423] = 0;
            ((unsigned char *)ctx)[0x422] = (unsigned char)pending;
        }
    }
    c = ((unsigned char *)ctx)[0x422];
    switch (c) {
    case 1:
        if (*(int *)((unsigned char *)ctx + 0x424) == *(int *)((unsigned char *)ctx + 0x430) / 2) {
            if (*(int *)((unsigned char *)ctx + 0x434) > 0) {
                unsigned char *cell1;
                unsigned int cellv1;
                cell1 = (unsigned char *)ctx + (col + row * 200) + 0x5a8;
                cellv1 = *cell1;
                if (cellv1 < 99)
                    *cell1 = (unsigned char)(cellv1 + 1);
                *(int *)((unsigned char *)ctx + 0x434) = *(int *)((unsigned char *)ctx + 0x434) - 1;
                exeseg = 0x80040000;
            } else {
                unsigned char *cell2;
                unsigned int cellv2;
                cell2 = (unsigned char *)ctx + (col + row * 200) + 0x5a8;
                cellv2 = *cell2;
                if (cellv2 != 0)
                    *cell2 = (unsigned char)(cellv2 - 1);
                *(int *)((unsigned char *)ctx + 0x434) = *(int *)((unsigned char *)ctx + 0x434) + 1;
                exeseg = 0x80040000;
            }
            (*(cardgame_exe_vec_t)*(uint32_t *)0x80055c48)(exeseg | 0x52c6);
            cell = (unsigned char *)ctx + (col + row * 200) + 0x5a8;
            ((cardgame_f2ec_ea0_t)*(uint32_t *)((unsigned char *)card + 0xea0))(card, row, col, *cell);
        }
        *(int *)((unsigned char *)ctx + 0x424) = *(int *)((unsigned char *)ctx + 0x424) + 1;
        if (*(int *)((unsigned char *)ctx + 0x430) < *(int *)((unsigned char *)ctx + 0x424)) {
            if (*(int *)((unsigned char *)ctx + 0x434) == 0)
                ((unsigned char *)ctx)[0x423] = 2;
            else {
                cell = (unsigned char *)ctx + (col + row * 200) + 0x5a8;
                if (*cell == 0)
                    ((unsigned char *)ctx)[0x423] = 2;
                else
                    ((unsigned char *)ctx)[0x423] = 1;
            }
        }
        break;
    case 2:
        ret = 1;
        break;
    }
    return ret;
}
