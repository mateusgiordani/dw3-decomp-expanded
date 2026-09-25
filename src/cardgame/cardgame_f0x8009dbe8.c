/*
 * CARDGAME:0x8009dbe8 CARDGAME_F0x8009dbe8
 * 732 bytes at CARDGAME.PRO offset 0x1af38 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009dbe8
 *  Symbols     CARDGAME_F0x8009dbe8=0x8009dbe8
 *  Compare     732 bytes from 0x8009dbe8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009dbe8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Table slots (from CARDGAME_F0x8009d310): ed8/f14/f18/f3c are EXE-callback
 * function pointers reached through tbl = *(param_2 + 0x18); rows stride 0x4c
 * (76), 40 rows.
 *
 * Signature is conservative: raw byte offsets, no invented struct; widths from
 * access widths (lh=l signed half, lbu/sb = byte, sw/lw = word). Incoming a1
 * (param_2) is saved and reloaded per row; tbl is re-read from it in the copy
 * step, reproduced here.
 */

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
