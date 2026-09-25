/*
 * CARDGAME:0x80086a30 CARDGAME_F0x80086a30
 * 760 bytes at CARDGAME.PRO offset 0x3d80 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80086a30, jump table (.rodata) at 0x80083294
 *  Symbols     CARDGAME_F0x80086a18=0x80086a18 CARDGAME_TBL_800a5c20=0x800a5c20
 *              EXE_F0x8001ebf8=0x8001ebf8 cardgame_f0x8009ca3c=0x8009ca3c
 *  Compare     760 bytes from 0x80086a30 and the jump table against the PAL
 *              overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80086a30
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Contiguous: next framed CARDGAME:0x80086d28 at 0x3d80+0x2f8=0x4078
 * (prologue/epilogue pair); prev 24B init CARDGAME:0x80086a18 ends exactly at
 * 0x80086a30.
 *
 * Callees/data (PAL-verified): jal CARDGAME:0x80086a18 (clears a1+0xe32/0xe34/
 * 0xe4c/0xe64/0xe7c); jal EXE:0x8001ebf8 (fills sp+0x10 buffer, fn ptr at
 * sp+0x3c); jalr via that fn ptr (arg s0val+1); jump table CARDGAME:0x80083294
 * (6 entries -> case bodies at 0x80086cc0..0x80086cf8); halfword table
 * CARDGAME:0x800a5c20 (5 entries 0x44,0x6f,0x9a,0xc5,0xf0).
 */

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