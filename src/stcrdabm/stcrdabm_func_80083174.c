#include "common/types.h"

/* STCRDABM:0x80083174 (1096 bytes, 274 instructions; file off 0x4c4 = vaddr - 0x80082cb0)
 * PAL-SLES-03936; reviewed body-80083174, base 0x80082cb0.
 * Card-album candidate grid renderer (album role unconfirmed; file name only).
 *
 * PAL-first (Ghidra ddw3-pal-sles-03936 / STCRDABM, read-only, no state change):
 * - disasm 0x80083174 (+0x80083214/+0x800832b4/+0x80083354/+0x800833f4/+0x80083494/+0x80083534,
 *   40 insns each): prologue addiu sp,-0x150, saves ra/s0-s8, epilogue jr ra + addiu sp,+0x150.
 * - 2 direct jals: EXE 0x8001f648 (draw-packet init, C_MATCHING) on sp+0x10,
 *   EXE 0x8001ebf8 (DISASSEMBLED) on sp+0xb0; rest are jalr through stack slots.
 * - decompile STCRDABM_func_80083174(int param_1, int param_2) cross-checked insn by insn.
 * - x-ref to: 1 caller STCRDABM_func_80083848 @ 0x80083a58 jal (a0 = caller s0,
 *   a1 = 0/1 flag selecting p1+0x5c vs p1+0x60 base).
 * - Reference words (primary checkout reference/extracted/pro/stcrdabm.bin @ off 1220):
 *   head 27BDFEB0 AFA40150 27A40010 AFBF014C ... match disasm LE bytes; PAL authority.
 *
 * Recovered accesses (conservative; no structs invented):
 * - p1+0x54/p1+0x58: args forwarded to two draw-packet methods.
 * - p1+0x64: entry count (outer loop bound); p2 selects base p1+0x5c (flag 0) / p1+0x60.
 * - Grid: col = i / 6, row = i % 6; x = row * 42, y = col * 54
 *   (signed /6 via 0x2AAAAAAB magic mult confirmed in disasm).
 * - Guard: return whole function when base + i >= 0x13b (315).
 * - Occupancy byte table at 0x80049213 (EXE low memory: lui 0x8005 - 0x72cc + 0x4df);
 *   zero byte takes the "empty slot" path (tile id 6), nonzero draws entry data.
 * - Entry data pointer = word at sp+0xb0 (work[0], filled by F0x8001ebf8); PAL re-reads
 *   work[0] per digit block (no separate spill slot); bytes [0..2] used:
 *   [0]-1 is the final tile id, [1]/[2] are drawn as two decimal tiles each
 *   (tens via /10 magic 0x66666667, 0 when tens is 0, ones + 0x1e; fixed tile 0x1c).
 * - Word at 0x80044f5c (s8 0x80044b38 + 0x424, sysbase[265]) holds a code pointer,
 *   called with 0x6050000; 0x80044b38 is pinned in s8 across the loop.
 * - Entry bytes are routed through signed int temporaries: cc1 lowers byte /10
 *   as unsigned (multu/0xcccccccd) but PAL shows signed (mult/0x66666667).
 * - work[] slot map (sp off -> work off = sp - 0xb0): 0xdc->0x2c, 0xe4->0x34,
 *   0xe8->0x38, 0xec->0x3c, 0xf0->0x40, 0xf4->0x44, 0x100->0x50. Contents filled
 *   by F0x8001ebf8 (DISASSEMBLED); data/code split inside work[] is unconfirmed.
 * - draw[] slot map (buf off = sp - 0x10): 0x84->0x74, 0x8c->0x7c, 0x94->0x84.
 *
 * Status: exact_byte_match 1096/1096 with psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, base
 * (argv adds --symbol D_80044B38=0x80044b38 --symbol D_80048D34=0x80048d34).
 * Matching notes: X/Y receive the grid products and x/y are copies used only by the
 * occupied-slot prefix, so CSE keeps X/Y canonical in the digit, final and empty-slot draws;
 * each branch has its own draw call (jump2 merges only the final jalr); t is reused for
 * quotient, remainder and digit counter.
 * Uncertainty preserved: p1/p2 stay ints (no record struct); work[]/draw[] stay
 * byte buffers (no packet struct); D_80049213 bounds unconfirmed; album semantics
 * unconfirmed.
 */
extern void F0x8001f648(void *buf);
extern void F0x8001ebf8(void *buf);
extern uint8_t D_80048D34[];
extern struct {
    uint8_t pad[0x424];
    int32_t (*fn424)(int32_t);
} D_80044B38;

void STCRDABM_func_80083174(int32_t p1, int32_t p2)
{
    uint8_t draw[0xa0];
    uint8_t work[0x58];
    int32_t dig[6];
    int32_t b0;
    int32_t b1;
    int32_t b2;
    int32_t base;
    int32_t i;
    int32_t col;
    int32_t row;
    int32_t x;
    int32_t y;
    int32_t v;
    int32_t vid;
    int32_t t;
    int32_t k;
    int32_t dx;
    int32_t X;
    int32_t Y;

    i = 0;
    F0x8001f648(draw);
    ((void (*)(int32_t, int32_t))*(void **)(draw + 0x7c))(*(int32_t *)(p1 + 0x54), *(int32_t *)(p1 + 0x58));
    ((void (*)(int32_t, int32_t))*(void **)(draw + 0x74))(0x280, 0);
    F0x8001ebf8(work);
    ((void (*)(int32_t, int32_t))*(void **)(work + 0x3c))(0x140, 0x100);
    ((void (*)(int32_t, int32_t))*(void **)(work + 0x40))(0x300, 0x100);
    ((void (*)(int32_t, int32_t))*(void **)(work + 0x38))(*(int32_t *)(p1 + 0x54), *(int32_t *)(p1 + 0x58));
    if (*(int32_t *)(p1 + 0x64) <= 0)
        return;
    do {
        if (p2 != 0)
            base = *(int32_t *)(p1 + 0x60) + i;
        else
            base = *(int32_t *)(p1 + 0x5c) + i;
        if (base >= 0x13b)
            return;
        row = i % 6;
        col = i / 6;
        X = row * 42;
        x = X;
        Y = col * 54;
        y = Y;
        if (D_80048D34[base + 0x4df] != 0) {
            ((void (*)(int32_t))*(void **)(work + 0x2c))(base);
            ((void (*)(int32_t, int32_t))*(void **)(work + 0x44))(row, col);
            ((void (*)(int32_t, int32_t))*(void **)(work + 0x34))(x + 0x27, y + 0x34);
            if (((int32_t (*)(void))*(void **)(work + 0x50))() != 0) {
                v = D_80044B38.fn424(0x6050000);
                ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(draw + 0x84))(v, 0x1d, x + 0x27, y + 0x53);
            } else {
                b1 = (*(uint8_t **)work)[1];
                t = b1 / 10;
                if (t != 0)
                    dig[0] = t + 0x1e;
                else
                    dig[0] = 0;
                dx = 0x27;
                dig[2] = 0x1c;
                t = b1 % 10;
                dig[1] = t + 0x1e;
                t = 0;
                do {
                    if (dig[t] != 0) {
                        v = D_80044B38.fn424(0x6050000);
                        ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(draw + 0x84))(v, dig[t], X + dx, Y + 0x53);
                    }
                    t += 1;
                    dx += 7;
                } while (t < 3);
                b2 = (*(uint8_t **)work)[2];
                t = b2 / 10;
                if (t != 0)
                    dig[0] = t + 0x1e;
                else
                    dig[0] = 0;
                dx = 0x3a;
                t = b2 % 10;
                dig[1] = t + 0x1e;
                t = 0;
                do {
                    if (dig[t] != 0) {
                        v = D_80044B38.fn424(0x6050000);
                        ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(draw + 0x84))(v, dig[t], X + dx, Y + 0x53);
                    }
                    t += 1;
                    dx += 7;
                } while (t < 2);
            }
            v = D_80044B38.fn424(0x6050000);
            ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(draw + 0x84))(v, (*(uint8_t **)work)[0] - 1, X + 0x23, Y + 0x32);
        } else {
            v = D_80044B38.fn424(0x6050000);
            ((void (*)(int32_t, int32_t, int32_t, int32_t))*(void **)(draw + 0x84))(v, 6, X + 0x23, Y + 0x32);
        }
        i += 1;
    } while (i < *(int32_t *)(p1 + 0x64));
}
