// CARDGAME:0x80086a30 (size 760, 0x2f8)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x3d80
// Framed function from boundary sweep reports/handoffs/cardgame-boundary-sweep.md #11:
// prologue 27bdff88 addiu sp,-0x78 ; sw s0,0x68(sp) ; move s0,a0 ; sw s1,0x6c(sp) ;
// move s1,a1 ; sw s2,0x70(sp) ; sw ra,0x74(sp) ; jal 0x80086a18 (delay move s2,a2) ;
// epilogue lw ra,0x74(sp) ; lw s2,0x70(sp) ; lw s1,0x6c(sp) ; lw s0,0x68(sp) ;
// jr ra ; addiu sp,+0x78. Contiguous: next framed CARDGAME:0x80086d28 at
// 0x3d80+0x2f8=0x4078 (prologue/epilogue pair); prev 24B init CARDGAME:0x80086a18
// ends exactly at 0x80086a30.
// Ghidra project ddw3-pal-sles-03936 program CARDGAME (base 0x80082cb0 known in
// symbols/overlays.csv), read-only: disasm 190 instr, decompile
// CARDGAME_F0x80086a30, x-ref to 0x80086a30 -> 8 callers (0x80086d60 wrapper,
// 0x8008c2b0/0x8008c4b4 in 0x8008c044, 0x8008b320 in 0x8008ad98, 0x8008a48c in
// 0x8008a068, 0x800884dc/0x80088ed4/0x80088c88 in 0x80087edc).
// PAL word check vs primary cardgame.bin: off 0x3d80 prologue 0x27bdff88,
// epilogue jr 0x03e00008 + 0x27bd0078; Ghidra LE bytes match (88ffbd27 ...).
// Callees/data (PAL-verified): jal CARDGAME:0x80086a18 (clears a1+0xe32/0xe34/
// 0xe4c/0xe64/0xe7c); jal EXE:0x8001ebf8 (fills sp+0x10 buffer, fn ptr at
// sp+0x3c); jalr via that fn ptr (arg s0val+1); jump table CARDGAME:0x80083294
// (6 entries -> case bodies at 0x80086cc0..0x80086cf8); halfword table
// CARDGAME:0x800a5c20 (5 entries 0x44,0x6f,0x9a,0xc5,0xf0).
// Entry stride 0x4c via sll/addu strength-reduced chain; plain C '* 76' matches
// that chain under the pinned toolchain (precedent cardgame_f0x8009ca3c.c,
// C_MATCHING with 'a1 * 76').
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base hypothesis).
#include "common/types.h"

extern void CARDGAME_F0x80086a18(void *a0, void *a1);
void EXE_F0x8001ebf8(void *buf);

extern int16_t CARDGAME_TBL_800a5c20[];

typedef struct {
    void *head;          /* +0x00: dereferenced, byte +0x06 selects the switch */
    uint8_t pad1[0x28];  /* +0x04..+0x2b: filled by the EXE helper, unread here */
    void (*fn)(int32_t); /* +0x2c: indirect target written by the EXE helper */
    uint8_t pad2[0x28];  /* +0x30..+0x57: EXE-helper workspace, unread here */
} cardgame_6a30_tmp_t;   /* 0x58 bytes: frame 0x78 - 0x10 args - 0x10 saved regs */

void CARDGAME_F0x80086a30(void *a0, uint8_t *b, int32_t a2)
{
    uint32_t mask;
    int32_t added;
    int32_t found;
    int32_t i;
    int32_t sval;
    cardgame_6a30_tmp_t tmp;
    uint8_t sel;

    uint8_t *t;

    CARDGAME_F0x80086a18(a0, b);

    {
        uint8_t *e = b + (a2 + *(int32_t *)((uint8_t *)a0 + 0x43c)) * 76;
        if (*(int16_t *)(e + 0x144) != 0) {
            mask = *(uint32_t *)(b + 0xe34) | *(uint8_t *)(e + 0x149);
            *(uint32_t *)(b + 0xe34) = mask;
            {
                uint8_t *p = b + (a2 + *(int32_t *)((uint8_t *)a0 + 0x43c)) * 76;
                mask |= (*(int16_t *)(p + 0x140) << 4);
            }
            *(int16_t *)(b + 0xe32) = 1;
            *(uint32_t *)(b + 0xe34) = mask;
        }
    }

    {
        uint8_t *e = b + (a2 + *(int32_t *)((uint8_t *)a0 + 0x43c)) * 76;
        if (*(uint8_t *)(e + 0x14d) == 3)
            goto exit_special;

        found = 0;
        added = *(int16_t *)((uint8_t *)a0 + (*(int16_t *)(e + 0x142) << 1) + 0x50) + 1;
        *(int32_t *)(b + 0xe4c) = added;
        for (i = 0; i < 5; i++) {
            if (CARDGAME_TBL_800a5c20[i] == *(int16_t *)((uint8_t *)a0 + (*(int16_t *)(b + (mask = (a2 + *(int32_t *)((uint8_t *)a0 + 0x43c)) * 76) + 0x142) << 1) + 0x50))
                found = 1;
        }
    }
    {
        uint8_t *e;
        t = b + (a2 + *(int32_t *)((uint8_t *)a0 + 0x43c)) * 76;
        if (*(int16_t *)(t + 0x144) != 0 && found == 0) {
            *(int32_t *)(b + 0xe7c) = 500;
            *(uint8_t *)(b + 0xe82) = 1;
            e = b + (a2 + *(int32_t *)((uint8_t *)a0 + 0x43c)) * 76;
            *(uint8_t *)(b + 0xe80) = *(uint8_t *)(e + 0x14b);
            e = b + (a2 + *(int32_t *)((uint8_t *)a0 + 0x43c)) * 76;
            *(uint8_t *)(b + 0xe81) = *(uint8_t *)(e + 0x14c);
        } else {
            *(uint8_t *)(b + 0xe82) = 0;
            *(int32_t *)(b + 0xe7c) = added;
        }
    }
    {
        uint8_t *e = b + (a2 + *(int32_t *)((uint8_t *)a0 + 0x43c)) * 76;
        sval = *(int16_t *)((uint8_t *)a0 + (*(int16_t *)(e + 0x142) << 1) + 0x50);
        EXE_F0x8001ebf8(&tmp);
        tmp.fn(sval + 1);
        sel = ((uint8_t *)tmp.head)[6];
    }
    switch (sel) {
    case 0:
        *(int32_t *)(b + 0xe64) = 0;
        break;
    case 1:
        *(int32_t *)(b + 0xe64) = 0x25;
        break;
    case 2:
        *(int32_t *)(b + 0xe64) = 0x26;
        break;
    case 3:
        *(int32_t *)(b + 0xe64) = 0x27;
        break;
    case 4:
        *(int32_t *)(b + 0xe64) = 0x28;
        break;
    case 5:
        *(int32_t *)(b + 0xe64) = 0x29;
        break;
    }
    return;

exit_special:
    *(uint8_t *)(b + 0xe82) = 0;
    *(int32_t *)(b + 0xe7c) = 500;
}