/*
 * CARDGAME:0x8009eefc CARDGAME_F0x8009eefc
 * 336 bytes at CARDGAME.PRO offset 0x1c24c (overlay loaded at 0x80082cb0).
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
 *  Link        .text at 0x8009eefc
 *  Symbols     (none)
 *  Compare     336 bytes from 0x8009eefc against the PAL overlay
 *  Verify      python tools/card_verify.py --only CARDGAME:0x8009eefc
 */
/*
 * Recovery notes (kept from the recovery work; historical, not re-verified).
 *
 * corpo PAL de 336 B na base verificada 0x80082cb0.
 *
 * Selection-sort do vetor de indices pela chave halfword em base[index*2 +
 * 0x50].
 *
 * Frame 0x20, 6 saves (s0-s5). idx e copia real do parametro indices (PAL move
 * s1,a1 depois do andi de start); sem isso start cai em a1 e flags&1 em s0.
 *
 * Compilador: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, -O2 -G0, variante
 * base.
 */

#include <stdint.h>

typedef struct CardKeyView {
    uint8_t pad_50[0x50];
    int16_t keys[0x100];
} CardKeyView;

struct TwoBytes {
    char b[2];
};

void CARDGAME_F0x8009eefc(uint8_t *base, short *indices, uint32_t range, int32_t flags) {
    int32_t end = (int32_t)range >> 16;
    int32_t start = (int32_t)(range & 0xFFFF);
    short *idx = indices;
    int32_t outer;
    int32_t inner;

    for (outer = start; outer < end - 1; outer++) {
        for (inner = outer + 1; inner < end; inner++) {
            int16_t oi = idx[outer];
            int16_t ii = idx[inner];
            if (((CardKeyView *)base)->keys[oi] > ((CardKeyView *)base)->keys[ii]) {
                uint16_t tmp_idx = idx[outer];
                idx[outer] = idx[inner];
                idx[inner] = tmp_idx;
                if (flags & 1) {
                    struct TwoBytes tmp = *(struct TwoBytes *)((uint8_t *)base + outer * 2 + 0x30a);
                    *(struct TwoBytes *)((uint8_t *)base + outer * 2 + 0x30a) = *(struct TwoBytes *)((uint8_t *)base + inner * 2 + 0x30a);
                    *(struct TwoBytes *)((uint8_t *)base + inner * 2 + 0x30a) = tmp;
                }
                if (flags & 2) {
                    uint8_t to = *((uint8_t *)base + (outer - start) + 0x446);
                    uint8_t ti = *((uint8_t *)base + (inner - start) + 0x446);
                    *((uint8_t *)base + (outer - start) + 0x446) = ti;
                    *((uint8_t *)base + (inner - start) + 0x446) = to;
                }
            }
        }
    }
}
