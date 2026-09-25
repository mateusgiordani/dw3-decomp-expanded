// CARDGAME:0x8009d540 (size 372, 0x174)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1a890
// Boundary: symbols/functions.csv CARDGAME 0x8009d540-0x8009d6b4 (372B, 93 insns)
//   Prologue 27bdffe8 addiu sp,-0x18; sw s0,0x10(sp); move s0,a0; sw ra,0x14(sp)
//   Epilogue lw ra,0x14(sp); lw s0,0x10(sp); jr ra; addiu sp,+0x18
//   Prev CARDGAME 0x8009d310 (560B) ends at 0x8009d540; next CARDGAME 0x8009d6b4
//   (prologue 27bdffe8) starts at +0x174, confirming no overlap.
// Ghidra CARDGAME (ddw3-pal-sles-03936, read-only, base 0x80082cb0 verified):
//   disasm 0x8009d540 --instructions 93 + tail 0x8009d694 --instructions 12,
//   PAL identical word-for-word; state gate *0xc ==1, dispatch on *0x10,
//   table 0x800a5bf0 (halfword pairs, addu offset-first, lh 0 / lh +2),
//   EXE 0x80044f44/0x80044f3c, halfwords +0x54/+0x56, method at +0x28, +0x50 flag
//   x-ref to 0x8009d540: 1 caller CARDGAME_F0x8009d6b4 via EXE_F0x80014504(task,0x58,0)
//   (task registration, evidence for callback role, not direct jal)
//   Ghidra state changed: no (read-only queries only)
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (variant base)
// Candidate status: EXACT (372B/372B, 0 diffs) - r5 S4, pending parent reproduction
//   r5 mechanism: integer table base + scaled offset before base (offset-first
//   addu), fresh idx re-read defeating CSE (reload lh + hazard nop), pointer
//   [1] second access (lh +2 offset, no ori). See strategy-r5.md.
#include <stdint.h>

extern int16_t D_800a5bf0[];
extern uint16_t D_8005cca8;
extern int32_t D_80044f44;
extern int32_t D_80044f3c;

void CARDGAME_F0x8009d540(int32_t arg0)
{
    int32_t state;
    int32_t v1;

    state = *(int32_t *)(arg0 + 0xc);
    if (state == 1)
        goto body;
    if (state < 2)
        goto else_body;
    if (state < 4)
        goto out;
else_body:
    *(int16_t *)(arg0 + 0x54) = 0;
    ((void (*)(int32_t, int32_t))*(void **)(arg0 + 0x28))(arg0, 1);
    *(int32_t *)(arg0 + 0x50) = 0;
    goto out;
body:
    v1 = *(int32_t *)(arg0 + 0x10);
    if (v1 == 0)
        goto do_block;
    if (v1 == 1)
        goto skip_block;
do_block:
    {
        int16_t idx = *(int16_t *)(arg0 + 0x54);
        int32_t tbl = (int32_t)D_800a5bf0;
        int32_t off = (int32_t)idx * 4;
        int16_t val = *(int16_t *)(off + tbl);
        *(int16_t *)(arg0 + 0x56) = val;
        if (*(int16_t *)((int32_t)(*(int16_t *)(arg0 + 0x54)) * 4 + tbl + 2) != 0) {
            *(int16_t *)(arg0 + 0x56) = val + (int16_t)D_8005cca8;
        }
        ((void (*)(int32_t))D_80044f44)(*(int16_t *)(arg0 + 0x56));
        *(int32_t *)(arg0 + 0x10) = *(int32_t *)(arg0 + 0x10) + 1;
    }
skip_block:
    if (((int32_t (*)(int32_t))D_80044f3c)(*(int16_t *)(arg0 + 0x56)) != 0)
        goto out;
    {
        int16_t cur = *(int16_t *)(arg0 + 0x54);
        int16_t nxt = cur + 1;
        int32_t t;
        *(int16_t *)(arg0 + 0x54) = nxt;
        t = (int32_t)nxt * 4;
        if (D_800a5bf0[t >> 1] == -2) {
            *(int16_t *)(arg0 + 0x54) = cur + 2;
            *(int32_t *)(arg0 + 0x50) = 1;
        }
        if (D_800a5bf0[*(int16_t *)(arg0 + 0x54) * 2] == -1) {
            ((void (*)(int32_t, int32_t))*(void **)(arg0 + 0x28))(arg0, 3);
        }
        *(int32_t *)(arg0 + 0x10) = 0;
    }
out:
    return;
}
