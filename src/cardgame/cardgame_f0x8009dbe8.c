// CARDGAME:0x8009dbe8 (size 732, 0x2DC)
// PAL: reference/extracted/pro/cardgame.bin base 0x80082cb0 file-off 0x1af38
// Ghidra project ddw3-pal-sles-03936 program CARDGAME (live, read-only, no mutation):
//   summary: min 0x80082cb0 max 0x800a5ddf MIPS LE 32, 286 functions (image_base 00000000
//     is the raw-BinaryLoader staging artifact; actual block starts at verified base).
//   function list: CARDGAME_F0x8009dbe8 size 732; next CARDGAME_F0x8009dec4 confirms end.
//   disasm 0x8009dbe8 (183 instrs) matches PAL words byte-for-byte (head 0x27bdffc8,
//     0xafb60028 ... tail 0x8fb10014, 0x8fb00010, 0x03e00008, 0x27bd0038); full prologue
//     addiu sp,-0x38 + 10 callee-saved stores, epilogue jr ra + addiu sp,+0x38.
//   decompile 0x8009dbe8 -> count select on param_4 (0/1/2) then per-row loop (hypothesis
//     only, confirmed against disasm, not copied).
//   x-ref to 0x8009dbe8: one UNCONDITIONAL_CALL from 0x8009e81c in CARDGAME_F0x8009e668.
//   graph callers depth2: 0x8009e668 <- FUN_800a2df8; graph callees: none (all calls are
//     indirect via table slots +0xed8/+0xf14/+0xf18/+0xf3c installed by CARDGAME_F0x8009d310).
// Table slots (from CARDGAME_F0x8009d310): ed8/f14/f18/f3c are EXE-callback function
// pointers reached through tbl = *(param_2 + 0x18); rows stride 0x4c (76), 40 rows.
// Upstream cardgame.s GUIDE only (never authority for base/boundary/compiler).
// Domain pack: none generated. Target touches GAME card logic only (count select +
// per-row indirect dispatch + byte copies); no battle/camera, skills, Digimon records,
// dialogue, field/maps/tiles, sprites/rendering, disc I/O, or overlay-contract question
// is actually exercised, so no read-only domain pack applies.
// Signature is conservative: raw byte offsets, no invented struct; widths from access
// widths (lh=l signed half, lbu/sb = byte, sw/lw = word). Incoming a1 (param_2) is saved
// and reloaded per row; tbl is re-read from it in the copy step, reproduced here.
// Toolchain: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79 -O2 -G0 (base variant):
// exact_byte_match 732/732 (r7 o55).
#include <stdint.h>

typedef int32_t (*dbe8_get_t)(int32_t, int32_t);
typedef void (*dbe8_put_t)(char *, int32_t, int32_t, int32_t);
typedef void (*dbe8_clr_t)(char *, int32_t);
typedef void (*dbe8_val_t)(char *, int32_t, int32_t);

void CARDGAME_F0x8009dbe8(char *p1, char *p2, int32_t idx, int32_t mode)
{
    char *tbl;
    char *sub;
    int32_t count;
    int32_t i;
    int32_t tmp;
    int32_t base;
    int32_t k;

    tbl = *(char **)(p2 + 0x18);
    sub = p1 + 0x498;

    /* k in a local: with idx * 200 inline the expander emits the product
       before p1 (addu v0,v0,p1); PAL has p1 first. */
    switch (mode) {
    case 0:
        k = idx * 200;
        *(int32_t *)(sub + 0x3c) = *(int16_t *)(p1 + k + 0x5a6);
        break;
    case 1:
        k = idx * 200;
        *(int32_t *)(sub + 0x3c) = *(int16_t *)(p1 + k + 0x5a2);
        break;
    case 2:
        k = idx * 200;
        *(int32_t *)(sub + 0x3c) = *(int16_t *)(p1 + k + 0x5a4);
        break;
    }
    *(int32_t *)(sub + 0x30) = 0;
    *(int32_t *)(sub + 0x34) = 0;
    *(int32_t *)(sub + 0x38) = 0;
    *(int32_t *)(sub + 0x40) = *(int32_t *)(sub + 0x3c) * 4 + 10;

    if (*(int32_t *)(sub + 0x3c) != 0) {
        base = idx * 200;
        for (i = 0; i < 40; i++) {
            count = *(int32_t *)(sub + 0x3c);
            if (i < count) {
                tmp = ((dbe8_get_t)*(void **)(tbl + 0xed8))(count, i);
                ((dbe8_put_t)*(void **)(tbl + 0xf14))(tbl, i, tmp + 0x1800, 0x6100);
                switch (mode) {
                case 0:
                    ((dbe8_val_t)*(void **)(tbl + 0xf3c))(tbl, i, *(int16_t *)(p1 + (base + i * 2) + 0x600));
                    break;
                case 1:
                    ((dbe8_val_t)*(void **)(tbl + 0xf3c))(tbl, i, *(int16_t *)(p1 + (base + i * 2) + 0x614));
                    break;
                case 2:
                    ((dbe8_val_t)*(void **)(tbl + 0xf3c))(tbl, i, *(int16_t *)(p1 + ((*(int16_t *)(p1 + base + 0x5a0) + i) * 2 + base) + 0x5b0));
                    break;
                }
                if (*(uint8_t *)(sub + 4) != 0) {
                    *(char *)(tbl + i * 0x4c + 0x14d) = 2;
                }
                *(int16_t *)(tbl + i * 0x4c + 0x120) = 0;
                /* Same for the row offset: keeps the reloaded table base first. */
                {
                    int32_t ro = i * 0x4c;
                    *(char *)(*(char **)(p2 + 0x18) + ro + 0x151) = *(char *)(p1 + i + 0x49e);
                }
            } else {
                ((dbe8_clr_t)*(void **)(tbl + 0xf18))(tbl, i);
            }
        }
        *(char *)(sub + 4) = 0;
    } else {
        *(char *)(sub + 4) = 0;
        tmp = ((dbe8_get_t)*(void **)(tbl + 0xed8))(*(int32_t *)(sub + 0x3c), 0);
        ((dbe8_put_t)*(void **)(tbl + 0xf14))(tbl, 0, tmp + 0x1800, 0x6100);
        *(char *)(tbl + 0x14d) = 3;
        *(int16_t *)(tbl + 0x120) = 0;
        for (i = 1; i < 40; i++) {
            ((dbe8_clr_t)*(void **)(tbl + 0xf18))(tbl, i);
        }
    }
}
