/*
 * CARDGAME:0x80090ba0 CARDGAME_F0x80090ba0
 * 312 bytes at CARDGAME.PRO offset 0xdef0 (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x80090ba0
 *  Symbols     CARDGAME_F0x80084320=0x80084320
 *  Compare     312 bytes from 0x80090ba0 against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x80090ba0
 */

#include "common/types.h"

/*
 * corpo PAL de 312 B na base verificada 0x80082cb0.
 *
 * Caller unico CARDGAME_F0x80084320 @ 0x800846c8 passa (obj, ctx, idx).
 *
 * O PAL copia ent (v1->t1) e off (t0->t2) so no caminho tomado do primeiro
 * laco, depois do beq de n<0x28. Sem lares distintos nesse bloco, o cc1 nasce
 * off em t2 / ent em t0, omite as duas words e gera 304 B. row e base sao os
 * mesmos valores com homes de laco separados; nao ha pin de registrador,
 * volatile nem uso ficticio.
 */

typedef void (*CardBa0Cb)(uint8_t *ctx, int32_t idx);

void CARDGAME_F0x80090ba0(uint8_t *obj, uint8_t *ctx, int32_t idx) {
    int32_t off = idx * 200;
    uint8_t *ent = obj + off;
    int cnt0 = *(int16_t *)(ent + 0x5a6);
    int n;
    int v;
    int32_t i;
    uint8_t *q;

    *(uint32_t *)(obj + 0x42c) = 0;
    *(uint32_t *)(obj + 0x438) = cnt0;
    n = *(int16_t *)(ent + 0x5a0);
    if (n < 0x28) {
        uint8_t *row;
        int32_t base;
        row = ent;
        base = off;
        do {
            if (*(int8_t *)(obj + n + 0x46f) != 0) {
                v = *(int16_t *)(obj + ((n << 1) + base) + 0x5b0);
                *(int16_t *)(obj + ((*(int16_t *)(row + 0x5a6)) * 2 + base) + 0x600) = v;
                (*(int16_t *)(row + 0x5a6))++;
                *(uint32_t *)(obj + 0x42c) = 1;
            }
            n++;
        } while (n < 0x28);
    }
    i = 0x27;
    q = obj + i;
    do {
        *(q + 0x49e) = 0;
        q--;
        i--;
    } while (i >= 0);
    if (*(uint32_t *)(obj + 0x438) != 0) {
        if (idx == 0) {
            obj[0x499] = 5;
        } else {
            obj[0x499] = 0xb;
            obj[0x49c] = 1;
        }
    } else {
        if (idx == 0) {
            obj[0x499] = 0x11;
        } else {
            obj[0x499] = 0x12;
            obj[0x49c] = 1;
        }
    }
    ((CardBa0Cb)*(void **)(ctx + 0xec0))(ctx, idx);
    obj[0x422] = 1;
}
