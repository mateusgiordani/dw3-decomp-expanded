// CARDGAME:0x8008f2ec (size 580, 0x244)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0xc63c
// Boundary: prologue 27bdffd8 addiu sp,-0x28, saves s0/s1/s2/s3/s4/ra;
// s0=a0 (ctx), s2=a1 (card), s1=a2 (row), s3=a3 (col); epilogue jr ra +
// 27bd0028 addiu sp,+0x28 at 0x8008f528/0x8008f52c. Next framed
// CARDGAME:0x8008f530 at +0x244 (contiguous, no overlap).
// Ghidra program CARDGAME (project ddw3-pal-sles-03936) read-only: disasm
// 145 insns word-equal vs PAL @ 0xc63c
// (first8 27bdffd8 afb00010 00808021 afb20018 00a09021 afb10014 00c08821 afb3001c;
// last8 8fbf0024 8fb40020 8fb3001c 8fb20018 8fb10014 8fb00010 03e00008 27bd0028);
// decompile CARDGAME_F0x8008f2ec; x-ref to from CARDGAME_F0x8008423c at
// 0x80084308 (unconditional call, args ctx/card/row/col); x-ref from: 3 indirect
// jalr (card+0xeb8/0xeb4/0xea0) + EXE vector *0x80055c48, rest intra-function.
// cardgame.s is GUIDE only, never copied as source. No Ghidra state change.
// Caller CARDGAME_F0x8008423c passes (ctx, card, row byte, col 0..4).
// Semantics: pending byte ctx+0x423 dispatches card slot calls, then moves to
// active byte ctx+0x422; active==1 runs a paced cell update on the byte grid at
// ctx+0x5a8 indexed by (col + row*200) with dir counter ctx+0x434 and limit
// ctx+0x430; active==2 reports done (returns 1, else 0).
// Matching notes (all portable C, no register variables): the pending dispatch
// tests ctx+0x423 twice (if + switch subject) so CSE keeps the move v1,v0 copy;
// the switch subject is the MEM load itself; the dir/counter cells reload the
// MEM fields per arm (byte-store aliasing blocks CSE); the cell address is
// grouped as col + row*200; each dir arm stages the EXE-call segment.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base, no alternates).
// Status: C_MATCHING (exact_byte_match, sha256
// 34d401427e089bf1d6ce7140fb545e3e996501a16efd0be96b923c8e8d1dff4c, 580 B,
// 6 relocations, portable C, no asm, no explicit register variables).
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
