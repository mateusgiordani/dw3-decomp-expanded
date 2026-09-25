// CARDGAME:0x8009eefc, corpo PAL de 336 B na base verificada 0x80082cb0.
// Selection-sort do vetor de indices pela chave halfword em base[index*2 + 0x50].
// flags em a3: bit0 troca struct de 2 bytes em 0x30a + index*2; bit1 troca
// o byte em 0x446 + (index - start). Bytes de referencia no off 0x1c24c.
// Frame 0x20, 6 saves (s0-s5). idx e copia real do parametro indices (PAL
// move s1,a1 depois do andi de start); sem isso start cai em a1 e flags&1 em s0.
// Compilador: psyq-gcc-2.8.1-sn32-4.0.0010 + aspsx-2.79, -O2 -G0, variante base.
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
