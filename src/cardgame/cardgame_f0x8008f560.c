/*
 * CARDGAME:0x8008f560 CARDGAME_F0x8008f560
 * 496 bytes at CARDGAME.PRO offset 0xc8b0 (overlay loaded at 0x80082cb0).
 *
 * Byte-match recipe (generated from recipes/card_cage.json by
 * tools/recipe_headers.py). Compiling this file as below reproduces the PAL
 * bytes of the function.
 *
 *  Preprocess  clang -E -nostdinc -include include/ps1_types.h -I include
 *  Compile     cc1 -quiet -O2 -G0 -mips1 -msoft-float -fno-strength-reduce
 *  Variant     o2-g0-no-strength-reduce
 *  Toolchain A (public, default)
 *    cc1       gcc-2.8.1-psx (decompals/old-gcc)
 *    assemble  maspsx 874855c --aspsx-version=2.79, then mipsel-linux-gnu-as
 *              -EL -march=r3000 -mtune=r3000 -no-pad-sections -O1 -G0
 *  Toolchain B (original PsyQ, optional)
 *    cc1       CC1PSX 2.8.1 SN32 BUILD 4.0.0010
 *    assemble  ASPSX 2.79, after removing the zero-divisor guard
 *              (tools/div_guard.py); the overlays have none and ASPSX would
 *              insert one after every division.
 *  Link        .text at 0x8008f560
 *  Symbols     cardgame_f0x80085f38=0x80085f38
 *  Compare     496 bytes from 0x8008f560 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8008f560
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Toolchain psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, -O2 -G0, variant
 * o2-g0-no-strength-reduce. Caller CARDGAME:0x80084320 (+0x852d4).
 */

#include <stdint.h>

typedef void (*cardgame_f560_fn1_t)(void *ctx, int32_t arg);
typedef void (*cardgame_f560_fn0_t)(void *ctx);
typedef void (*cardgame_f560_fn3_t)(void *ctx, int32_t a1, int32_t a2, int32_t a3);

int32_t CARDGAME_F0x8008f560(uint8_t *obj, uint8_t *ctx) {
    cardgame_f560_fn1_t fn1;
    cardgame_f560_fn0_t fn0;
    cardgame_f560_fn3_t fn3;
    int32_t result = 0;
    int32_t saved;
    int32_t i;
    int32_t j;
    int32_t off;
    int32_t base;
    int32_t cell;
    int32_t found;
    int32_t i2;
    int32_t j2;
    int32_t off2;
    int32_t base2;

    if (obj[0x423] != 0) {
        switch (obj[0x423]) {
        case 1:
            fn1 = *(cardgame_f560_fn1_t *)(ctx + 0xEB8);
            fn1(ctx, 0x3FF);
            *(int32_t *)(obj + 0x424) = 0;
            *(int32_t *)(obj + 0x428) = 0;
            break;
        case 2:
            fn0 = *(cardgame_f560_fn0_t *)(ctx + 0xEB4);
            fn0(ctx);
            break;
        }
        saved = obj[0x423];
        obj[0x423] = 0;
        obj[0x422] = (uint8_t)saved;
    }
    switch (obj[0x422]) {
    case 1: {
        if (*(int32_t *)(obj + 0x424) == *(int32_t *)(obj + 0x430) / 2) {
            for (i = 0, off = 0; i < 2; i++, off += 200) {
                uint8_t *p;
                j = 0;
                base = off;
                for (; j < 5; j++) {
                    cell = j + base;
                    p = obj + cell;
                    cell = p[0x5A8];
                    if (cell != 0) {
                        p[0x5A8] = (uint8_t)(cell - 1);
                    }
                    fn3 = *(cardgame_f560_fn3_t *)(ctx + 0xEA0);
                    fn3(ctx, i, j, p[0x5A8]);
                }
            }
        }
        *(int32_t *)(obj + 0x424) += 1;
        if (*(int32_t *)(obj + 0x424) > *(int32_t *)(obj + 0x430)) {
            found = 0;
            i2 = 0;
            off2 = 0;
            do {
                uint8_t *p2;
                j2 = 0;
                base2 = off2;
                do {
                    cell = j2 + base2;
                    p2 = obj + cell;
                    if (p2[0x5A8] != 0) {
                        found = 1;
                    }
                    j2++;
                } while (j2 < 5);
                i2++;
                off2 += 200;
            } while (i2 < 2);
            if (found != 0) {
                obj[0x423] = 1;
            } else {
                obj[0x423] = 2;
            }
        }
        break;
    }
    case 2:
        result = 1;
        break;
    }
    return result;
}
