/*
 * CARDGAME:0x80085de8 CARDGAME_F0x80085de8
 * 336 bytes at CARDGAME.PRO offset 0x3138 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80085de8
 *  Symbols     (none)
 *  Compare     336 bytes from 0x80085de8 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80085de8
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * Overlay: CARDGAME, verified PAL-SLES-03936 base 0x80082cb0, file offset
 * 0x3138
 *
 * Selection sort on 40 card deck entries from index *(int16_t *)(ctx + 0x668)
 * to 39.
 *
 * Swaps uint16_t entries at +0x678 and 2-byte card records at +0x30a.
 *
 * Tail swaps the selected card at +0x41c with the cursor at +0x440.
 */

#include <stdint.h>

struct TwoBytes {
    char b[2];
};

void CARDGAME_F0x80085de8(int32_t ctx) {
    uint16_t h;
    struct TwoBytes tmp;
    int32_t i = *(int16_t *)(ctx + 0x668);
    int32_t b = ctx + 0x678;
    int16_t *pb;
    int32_t pa;
    int32_t j;
    int32_t q;
    int16_t *r;
    int16_t *qb;
    int32_t qa;

    if (i < 39) {
        pb = (int16_t *)((i * 2) + b);
        pa = (i * 2) + ctx;
        do {
            j = i + 1;
            if (j < 40) {
                q = pa;
                r = pb;
                qb = (int16_t *)((j * 2) + b);
                qa = (j * 2) + ctx;
                do {
                    if (*(uint8_t *)(q + 0x30a) > *(uint8_t *)(qa + 0x30a)) {
                        h = *r;
                        *r = *qb;
                        *qb = h;
                        tmp = *(struct TwoBytes *)(q + 0x30a);
                        *(struct TwoBytes *)(q + 0x30a) = *(struct TwoBytes *)(qa + 0x30a);
                        *(struct TwoBytes *)(qa + 0x30a) = tmp;
                    }
                    qb++;
                    j++;
                    qa += 2;
                } while (j < 40);
            }
            pb++;
            i++;
            pa += 2;
        } while (i < 39);
    }

    {
        int32_t idx = *(uint8_t *)(ctx + 0x41c);
        int32_t off = idx * 2;
        int32_t pi = ctx + off;
        int16_t *qbi;
        int16_t *pcb;

        tmp = *(struct TwoBytes *)((int32_t)(ctx + (*(int32_t *)(ctx + 0x440) << 1)) + 0x30a);
        qbi = (int16_t *)(off + b);
        *(struct TwoBytes *)((int32_t)(ctx + (*(int32_t *)(ctx + 0x440) << 1)) + 0x30a) = *(struct TwoBytes *)(pi + 0x30a);
        *(struct TwoBytes *)(pi + 0x30a) = tmp;

        pcb = (int16_t *)(*(int32_t *)(ctx + 0x440) * 2 + b);
        h = *pcb;
        *pcb = *qbi;
        *qbi = h;
        *(int32_t *)(ctx + 0x440) = idx;
    }
}
